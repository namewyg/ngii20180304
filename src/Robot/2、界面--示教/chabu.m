clc; clear all;
%先根据D-H表，建立机械手的各个连杆
L1=Link('d',0.2,'a',0,'alpha',pi/2); 
L2=Link('d',0,'a',0.22,'alpha',0,'offset',pi/2);
L3=Link('d',0,'a',0,'alpha',pi/2);
L4=Link('d',0.27,'a',0,'alpha',-pi/2);
L5=Link('d',0,'a',0,'alpha',-pi/2,'offset',-pi/2);
L6=Link('d',-0.1,'a',0,'alpha',0);
L=([L1 L2 L3 L4 L5 L6]);
%%%设定各关节转动范围
L1.qlim=[-5/6*pi,5/6*pi];  %OK
L2.qlim=[-2/3*pi,2/3*pi];  %OK
L3.qlim=[-1/3*pi, pi];      %待定
L4.qlim=[-pi,pi];           %OK
L5.qlim=[-1/12*pi,14/12*pi];  %待定
L6.qlim=[-pi,pi];           %OK
robot = SerialLink(L,'name','六自由度教学机器人');
view(3);
robot.plot([0 0 0 0 0 0 ], 'trail', '.r--');

Shi_Jiao_ALL_Points = [
                       pi/2,pi/8,pi/9,pi/10,pi/15,pi/16;
                       pi/4,pi/9,pi/10,pi/11,pi/15,pi/16;
                       pi/12,pi/12,pi/13,pi/14,pi/15,pi/16  ];

 %直接调用app.Shi_Jiao_ALL_Points
 for i = 1:1:size(Shi_Jiao_ALL_Points,1)
     T = robot.fkine( Shi_Jiao_ALL_Points(i,:) );
     %fkine（）正解出来是SE3类型，无奈只能先将其转为4x4double
     T1 = tr2rt (T) ;%获得T的旋转矩阵
     T2 = transl(T); %获得T的平移坐标XYZ
     T_shijiao(:,:,i) = [T1,T2';[0 0 0 1]];%合并为4x4的齐次变换矩阵
 end
 %下面对示教点进行插补
T_all = [];
for i = 1:1:size(Shi_Jiao_ALL_Points,1)-1  
   %直接采用ctraj对两点之间进行两两插补
   t = [0:0.01:1];
   T_chabu = ctraj(T_shijiao(:,:,i) , T_shijiao(:,:,i+1) , length(t));
   T_all = cat( 3, T_all , T_chabu ) ;  %将两个三维矩阵进行拼接
end
Joints_all = robot.ikine( T_all );
view(3);
% robot.plot( Joints_all, 'trail', '.r--' );  % 仿真动画  

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
delete (instrfindall);  %消除系统之前已经打开的串口资源；不可少
Port = serial('COM8');
% set(Port,'BaudRate',115200,'DataBits', 8, 'Parity', 'none','StopBits', 1, 'FlowControl', 'none','Terminator','CR/LF');
set(Port,'BaudRate',115200) ;  %设置波特率
set(Port,'inputBufferSize',4072000 ); %设置输入缓冲区为3M
set(Port,'outputBufferSize',4072000 ); %设置输出缓冲区为3M
set(Port,'Timeout', 20); %设置一次读入或写入操作完成最大时间为0.5秒
set(Port,'Terminator','LF'); %串口触发字符为"换行符"
fopen(Port); 
pause(2);  %%这个延时极其重要，少了就不对！！！

 for i=1:1:size(Joints_all,1)  %矩阵q的行数
    for j=1:1:size(Joints_all,2)  %矩阵q的列数
        if (j==size(Joints_all,2)) %写完一行后换行
            fprintf(Port,'%.4f\n',Joints_all(i,j) );%写完每行最后一个数据后换行
        else 
            fprintf(Port,'%.4f\t',Joints_all(i,j) );%每个数据间用空格隔开
        end
        pause(0.01);  %延时多，传输很慢；延时少，数据会乱，会丢数据
    end
end
delete (instrfindall);  %消除系统之前已经打开的串口资源；不可少

