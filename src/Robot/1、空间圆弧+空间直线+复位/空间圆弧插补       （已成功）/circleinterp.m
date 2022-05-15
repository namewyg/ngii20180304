%�ռ�Բ���岹
%λ�ò岹�������߲岹����ʵ�֣���̬�岹����Ԫ������ʵ�֣���̬Ϊ���Ȳ岹���������м��
%��̬�岹��λ�ò岹�Ĳ岹�������ΪN+1
%T=function circleinterp(Cirtheta,v,a,N)
%Cirtheta:Բ�Ľ�,v:���������ٶ��ٶȣ������ͼӼ��٣���a�������߼��ٶȣ�N:�ֳ�N��

function T_circle2 = circleinterp(TA,TC,r,O,Cirtheta,v,a,N)
deltat=(Cirtheta/v+v/a)/N;
% for i=0:1:N
%       T(i+1)=deltat*i
% end
for i=0:1:N
    if  i*deltat>=0&&i*deltat<=v/a   %���ٽ׶�
        Ttheta(i+1)=a*(i*deltat)^2/2;  %��λ��=1/2at^2
    elseif  (i*deltat>v/a&&i*deltat<Cirtheta/v)
        Ttheta(i+1)=(v^2)/(2*a)+(i*deltat-v/a)*v; %��λ��=1/2at^2+V0*t (t=v/a)
    else
        Ttheta(i+1)=Cirtheta-a*((Cirtheta/v+v/a)-i*deltat)^2/2; %ͬ��
     end 
end
% plot(T,Ttheta)
[RA,tA]=tr2rt(TA); %tr2rt��t����t2r��t������ξ���t��ά����ת����+λ������  
[RC,tC]=tr2rt(TC);
middle=unit((tA'-O));  %δ�鵽��Ӧ���ǵ�Ԫ����λ��������
approach=unit(cross(tA'-O,tC'-O));
orientation=unit(cross(approach,middle));
Rtr=oa2r(approach,orientation);  %ת������ͽӽ���������ת����
for i=1:N+1
     Pre(:,i)=[r*cos(Ttheta(i)),r*sin(Ttheta(i)),0]; %��x��y����
     t(:,i)=Pre(:,i)'*Rtr;  %δ�鵽
end

R=RA\RC;
Qmiddle=double(UnitQuaternion(R));%UnitQuaternion�ǵ�λ��Ԫ�����������ڱ�ʾ��̬��
qtheta=Qmiddle(1);
v=[Qmiddle(2),Qmiddle(3),Qmiddle(4)];
deltatheta=qtheta/N;
for i=0:1:N
    Qtheta(i+1)=i*deltatheta;
end
for i=1:N+1
Rtr=angvec2r(Qtheta(i),v);  %�ɵ�Ч��������ʾת��Ϊ��ת����ĺ���
R(:,:,i)=RA*Rtr;
end

for i=1:N+1
T_circle2(:,:,i)=rt2tr(R(:,:,i),t(:,i));  %��ת����תΪ��α任����
% trplot(T(:,:,i)) %trplot(T):��������α任����T��ʾ����ά����ϵ������TΪ4��4�ľ���
hold on
end
