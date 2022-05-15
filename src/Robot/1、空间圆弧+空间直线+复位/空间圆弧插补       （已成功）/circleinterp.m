%空间圆弧插补
%位置插补用抛物线插补方法实现，姿态插补用四元数方法实现，姿态为均匀插补，不考虑中间点
%姿态插补和位置插补的插补点个数都为N+1
%T=function circleinterp(Cirtheta,v,a,N)
%Cirtheta:圆心角,v:抛物线匀速段速度（即梯型加减速），a：抛物线加速度，N:分成N份

function T_circle2 = circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
deltat=(Cirtheta/v+v/a)/N;
% for i=0:1:N
%       T(i+1)=deltat*i
% end
for i=0:1:N
    if  i*deltat>=0&&i*deltat<=v/a   %加速阶段
        Ttheta(i+1)=a*(i*deltat)^2/2;  %角位移=1/2at^2
    elseif  (i*deltat>v/a&&i*deltat<Cirtheta/v)
        Ttheta(i+1)=(v^2)/(2*a)+(i*deltat-v/a)*v; %角位移=1/2at^2+V0*t (t=v/a)
    else
        Ttheta(i+1)=Cirtheta-a*((Cirtheta/v+v/a)-i*deltat)^2/2; %同理
     end 
end
% plot(T,Ttheta)
[RA,tA]=tr2rt(TA); %tr2rt（t），t2r（t）把齐次矩阵t降维成旋转矩阵+位置坐标  
[RC,tC]=tr2rt(TC);
middle=unit((tA'-O));  %未查到，应该是单元化或单位向量函数
approach=unit(cross(tA'-O,tC'-O));
orientation=unit(cross(approach,middle));
Rtr=oa2r(approach,orientation);  %转换方向和接近向量到旋转矩阵
for i=1:N+1
     Pre(:,i)=[r*cos(Ttheta(i)),r*sin(Ttheta(i)),0]; %求x、y坐标
     t(:,i)=Pre(:,i)'*Rtr;  %未查到
end

R=RA\RC;
Qmiddle=double(UnitQuaternion(R));%UnitQuaternion是单位四元数函数，用于表示姿态的
qtheta=Qmiddle(1);
v=[Qmiddle(2),Qmiddle(3),Qmiddle(4)];
deltatheta=qtheta/N;
for i=0:1:N
    Qtheta(i+1)=i*deltatheta;
end
for i=1:N+1
Rtr=angvec2r(Qtheta(i),v);  %由等效轴角坐标表示转换为旋转矩阵的函数
R(:,:,i)=RA*Rtr;
end

for i=1:N+1
T_circle2(:,:,i)=rt2tr(R(:,:,i),t(:,i));  %旋转矩阵转为齐次变换矩阵
% trplot(T(:,:,i)) %trplot(T):绘制由齐次变换矩阵T表示的三维坐标系，其中T为4×4的矩阵。
hold on
end
