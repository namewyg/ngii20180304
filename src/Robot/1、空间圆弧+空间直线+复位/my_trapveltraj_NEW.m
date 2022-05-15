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

%目前第4和第6关节存在突变，先传过去动一个看看耐不耐得住，耐不住再说！
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % 第二段：走目标轨迹，此处为走Z型(YOZ平面)，
% wpts = [ 0.27 ,  0.27  , 0.27  , 0.27  ;
%          0 ,  -0.3 , 0  , -0.3 ;
%          0.32 ,  0.32  , 0.12 , 0.12 ; ]; 

% % 走Z型(XOY平面)，
% wpts = [ 0.27 ,  0.27  , 0.20  , 0.20  ;
%          0 ,    -0.10 ,  0  , -0.10 ;
%          0.32 ,  0.32  , 0.32 , 0.32 ; ]; 

% 走单条直线(XOY平面)，
wpts = [ 0.27 ,  0.27   ;
         0 ,    -0.10 ;
         0.32 ,  0.32 ; ]


% %%不走Z了，先走个多段直线看看
% wpts = [ 0.27 ,  0.27, 0.27, 0.27 ;
%          0 ,  -0.3 , -0.3, 0  ;
%          0.32 ,  0.32  , 0.1 , 0.1  ;]; 

%初始时间是0s，终点时间是1s
tpts = [0:3];
%仿真所用的时长为1s，仿真时间间隔为0.1s
tvec = 0:0.01:3;
%%用梯形加减速
[q,qd,qdd,tvec,pp] = trapveltraj(wpts,length(tvec));

for i=1:1:length(tvec)
    Ts_2(:,:,i) = transl(q(:,i));  %末端位姿绕y轴转过-90°，使末端正对待刷墙面 
end
Joints_2 = robot.ikine(Ts_2);  
view(3);
robot.plot( Joints_2, 'trail', '.r--' );  % 仿真动画  
%  plot(tvec,qd(1,:))

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

 for i=1:1:size(Joints_2,1)  %矩阵q的行数
    for j=1:1:size(Joints_2,2)  %矩阵q的列数
        if (j==size(Joints_2,2)) %写完一行后换行
            fprintf(Port,'%.4f\n',Joints_2(i,j) );%写完每行最后一个数据后换行
        else 
            fprintf(Port,'%.4f\t',Joints_2(i,j) );%每个数据间用空格隔开
        end
        pause(0.01);  %延时多，传输很慢；延时少，数据会乱，会丢数据
    end
end
delete (instrfindall);  %消除系统之前已经打开的串口资源；不可少





