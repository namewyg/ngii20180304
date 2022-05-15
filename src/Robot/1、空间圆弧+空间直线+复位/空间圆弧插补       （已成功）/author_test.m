clear all;clc;
%figure
%第一组：[5,3,2;0,3,0;1,4,5]  %三个点的（x,y,z）坐标
%第二组：[6,3,2;0,4,0;1,4,6]
axis equal %axis equal 将横轴纵轴的定标系数设成相同值 ,即单位长度相同，
p1 = [6,3,2];
p2 = [0,4,0];
p3 = [1,4,6];
N = 100;
TA=rt2tr(angvec2r(pi/2,[0,0,1]),p1'); %rt2tr（t）旋转矩阵t增广为齐次矩阵,即增加位置（x,y,z）
TC=rt2tr(angvec2r(pi/4,[1,0,0]),p3');% angvec2r(theta, V)（记得把v做单位化的处理v = v / norm(v)）:由等效轴角坐标表示转换为旋转矩阵的函数
TB=rt2tr(eye(3),p2'); 

% v=10;
% a=1;
%P=CirInterpolation2(p1,p2,p3,v,a,N)
P=CirInterpolation(p1,p2,p3,N)  %空间圆弧插补函数，生成插补点坐标P
O=CalCircleCenter(p1,p2,p3);  %计算圆心坐标O
r=Distance(O,p1);  %由圆心O和起点P1计算半径r
Cirtheta=CalCentralAngle(p1,p2,p3,O,r); % 计算圆弧AOC的圆心角AOC
circleinterp(TA,TC,r,O,Cirtheta,2,1,N); %circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
%          circleinterp() %用于生成Figure图像        %空间圆弧插补
  
 