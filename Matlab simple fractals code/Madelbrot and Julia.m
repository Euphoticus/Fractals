
iter =  128 ;   %128 is enough to see preety zoomed   or  VERY zoomed in
#https://arachnoid.com/mandelbrot_set/

OutOfBounds =  9; %


function [x, y] = Interv( points, xfrom , xto ,  yfrom, yto )
   x = xfrom : abs(xto - xfrom) / points : xto   ;
   y = yfrom : abs(yto - yfrom) / points : yto   ;
   
end

% -1.75,0  +- 0.01 mini mandelbrot
%x = -1.78 :  0.0001 : -1.745 ;
%y = -0.02 : 0.0001 : 0.02 ;

%[x , y] = Interv( 100 , -1.8, -1.7    ,  -0.02 , 0.02);
%[x , y] = Interv( 200 , -1.78,  -1.745    ,  -0.02 , 0.02);



% 1/4  0 
%x = 1/4 - 0.3 : 0.005 : 1/4 + 0.3;
%y = -0.2  : 0.2 / (2*(length(x)-1)-1) : 0.2;

%Entire thing:
rezo = 0.006;  %0.03 %resolution
%x = -2 : rezo : 2 ;
%y = -2 : rezo : 2;

% more spots:   http://www.nahee.com/Derbyshire/manguide.html

%Julia:
rezo = 0.05;   %resolution
x = -2 : rezo : 2;
y = -2 : rezo : 2;




length(x)
length(y)

function TheM = main(x, y, iter, OutOfBounds)
  [X, Y] = meshgrid(x , y);

  TheM = zeros(length(y)  , length(x) );   %THe Matrix      we fill with zeroes
  % zero is the first iteration value

  SqOfBounds = sqrt(OutOfBounds);

%  h = waitbar(0, 'please wait ... or do not and be DOOOOOOOMED!')
  len = length(y);
  

  for i = 1:length(y)   %go through rows
    for j = 1:length(x)   %go through columns
    
      if( X(i,j)^2 + Y(i,j)^2 < 2^2)      %Check if we should even calculate
      
         %TheM(i,j) = CalcMandel( TheM(i,j) , X(i,j),  Y(i,j) , iter, SqOfBounds);
         TheM(i,j) = CalcJulia(  X(i,j) + Y(i,j) * 1i  , -0.75 , 2 ,  2,  iter, SqOfBounds);
                                              % rez    ,  C ,    n ,   ER , iter, sq...
      
       else     % We do not need to calculate, just assign large value
          TheM(i,j) = OutOfBounds;
       end  %else
       
    end %j
%    waitbar( i / len);
  end %i
  
%  close(h)
 
 
   
  %plot3(X,Y, TheM);
  %surf(X,Y, TheM);
  %contour( X, Y, TheM );  %real(TheM) )
  %imagesc(X,Y, TheM);


  colormap ("prism"); % colormap ("hsv");   %colormap ("jet");
  pcolor(X,Y, TheM);
  shading interp;
 
 
 
end %main


  
  
  
function rez = CalcMandel(rez, cr, ci, iter, SqOfBounds)

  for k = 1:iter   %Calulate the iterations
  
     sq = rez^2;
     
      if real(sq)^2 + imag(sq)^2 >= 16  %  4^2      %  abs(sq) >= 4     
          rez = SqOfBounds;
          break          
        else
           rez = sq + cr + 1i*ci;
        end  %end sq
        
   %        TheM(i,j) = abs( TheM(i,j) )^2 + X(i,j) + 1i*Y(i,j);
   end % interations k
   
   rez = real(rez)^2 + imag(rez)^2;  
       
end



function rez = CalcJulia( rez, C, n, ER,  iter, SqOfBounds)   
  ER = ER^2;
  for k = 1:iter   %Calulate the iterations
    
    rez = rez^n + C;  
    %rez = exp(rez) + C;  
    
    if real(rez)^2 + imag(rez)^2  >=  ER
       rez = SqOfBounds;
       break
     end  %end sq
   
   end % interations k
   rez = real(rez)^2 + imag(rez)^2;       
end



TheM = main(x, y, iter, OutOfBounds);

[X, Y] = meshgrid(x , y);   % For plotting from command lne after code has executed


 

