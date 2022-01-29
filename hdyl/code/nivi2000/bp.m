clear all;
load('C:\Users\test1.txt');
k = (test1)/100;
max=max(k);
sum = 0;
len = size(k(:,1))
for i = 1:1:len(1)
 if(k(i,1)>=max)
    initial_index = i;
 end
end
% for i = initial_index:1:len(1)
%  if(k(i,1)<=10)
%     stop_index = i;
%     break;
%  end
% end
y = 0;
%len = stop_index-initial_index;
for i =1:1:len(1)
    y(i,1)= k(i,1);
end
%initial_index = 13625;

smooth_size = 100
smooth_size2 = 150
for i = initial_index:1:len(1)-smooth_size
    for j = i+1:1:i+smooth_size
        sum =sum+ y(j);
    end
    sfiler(i,1)= sum/smooth_size;
sum=0;
end
for i = initial_index:1:len(1)-smooth_size
error_fileter2(i,1) = sfiler(i,1)-y(i);
end
error_fileter3 = error_fileter2;
for i = initial_index:1:len(1)-100-smooth_size2
if(abs(error_fileter2(i,1))>4)
   
     for j = i:1:smooth_size2+i
     error_fileter2(j,1)=0;
     end
     if(i>smooth_size2)
     for j = i-smooth_size2/5:1:i
     error_fileter2(j,1)=0;
     end
     else
     for j = initial_index:1:i
     error_fileter2(j,1)=0;
     end         
     end
end;
end
windowSize = 10; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
error_fileter4 = filter(b,a,error_fileter2);

figure (1);
plot(k,'black');
%figure (2);
hold on
plot(error_fileter3,'b');
%figure (3);
hold on
plot(error_fileter2,'r');
hold on

plot(error_fileter4,'g');
XN=fft(error_fileter4);

Ts = 1/10;
y = fft(XN);   
fs = 1/Ts;
f = (0:length(y)-1)*fs/length(y);
figure (4);
plot(f,abs(y))
xlabel('Frequency (Hz)')
ylabel('Magnitude')
title('Magnitude')