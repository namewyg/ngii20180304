/*********************************************************************
 * INCLUDES
 */

#include <stdio.h>
#include <string.h>
#include "AF.h"
#include "OnBoard.h"
#include "OSAL_Tasks.h"
#include "SampleApp.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "hal_drivers.h"
#include "hal_key.h"
#if defined ( LCD_SUPPORTED )
  #include "hal_lcd.h"
#endif
#include "hal_led.h"
#include "hal_uart.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "ds18b20.h" 
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#if !defined( SAMPLE_APP_PORT )
#define SAMPLE_APP_PORT  0
#endif

#if !defined( SAMPLE_APP_BAUD )
  #define SAMPLE_APP_BAUD  HAL_UART_BR_115200
#endif

// When the Rx buf space is less than this threshold, invoke the Rx callback.
#if !defined( SAMPLE_APP_THRESH )
#define SAMPLE_APP_THRESH  64
#endif

#if !defined( SAMPLE_APP_RX_SZ )
#define SAMPLE_APP_RX_SZ  128
#endif

#if !defined( SAMPLE_APP_TX_SZ )
#define SAMPLE_APP_TX_SZ  128
#endif

// Millisecs of idle time after a byte is received before invoking Rx callback.
#if !defined( SAMPLE_APP_IDLE )
#define SAMPLE_APP_IDLE  6
#endif

// Loopback Rx bytes to Tx for throughput testing.
#if !defined( SAMPLE_APP_LOOPBACK )
#define SAMPLE_APP_LOOPBACK  FALSE
#endif

// This is the max byte count per OTA message.
#if !defined( SAMPLE_APP_TX_MAX )
#define SAMPLE_APP_TX_MAX  80
#endif

#define SAMPLE_APP_RSP_CNT  4

// This list should be filled with Application specific Cluster IDs.
const cId_t SampleApp_ClusterList[SAMPLE_MAX_CLUSTERS] =
{
  SAMPLEAPP_MPU6050_CLUSTERID,
  SAMPLEAPP_TEMP_CLUSTERID,
};

const SimpleDescriptionFormat_t SampleApp_SimpleDesc =
{
  SAMPLEAPP_ENDPOINT,              //  int   Endpoint;
  SAMPLEAPP_PROFID,                //  uint16 AppProfId[2];
  SAMPLEAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  SAMPLEAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  SAMPLEAPP_FLAGS,                 //  int   AppFlags:4;
  SAMPLE_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList,  //  byte *pAppInClusterList;
  SAMPLE_MAX_CLUSTERS,          //  byte  AppNumOutClusters;
  (cId_t *)SampleApp_ClusterList   //  byte *pAppOutClusterList;
};

endPointDesc_t SampleApp_epDesc =
{
  SAMPLEAPP_ENDPOINT,
 &SampleApp_TaskID,
  (SimpleDescriptionFormat_t *)&SampleApp_SimpleDesc,
  noLatencyReqs
};

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
devStates_t SampleApp_NwkState;   
uint8 SampleApp_TaskID;           // Task ID for internal task/event processing.
/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint8 SampleApp_MsgID;

afAddrType_t SampleApp_Periodic_DstAddr; //广播
afAddrType_t SampleApp_Flash_DstAddr;    //组播
afAddrType_t SampleApp_P2P_DstAddr;      //点播


static afAddrType_t SampleApp_TxAddr;
static uint8 SampleApp_TxSeq;
static uint8 SampleApp_TxBuf[SAMPLE_APP_TX_MAX+1];
static uint8 SampleApp_TxLen;

static afAddrType_t SampleApp_RxAddr;
static uint8 SampleApp_RxSeq;
static uint8 SampleApp_RspBuf[SAMPLE_APP_RSP_CNT];
static uint8 gMpu6050InitOk=0;
uint8 deviceID=3;//ID取值分别为1，2，3，1表示是温度MPU6050　2表示温度DS18B20节点1　3表示温度DS18B20节点2


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void SampleApp_ProcessMSGCmd( afIncomingMSGPacket_t *pkt );
void SampleApp_CallBack(uint8 port, uint8 event); 
static void SampleApp_Send_MPU6050_Message( void );
static void SampleApp_Send_Temp_Message( void );



/*********************************************************************
 * @fn      SampleApp_Init
 *
 * @brief   This is called during OSAL tasks' initialization.
 *
 * @param   task_id - the Task ID assigned by OSAL.
 *
 * @return  none
 */
void SampleApp_Init( uint8 task_id )
{
  uint8 res=0;
  uint8 buff[20]={0};
  halUARTCfg_t uartConfig;

  SampleApp_TaskID = task_id;
  SampleApp_RxSeq = 0xC3;
  SampleApp_NwkState = DEV_INIT;       

  MT_UartInit();                  //串口初始化
  MT_UartRegisterTaskID(task_id); //注册串口任务
  afRegister( (endPointDesc_t *)&SampleApp_epDesc );
  RegisterForKeys( task_id );
  
  SampleApp_Periodic_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;//广播
  SampleApp_Periodic_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;

  // Setup for the flash command's destination address - Group 1
  SampleApp_Flash_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;//组播
  SampleApp_Flash_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Flash_DstAddr.addr.shortAddr = SAMPLEAPP_FLASH_GROUP;
  
  SampleApp_P2P_DstAddr.addrMode = (afAddrMode_t)Addr16Bit; //点播 
  SampleApp_P2P_DstAddr.endPoint = SAMPLEAPP_ENDPOINT; 
  SampleApp_P2P_DstAddr.addr.shortAddr = 0x0000;            //发给协调器
  
}

