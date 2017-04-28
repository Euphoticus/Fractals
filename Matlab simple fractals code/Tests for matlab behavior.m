test1 = 0^2 + 1 + 1i*2
test2 = abs(test1)^2 + 1 + 1i*2
real(test2)
imag(test2)




format compact
c = 1 + 1i;

iter = 10;
y = zeros(1,iter);

for i = 2:iter
    %y(i) = y(i-1)^2 + c;
    y(i) = real(y(i-1))^2 + imag(y(i-1))^2 + c;
    y(i)
end  

%y
y(length(y))


abs(1+1i)^2
(1+1i)^2


