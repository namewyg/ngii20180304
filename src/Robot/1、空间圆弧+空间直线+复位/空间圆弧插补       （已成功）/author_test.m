clear all;clc;
%figure
%��һ�飺[5,3,2;0,3,0;1,4,5]  %������ģ�x,y,z������
%�ڶ��飺[6,3,2;0,4,0;1,4,6]
axis equal %axis equal ����������Ķ���ϵ�������ֵͬ ,����λ������ͬ��
p1 = [6,3,2];
p2 = [0,4,0];
p3 = [1,4,6];
N = 100;
TA=rt2tr(angvec2r(pi/2,[0,0,1]),p1'); %rt2tr��t����ת����t����Ϊ��ξ���,������λ�ã�x,y,z��
TC=rt2tr(angvec2r(pi/4,[1,0,0]),p3');% angvec2r(theta, V)���ǵð�v����λ���Ĵ���v = v / norm(v)��:�ɵ�Ч��������ʾת��Ϊ��ת����ĺ���
TB=rt2tr(eye(3),p2'); 

% v=10;
% a=1;
%P=CirInterpolation2(p1,p2,p3,v,a,N)
P=CirInterpolation(p1,p2,p3,N)  %�ռ�Բ���岹���������ɲ岹������P
O=CalCircleCenter(p1,p2,p3);  %����Բ������O
r=Distance(O,p1);  %��Բ��O�����P1����뾶r
Cirtheta=CalCentralAngle(p1,p2,p3,O,r); % ����Բ��AOC��Բ�Ľ�AOC
circleinterp(TA,TC,r,O,Cirtheta,2,1,N); %circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
%          circleinterp() %��������Figureͼ��        %�ռ�Բ���岹
  
 