/*********************************************************************
 * @fn      SampleApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events   - Bit map of events to process.
 *
 * @return  Event flags of all unprocessed events.
 */
UINT16 SampleApp_ProcessEvent( uint8 task_id, UINT16 events )
{
  (void)task_id;  // Intentionally unreferenced parameter
  
  if ( events & SYS_EVENT_MSG )
  {
    afIncomingMSGPacket_t *MSGpkt;

    while ( (MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID )) )
    {
      switch ( MSGpkt->hdr.event )
      {
      case AF_INCOMING_MSG_CMD:
        SampleApp_ProcessMSGCmd( MSGpkt );
        break;
        
      case ZDO_STATE_CHANGE:
        SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
        if (// (SampleApp_NwkState == DEV_ZB_COORD)||
            //(SampleApp_NwkState == DEV_ROUTER)||
             (SampleApp_NwkState == DEV_END_DEVICE) )
        {
            
            osal_start_timerEx( SampleApp_TaskID,
                              SAMPLEAPP_SEND_PERIODIC_MSG_EVT,
                              SAMPLEAPP_SEND_PERIODIC_MSG_TIMEOUT );
        }
        else
        {
          // Device is no longer in the network
        }
        break;

      default:
        break;
      }

      osal_msg_deallocate( (uint8 *)MSGpkt );
    }

    return ( events ^ SYS_EVENT_MSG );
  }

  if ( events & SAMPLEAPP_SEND_PERIODIC_MSG_EVT )
  {
    if(deviceID==1)
    {
        //采集mpu6050的数据上传协调器 
        SampleApp_Send_MPU6050_Message();

        //重新启动定定时器，定时的时间是 300 ms
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_SEND_PERIODIC_MSG_EVT,300 );
    }
    else
    {
        //采集终端的温度上传
        SampleApp_Send_Temp_Message();

        //重新启动定定时器，定时的时间是 1000 ms
        osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_SEND_PERIODIC_MSG_EVT, 1000 );
    }

    // return unprocessed events
    return (events ^ SAMPLEAPP_SEND_PERIODIC_MSG_EVT);
  }


  return ( 0 );  // Discard unknown events.
}

/*********************************************************************
 * @fn      SerialApp_ProcessMSGCmd
 *
 * @brief   Data message processor callback. This function processes
 *          any incoming data - probably from other devices. Based
 *          on the cluster ID, perform the intended action.
 *
 * @param   pkt - pointer to the incoming message packet
 *
 * @return  TRUE if the 'pkt' parameter is being used and will be freed later,
 *          FALSE otherwise.
 */
void SampleApp_ProcessMSGCmd( afIncomingMSGPacket_t *pkt )
{
  uint8 buff[40]={0};

  switch ( pkt->clusterId )
  {
  // 接收终端上传的欧拉角
  case SAMPLEAPP_MPU6050_CLUSTERID: 
    {
        short aacx,aacy,aacz;
        short gyrox,gyroy,gyroz;
      
        aacx=(short)BUILD_UINT16(pkt->cmd.Data[0], pkt->cmd.Data[1]);
        aacy=(short)BUILD_UINT16(pkt->cmd.Data[2], pkt->cmd.Data[3]);
        aacz=(short)BUILD_UINT16(pkt->cmd.Data[4], pkt->cmd.Data[5]);
      
        gyrox=(short)BUILD_UINT16(pkt->cmd.Data[6], pkt->cmd.Data[7]);
        gyroy=(short)BUILD_UINT16(pkt->cmd.Data[8], pkt->cmd.Data[9]);
        gyroz=(short)BUILD_UINT16(pkt->cmd.Data[10], pkt->cmd.Data[11]);


        sprintf(buff, "A:%.02f,%.02f,%.02f.\r\n",(double)((double)aacx/16384.0),(double)((double)aacy/16384.0),(double)((double)aacz/16384.0));
        HalUARTWrite(0, buff, osal_strlen(buff));//串口输出加速度

        sprintf(buff, "G:%.02f,%.02f,%.02f.\r\n",(double)((double)gyrox/131.0),(double)((double)gyroy/131.0),(double)((double)gyroz/131.0));
        HalUARTWrite(0, buff, osal_strlen(buff));//串口输出陀螺仪
    }
    break;

  case SAMPLEAPP_TEMP_CLUSTERID:
    {
        uint8 id=pkt->cmd.Data[0];//取出ID
        uint8 z=pkt->cmd.Data[1];//温度的整数
        uint8 x=pkt->cmd.Data[2];//温度的小数

        if(x>=10) x=0;//小数是0~9

        if(id==2)//温度节点1数据
        {        
            sprintf(buff, "T2:%d.%d.\r\n",z,x);
            HalUARTWrite(0, buff, osal_strlen(buff));//串口输出温度
        }
        else if(id==3)//温度节点2数据
        {
            sprintf(buff, "T3:%d.%d.\r\n",z,x);
            HalUARTWrite(0, buff, osal_strlen(buff));//串口输出温度
        }
    }
    break;

    default:
      break;
  }
}


