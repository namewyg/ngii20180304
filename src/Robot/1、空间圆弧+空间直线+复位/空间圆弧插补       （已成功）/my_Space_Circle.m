clear all;
clc;
%�ȸ���D-H��������е�ֵĸ�������,��cmΪ��λ
L1=Link('d',0.2,'a',0,'alpha',pi/2); 
L2=Link('d',0,'a',0.22,'alpha',0,'offset',pi/2);
L3=Link('d',0,'a',0,'alpha',pi/2);
L4=Link('d',0.27,'a',0,'alpha',-pi/2);
L5=Link('d',0,'a',0,'alpha',-pi/2,'offset',-pi/2);
L6=Link('d',-0.1,'a',0,'alpha',0);
L=([L1 L2 L3 L4 L5 L6]);

%%%�趨���ؽ�ת����Χ
L1.qlim=[-5/6*pi,5/6*pi];  %OK
L2.qlim=[-2/3*pi,2/3*pi];  %OK
L3.qlim=[-1/3*pi, pi];      %����
L4.qlim=[-pi,pi];           %OK
L5.qlim=[0,15/12*pi];  %����
L6.qlim=[-pi,pi];           %OK

robot=SerialLink(L,'name','robot6');
robot.display();%��ʾD-H������
robot.plot([0 0 0 0 0 0 ], 'trail', '.r--');%��ʾ�����ɶȻ�е����λ��״̬

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%ָ����Բ�������㣺��㡢�м�㡢�յ�
%��һ�飺[5,3,2;0,3,0;1,4,5] �������꣨5,0,1����3,3,4����2,0,5��
%�ڶ��飺[6,3,2;0,4,0;1,4,6]
axis equal %����������Ķ���ϵ�������ֵͬ ,����λ������ͬ��
%��λ�ǣ�0.27,0,0.32��
p1 = [0.27,-0.15,0.22];
p2 = [0.37,-0.20,0.30];
p3 = [0.32,-0.25,0.25];
N = 100; %�岹��ĸ���
P = CirInterpolation(p1,p2,p3,N);  %ֻ��λ�ò岹������̬���ٶȣ����ɲ岹������P
t=[0:0.05:9.95];
for i=1:1:size(P,1)
    T_circle(:,:,i) = transl(P(i,:))*troty(-1/3*pi); 
end
Joints_circle = robot.ikine(T_circle);  %���ùؽڽ�����
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%����Բ������������һ��ֱ�ߣ�����Բ����������յ㣬�����ȿɱ�֤��һ��ƽ����
Tq = transl(0.32,-0.25,0.25)*troty(-1/3*pi);
Tz = transl(0.27,-0.15,0.22)*troty(-1/3*pi);
T_line = ctraj(Tq,Tz,length(t));
Joints_line = robot.ikine(T_line);  %���ùؽڽ�����
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Joints_all = [Joints_circle;Joints_line];
view(3);
robot.plot(Joints_all , 'trail', '.r--');  %���涯��

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
delete (instrfindall);  %����ϵͳ֮ǰ�Ѿ��򿪵Ĵ�����Դ��������
Port = serial('COM8');
% set(Port,'BaudRate',115200,'DataBits', 8, 'Parity', 'none','StopBits', 1, 'FlowControl', 'none','Terminator','CR/LF');
set(Port,'BaudRate',115200) ;  %���ò�����
set(Port,'inputBufferSize',4072000 ); %�������뻺����Ϊ3M
set(Port,'outputBufferSize',4072000 ); %�������������Ϊ3M
set(Port,'Timeout', 20); %����һ�ζ����д�����������ʱ��Ϊ0.5��
set(Port,'Terminator','LF'); %���ڴ����ַ�Ϊ"���з�"
fopen(Port); 
pause(2);  %%�����ʱ������Ҫ�����˾Ͳ��ԣ�����

 for i=1:1:size(Joints_all,1)  %����q������
    for j=1:1:size(Joints_all,2)  %����q������
        if (j==size(Joints_all,2)) %д��һ�к���
            fprintf(Port,'%.4f\n',Joints_all(i,j) );%д��ÿ�����һ�����ݺ���
        else 
            fprintf(Port,'%.4f\t',Joints_all(i,j) );%ÿ�����ݼ��ÿո����
        end
        pause(0.01);  %��ʱ�࣬�����������ʱ�٣����ݻ��ң��ᶪ����
    end
end
delete (instrfindall);  %����ϵͳ֮ǰ�Ѿ��򿪵Ĵ�����Դ��������


%%%%%%%%%%������һ�ֿռ�Բ���岹��λ��+��̬+�ٶȶ��У����������⣬����ʱ���ϵ�����Ȳ�����
% TA = rt2tr(angvec2r(pi/2,[0,0,1]),p1'); %rt2tr��t����ת����t����Ϊ��ξ���,������λ�ã�x,y,z��
% TC = rt2tr(angvec2r(pi/4,[1,0,0]),p3');% angvec2r(theta, V)���ǵð�v����λ���Ĵ���v = v / norm(v)��:�ɵ�Ч��������ʾת��Ϊ��ת����ĺ���
% TB = rt2tr(eye(3),p2'); 
% O = CalCircleCenter(p1,p2,p3);  %����Բ������O
% r = Distance(O,p1);  %��Բ��O�����P1����뾶r
% Cirtheta = CalCentralAngle(p1,p2,p3,O,r); % ����Բ��AOC��Բ�Ľ�AOC
% T_circle2 = circleinterp(TA,TC,r,O,Cirtheta,2,1,N); %circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
% Joint_all = robot.ikine(T_circle2);  %���ùؽڽ����ݣ��ǳ���Ҫ��һ����
% view(3);
% robot.plot(Joint_all , 'trail', '.r--');  %���涯��
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  �ռ�Բ���岹����
%  p = CirInterpolation(p1,p2,p3,N)
%  
%  p1 -- Բ����ʼ��
%  p2 -- Բ���м��
%  p3 -- Բ����ֹ�㣨ע��p1,p2,p3�����㲻��ͬһֱ���ϣ�
%  N --  �岹����
%  p --  �岹����� 
%
%  �㷨ԭ���������ȡ������˵����ֹ������߲岹�㷨���е�Բ���岹�㷨
%  ���ߣ�caleb     ʱ�䣺2011-11-18
 
 function p = CirInterpolation(p1,p2,p3,N)
 
 a = p1;
 b = p2;
 c = p3;
 
 O = CalCircleCenter(a,b,c);          % �����a,b,c����Բ��Բ������O
 R = Distance(a,O);                   % ����Բ�뾶R
 theta = CalCentralAngle(a,b,c,O,R);  % ����Բ����Ӧ��Բ�Ľ�theta
  
 % ����Բ��ƽ��ķ�����n
 AB = a-b;
 BC = b-c;
 n = cross(AB,BC);
 
 u = n(1);
 v = n(2);
 w = n(3);
 
 delta = theta/(N-1);             % �����
 ds = delta*R;                    % �岹��Բ�������ƶ�����
 E = ds/( R*sqrt(u.^2+v.^2+w.^2));
 G = R/sqrt(R.^2+ds.^2);
 
 xO = O(1);
 yO = O(2);
 zO = O(3);
 
 p(1,:) = a;          % Բ����ʼ����
 p(N,:) = c;          % Բ����������
 
 % �����м�岹��
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