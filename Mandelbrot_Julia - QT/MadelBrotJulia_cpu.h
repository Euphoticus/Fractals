#ifndef MadelBrotJulia_cpu_H
#define MadelBrotJulia_cpu_H


#include <iostream>
#include <cmath>

#include <math.h>      // log


//#include <vector>

// spalva pagal tai, kokioje iteracijoje sustojo -   paskutineje juoda,   per viduri, kazkokia spalvota spalva,    is pradziu - balta

// galima issaugoti praeita iteracija,  ir skaiciuojant prizoominta skaiciuoti daugiau tasku


//#include <chrono>   // measuring execution time
#include <ctime>

// Right click on project ->  Build options  ->   Linker settngs  -> (in the "other linker settings" field,   type   EasyBMP.cpp     if code doesen't run)

using namespace std;

//  It would be faster to pass large array possibly by reference, than to make a copy - but i need to initialise it correctly  http://www.learncpp.com/cpp-tutorial/74a-returning-values-by-value-reference-and-address/



double CalculatePoint(double x, double y, double xc, double yc, int iter){

    double x2 = x*x;
    double y2 = y*y;	// x is Real part,	y is imaginary part

    for(int i = 0; i< iter    ; i++){	// cycle through amount of iterations

        x2 = x*x;
        y2 = y*y;

        if(x2 + y2 < 4){
            y =  2*x*y + yc;
            x = x2 - y2 + xc;
        }
        else{
          //  return  double(i); // / double(iter);
           // return fmod(  ( i - log2(  log(x*x + y*y)/log( pow(10, 100)) )  )/300  , 0.999 )  ;

            return (   i + 1 - log2(  log(x*x + y*y) / 100000   )   )/ iter  ;
            //break;
        }  // if


    }  // for i

    return - ( x*x + y*y ); //- 0.000000000001 ;       //    return x*x + y*y
}  // CalculatePoint





double* CorrectRatio(   double* info  ){

  double x1 = info[0];      double x2 = info[1];
  double y1 = info[2];      double y2 = info[3];
  int rwidth = info[4];     int rheight = info[5];

    double xLen = rwidth  / abs(x1 - x2);
    double yLen = rheight / abs(y1 - y2);

//    cout << xLen << endl << yLen << endl;

    if(xLen > yLen){        // small number means that every2hing fits,  larger that it needs more pixels for that interval
        double Points  = rwidth / yLen ;
        Points = ( Points - abs(x1 - x2) ) / 2 ;
         info[0] = x1 - Points;       info[1] = x2 + Points;
    }
    else{
        double Points  = rheight / xLen ;
        Points = ( Points - abs(y1 - y2) ) / 2 ;
        info[2] = y1 - Points;       info[3] = y2 + Points;
    }  // if

   return info;
}







unsigned char* CalculateMadel(int iter, double* info,  unsigned char* ThePic ){
    // x1 - x left,   x2 - x right,  y1 - y bottom,   y2 - y top,      iter - iterations,    rwidth,rheight - picture resolution

 long t = clock();  // bool T = true;


  double x1 = info[0];
  double x2 = info[1];
  double y1 = info[2];
  double y2 = info[3];
  int rwidth = info[4];
  int rheight = info[5];

    cout << 4 * rwidth * rheight / 1000 << " KB       " <<  4 * rwidth * rheight / 1000000 << " MB" << endl;

    //int InfinityValue = 9;  // placeholder  value  that is assigned for infinity points

    double xCh = abs(x1 - x2) / rwidth ;  // the change of x for each iteration
    double yCh = abs(y1 - y2) / rheight ;

	
    double yc = y1;  // y current
    for(int j = 0;  j < rheight ; j++){
    
        double xc = x1;		// x current coordinate         , it goes from x1,  up to x2
        for(int i = 0;  i < rwidth ; i++){

            int pos = 4* (j*rwidth + i ) ;		// go through every width line, that's why line 2 will start at 2*rwidth 
            ThePic[ pos + 3]=0;     // alpha

  /*          if(xc*xc + yc*yc >= 4){      //  if current point is outside of circle of r = 2,  then it is no longer Madelbrot set
                ThePic[ pos ]= 100; // 100;
                ThePic[ pos + 1 ]= 40;    //0;
                ThePic[ pos + 2 ]= 40;   // 0;
            }else{ */		// The point is within circle of r=2,   we calculate weather that point diverges or stays under 2

   			   double Rez = CalculatePoint(0 , 0, xc, yc, iter) ;
         //       cout<< Rez  << endl;
                if( Rez>0 ){        // Rez < 1 & Rez > 0   ){      // Rez < 1    if(Rez < 4){            	// Paint outside

					//Rez = fmod ( Rez/100 , 1)  ;
                    Rez = fmod( 2*Rez , 1  );

                    if( Rez < 0.1){	// 0.125  ->  8
                            Rez = 10*Rez;	//normalised 0-1
                            ThePic[ pos ]  = 100 + Rez*100;    // B
                            ThePic[ pos+1 ] = 40;    	   // G
                            ThePic[ pos+2 ] = 40;   	  // R
							
                    }else if(Rez < 0.5 ){	// 0.4  -> 5/2
                            Rez = 5/2*(Rez-0.1);	//normalised 0-1
                            ThePic[ pos ]  = (1 - Rez) *200 ;     	// B
                            ThePic[ pos+1 ] =  40+   Rez *215 ;    	   // G
                            ThePic[ pos+2 ] =  40+   Rez *215 ;   	       // R
							
                            }else if(Rez<0.8){	// 0.3  ->  10/3
                                Rez = 10/3*(Rez-0.5);	//normalised 0-1
                                ThePic[ pos ]  = 0;   // 255 ;     	// B
                                ThePic[ pos+1 ] = 40 + (1-Rez)*215;    	   // G
                                ThePic[ pos+2 ] = 255; //* (1 - 4/3*(Rez-0.625)  );   	       // R

                            }else{  // 0.2  ->  5
                                Rez = 5*(Rez-0.8);	//normalised 0-1
                                ThePic[ pos ]  = Rez*100;   // 255 ;     	// B
                                ThePic[ pos+1 ] = 40;    	   // G
                                ThePic[ pos+2 ] = 40 + (1-Rez)*215; //* (1 - 4/3*(Rez-0.625)  );   	       // R
                             }
	
                   
										//  ( - pow(4*Rez -1  , 2) + 1 )* 255;   ThePic[ pos + 2 ] =  ( - (Rez-0.75)*(Rez-0.75) +0.5  ) * 255 ; //  * round( abs(1 - (Rez + 0.5) ) )  ;   //40 + fmod( Rez*80 , 215 );                                    // R

                }else{          // paint inside
                    ThePic[ pos ]=    0; //fmod( ( -1/Rez / 20000 - Rez * 20000 )   ,255) ;     // fmod( -1/Rez * 20 ,255) ;      //0;
                    ThePic[ pos + 1]=  0; //fmod( ( -1/Rez / 30000 - Rez * 30000 )   ,255) ;      // fmod( -1/Rez * 30 ,255) ;   // 0;
                    ThePic[ pos + 2]=  0;  //fmod( ( -1/Rez / 8000  - Rez * 8000)   ,255) ;       // fmod( -1/Rez * 8 ,255)  ;    //0;
                }
    //        } //  if(xc*xc + yc*yc >= 4){  

            xc += xCh ;  // x increase
        } // for j
		yc += yCh;  // y increase
    } // for i


 printf (" for:   %.ld ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );

 return ThePic;

}   // CalculateMadel



#endif