/*********************************************************************
 * @fn      SampleApp_CallBack
 *
 * @brief   Send data OTA.
 *
 * @param   port - UART port.
 * @param   event - the UART port event flag.
 *
 * @return  none
 */
void SampleApp_CallBack(uint8 port, uint8 event)
{
  (void)port;

  if ((event & (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL | HAL_UART_RX_TIMEOUT)) &&
#if SAMPLE_APP_LOOPBACK
      (SampleApp_TxLen < SAMPLE_APP_TX_MAX))
#else
      !SampleApp_TxLen)
#endif
  {
    SampleApp_TxLen += HalUARTRead(SAMPLE_APP_PORT, SampleApp_TxBuf+SampleApp_TxLen+1,
                                                      SAMPLE_APP_TX_MAX-SampleApp_TxLen);
  }
}

void SampleApp_Send_MPU6050_Message( void )
{
  float pitch,roll,yaw; 		//欧拉角
  uint8 buff[50]={0};
  uint8 len=0;
  short aacx,aacy,aacz;
  short gyrox,gyroy,gyroz;

  P1_0=!P1_0;//闪灯

  if(!gMpu6050InitOk)
  {
    MPU_Init();	//初始化MPU6050
    if(mpu_dmp_init()>0)
    {
      HalLcdWriteString("MPU6050 ERROR", HAL_LCD_LINE_2); //LCD显示第二行
      return;
    }
    else
    {
      gMpu6050InitOk=1;
    }
  }
  
  if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
  {
     //得到加速度传感器数据
     //返回0表示数据成功读取
    if(MPU_Get_Accelerometer(&aacx,&aacy,&aacz)!=0) return;

    //得到陀螺仪数据
    //返回0表示数据成功读取
    if(MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz)!=0) return;    

  
    //LCD显示
    {
    sprintf(buff, "P:%d",(int)pitch);
    HalLcdWriteString(buff, HAL_LCD_LINE_2); //LCD显示第二行

    sprintf(buff, "R:%d",(int)roll);
    HalLcdWriteString(buff, HAL_LCD_LINE_3); //LCD显示第三行

    sprintf(buff, "Y:%d",(int)yaw);
    HalLcdWriteString(buff, HAL_LCD_LINE_4); //LCD显示第四行
    }

    //以下为打包数据上传协调器


    //加速度
    buff[0]=LO_UINT16((uint16)aacx);
    buff[1]=HI_UINT16((uint16)aacx);
    buff[2]=LO_UINT16((uint16)aacy);
    buff[3]=HI_UINT16((uint16)aacy);
    buff[4]=LO_UINT16((uint16)aacz);
    buff[5]=HI_UINT16((uint16)aacz);

    //陀螺仪
    buff[6]=LO_UINT16((uint16)gyrox);
    buff[7]=HI_UINT16((uint16)gyrox);
    buff[8]=LO_UINT16((uint16)gyroy);
    buff[9]=HI_UINT16((uint16)gyroy);
    buff[10]=LO_UINT16((uint16)gyroz);
    buff[11]=HI_UINT16((uint16)gyroz);

    len=12;



    //无线发送到协调器
    if ( AF_DataRequest( &SampleApp_P2P_DstAddr, &SampleApp_epDesc,
                         SAMPLEAPP_MPU6050_CLUSTERID,
                         len,
                         buff,
                         &SampleApp_MsgID,
                         AF_DISCV_ROUTE,
                         AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
    {
    }
  }
}


void SampleApp_Send_Temp_Message( void )
{
    uint8 str[5]={0};//第一字节为温度的整数，第二字节为温度的小数
    uint8 strTemp[20]={0};
    float tempFloat;   //带小数的湿度
    int len=0;

    //终端2或者终端3是温度
    if(!(deviceID==2|| deviceID==3)) return;

    tempFloat = floatReadDs18B20();                //带1位小数温度数据

    str[0] = deviceID;//ID
    str[1] = (uint8)tempFloat;//取出温度的整数
    str[2] = (uint8)((int)(tempFloat*10)%10);//取出温度的小数

    len=3;

    //LCD显示
    sprintf(strTemp, "TEMP:%d.%d C", str[1],str[2]);
    HalLcdWriteString(strTemp, HAL_LCD_LINE_3); 

    //无线发送到协调器
    if ( AF_DataRequest( &SampleApp_P2P_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_TEMP_CLUSTERID,
                       len,
                       str,
                       &SampleApp_MsgID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
    {
    }
    else
    {
    // Error occurred in request to send.
    }
}

