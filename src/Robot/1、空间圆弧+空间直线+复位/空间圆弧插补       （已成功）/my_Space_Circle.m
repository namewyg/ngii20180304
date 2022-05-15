clear all;
clc;
%先根据D-H表，建立机械手的各个连杆,以cm为单位
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
L5.qlim=[0,15/12*pi];  %待定
L6.qlim=[-pi,pi];           %OK

robot=SerialLink(L,'name','robot6');
robot.display();%显示D-H参数表
robot.plot([0 0 0 0 0 0 ], 'trail', '.r--');%显示六自由度机械手门位置状态

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%指定画圆弧的三点：起点、中间点、终点
%第一组：[5,3,2;0,3,0;1,4,5] 三点坐标（5,0,1）（3,3,4）（2,0,5）
%第二组：[6,3,2;0,4,0;1,4,6]
axis equal %将横轴纵轴的定标系数设成相同值 ,即单位长度相同，
%零位是（0.27,0,0.32）
p1 = [0.27,-0.15,0.22];
p2 = [0.37,-0.20,0.30];
p3 = [0.32,-0.25,0.25];
N = 100; %插补点的个数
P = CirInterpolation(p1,p2,p3,N);  %只是位置插补，无姿态和速度，生成插补点坐标P
t=[0:0.05:9.95];
for i=1:1:size(P,1)
    T_circle(:,:,i) = transl(P(i,:))*troty(-1/3*pi); 
end
Joints_circle = robot.ikine(T_circle);  %逆解得关节角数据
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%走完圆弧，再让它走一条直线，就走圆弧的起点与终点，这样既可保证在一个平面内
Tq = transl(0.32,-0.25,0.25)*troty(-1/3*pi);
Tz = transl(0.27,-0.15,0.22)*troty(-1/3*pi);
T_line = ctraj(Tq,Tz,length(t));
Joints_line = robot.ikine(T_line);  %逆解得关节角数据
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Joints_all = [Joints_circle;Joints_line];
view(3);
robot.plot(Joints_all , 'trail', '.r--');  %仿真动画

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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


%%%%%%%%%%这是另一种空间圆弧插补，位置+姿态+速度都有，但存在问题，由于时间关系，就先不理了
% TA = rt2tr(angvec2r(pi/2,[0,0,1]),p1'); %rt2tr（t）旋转矩阵t增广为齐次矩阵,即增加位置（x,y,z）
% TC = rt2tr(angvec2r(pi/4,[1,0,0]),p3');% angvec2r(theta, V)（记得把v做单位化的处理v = v / norm(v)）:由等效轴角坐标表示转换为旋转矩阵的函数
% TB = rt2tr(eye(3),p2'); 
% O = CalCircleCenter(p1,p2,p3);  %计算圆心坐标O
% r = Distance(O,p1);  %由圆心O和起点P1计算半径r
% Cirtheta = CalCentralAngle(p1,p2,p3,O,r); % 计算圆弧AOC的圆心角AOC
% T_circle2 = circleinterp(TA,TC,r,O,Cirtheta,2,1,N); %circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
% Joint_all = robot.ikine(T_circle2);  %逆解得关节角数据，非常重要的一步！
% view(3);
% robot.plot(Joint_all , 'trail', '.r--');  %仿真动画
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  空间圆弧插补函数
%  p = CirInterpolation(p1,p2,p3,N)
%  
%  p1 -- 圆弧起始点
%  p2 -- 圆弧中间点
%  p3 -- 圆弧终止点（注意p1,p2,p3这三点不在同一直线上）
%  N --  插补点数
%  p --  插补点矩阵 
%
%  算法原理：陈永明等《机器人的三种规则曲线插补算法》中的圆弧插补算法
%  作者：caleb     时间：2011-11-18
 
 function p = CirInterpolation(p1,p2,p3,N)
 
 a = p1;
 b = p2;
 c = p3;
 
 O = CalCircleCenter(a,b,c);          % 计算过a,b,c三点圆的圆心坐标O
 R = Distance(a,O);                   % 计算圆半径R
 theta = CalCentralAngle(a,b,c,O,R);  % 计算圆弧对应的圆心角theta
  
 % 计算圆弧平面的法向量n
 AB = a-b;
 BC = b-c;
 n = cross(AB,BC);
 
 u = n(1);
 v = n(2);
 w = n(3);
 
 delta = theta/(N-1);             % 步距角
 ds = delta*R;                    % 插补中圆弧切向移动距离
 E = ds/( R*sqrt(u.^2+v.^2+w.^2));
 G = R/sqrt(R.^2+ds.^2);
 
 xO = O(1);
 yO = O(2);
 zO = O(3);
 
 p(1,:) = a;          % 圆弧起始坐标
 p(N,:) = c;          % 圆弧结束坐标
 
 % 计算中间插补点
for i=1:1:N-2
     
  xi = p(i,1);
  yi = p(i,2);
  zi = p(i,3);
 
  mi = v*(zi-zO)-w*(yi-yO);
  ni = w*(xi-xO)-u*(zi-zO);
  li = u*(yi-yO)-v*(xi-xO);
   
  x = xO + G*(xi + E*mi-xO);
  y = yO + G*(yi + E*ni-yO);
  z = zO + G*(zi + E*li-zO);
 
  p(i+1,:) = [x y z]; 
   
end
 end