#ifndef MadelBrotJulia_cpu_H
#define MadelBrotJulia_cpu_H


#include <iostream>   // debugging output
#include <cmath>  

#include <math.h>      // log


#include <ctime>   // measuring execution time

using namespace std;


double CalculatePoint(double x, double y, double xc, double yc, int iter){
		
		
    double x2 = x*x;
    double y2 = y*y;	// x is Real part,	y is imaginary part

    for(int i = 0; i< iter    ; i++){	// cycle the amount of iterations

        x2 = x*x;
        y2 = y*y;
		
			// Jeigu modulio kvadratas yra mazesnis uz 4,  tesiamas skaiciavimas iteracijom
        if(x2 + y2 < 4){	
            y =  2*x*y + yc;
            x = x2 - y2 + xc;
        }		// Modulio kvadratas didesnis uz 4 -   grazinamas iteracijos skaicius, ties kuriuo sustojo
        else{
            return (   i  + 1- log2(  log( x*x + y*y ) / 10000000   )   )  ;		
        }  // if


    }  // for i

			// visi ciklai baigti,  tai reiskia, kad sis taskas (iteraciju tikslumu) ....      Neigiama verte tam, kad tai butu galima atpazinti
    return -1;     //  x*x + y*y ; //- 0.000000000001 ;       
}  // CalculatePoint




		// Modifikuojamos koordinates, taip, kad nebutu jokiu istempimu, kai yra naudojama  widescreen  rezoliucija
double* CorrectRatio(   double* info  ){

  double x1 = info[0];      double x2 = info[1];
  double y1 = info[2];      double y2 = info[3];
  int rwidth = info[4];     int rheight = info[5];
		
		// mazas skaicius reiskia, kad viskas telpa,     o didelis kad reikia daugiau pikseliu siam intervalui
    double xLen = rwidth  / abs(x1 - x2);
    double yLen = rheight / abs(y1 - y2);

//    cout << xLen << endl << yLen << endl;
		// 
    if(xLen > yLen){        
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






			// Kiekvieno pikselio spalvos skaiciavimo funkcija 
unsigned char* CalculateMadel(int iter, double* info,  unsigned char* ThePic , bool ToJulia ){   
			// info -  x1 - x left,   x2 - x right,  y1 - y bottom,   y2 - y top,          rwidth,rheight - picture resolution

long t = clock();  					// bool T = true;


  double x1 = info[0];		// x1 - kairysis x asies sonas
  double x2 = info[1];		// x2 - desnysis x asies sonas
  double y1 = info[2];		// y1 - apatinis y asies sonas
  double y2 = info[3];		// y2 - virsutinis y asies sonas
  int rwidth = info[4];		// rezoliucija
  int rheight = info[5];
  double Jx = info[6];		// Julia C parametrai
  double Jy = info[7];

  
cout << 4 * rwidth * rheight / 1000 << " KB       " <<  4 * rwidth * rheight / 1000000 << " MB" << endl;

			// Suskaiciuojame, kiek pakis x ir y  per kiekviena iteracija - tai priklauso nuo rezoliucijos
    double xCh = abs(x1 - x2) / rwidth ;  
    double yCh = abs(y1 - y2) / rheight ;

			
    double yc = y1;  	// dabartine y korodinate ,  kuri bus didinama nuo y1 iki y2
    for(int j = 0;  j < rheight ; j++){
    
        double xc = x1;	  // dabartine x korodinate ,  kuri bus didinama nuo x1 iki x2
        for(int i = 0;  i < rwidth ; i++){

						// einame kas eilute, j -toji eilute dauginama is verciu skaicius toje eiluteje
						// 4 *   nes yra alfa, R, G , B				  , i - i kuria verte irasome toje eiluteje
            int pos = 4* (j*rwidth + i ) ;		
			
            ThePic[ pos + 3]=0;     // irasoma alfa verte
			
            double Rez = 0;		// 
            if(ToJulia){        //  Skaiciuojame Julia 
                Rez = CalculatePoint( xc , yc, Jx , Jy, iter) ;
             }else{         // SKaiciuojame Mandel
                Rez = CalculatePoint(0 , 0, xc, yc, iter) ;
             }

			if( Rez>0 ){   // Jei grazintoji verte yra didesne uz nuli  - mes piesiame uz mandelbrot figuros ribu
						// Rez < 1 & Rez > 0   ){      // Rez < 1    if(Rez < 4){            	
				
				Rez = fmod( Rez , 100 ) / 100  ;		// , 250 ) / 500 * 2;    // Rez = fmod( Rez , 1  );

				if( Rez < 0.1){	// 0.1  ->  10
					Rez = 10*Rez;	//normalised 0-1
					ThePic[ pos ]  = 100 + Rez*100;    // B
					ThePic[ pos+1 ] = 40;    	   // G
					ThePic[ pos+2 ] = 40;   	  // R
						
				}else if(Rez < 0.4 ){	// 0.3  -> 10/3
					Rez = 10/3*(Rez-0.1);	//normalised 0-1
					ThePic[ pos ]  = (1 - Rez) *200 ;     	// B
					ThePic[ pos+1 ] =  40+   Rez *215 ;    	   // G
					ThePic[ pos+2 ] =  40+   Rez *215 ;   	       // R
						
					}else if(Rez<0.7){	// 0.3  ->  10/3
						Rez = 10/3*(Rez-0.4);	//normalised 0-1
						ThePic[ pos ]  = 0;   // 255 ;     	// B
						ThePic[ pos+1 ] = 40 + (1-Rez)*215;    	   // G
						ThePic[ pos+2 ] = 255; //* (1 - 4/3*(Rez-0.625)  );   	       // R

					}else{  // 0.3  ->  10/3
						Rez = 10/3*(Rez-0.7);	//normalised 0-1
						ThePic[ pos ]  = Rez*100;   // 255 ;     	// B
						ThePic[ pos+1 ] = 40;    	   // G
						ThePic[ pos+2 ] = 40 + (1-Rez)*215; //* (1 - 4/3*(Rez-0.625)  );   	       // R
					 }
					 
		//  ( - pow(4*Rez -1  , 2) + 1 )* 255;   ThePic[ pos + 2 ] =  ( - (Rez-0.75)*(Rez-0.75) +0.5  ) * 255 ; //  * round( abs(1 - (Rez + 0.5) ) )  ;   //40 + fmod( Rez*80 , 215 );                                    // R

                }else{      // rezultatas mazesnis uz 0,  piesiame viduje mandelbrot figuros
                    ThePic[ pos ]=    0; 		//fmod( ( -1/Rez / 20000 - Rez * 20000 )   ,255) ;     // fmod( -1/Rez * 20 ,255) ;      //0;
                    ThePic[ pos + 1]=  0; 		//fmod( ( -1/Rez / 30000 - Rez * 30000 )   ,255) ;      // fmod( -1/Rez * 30 ,255) ;   // 0;
                    ThePic[ pos + 2]=  0;  		//fmod( ( -1/Rez / 8000  - Rez * 8000)   ,255) ;       // fmod( -1/Rez * 8 ,255)  ;    //0;
                }

            xc += xCh ;  // didiname x verte
        } // for j
		yc += yCh;  // didiname y verte
    } // for i


 printf (" for:   %.ld ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );

	return ThePic;

}   // CalculateMadel



#endif
