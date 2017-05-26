#ifndef MadelBrotJulia_cpu_H
#define MadelBrotJulia_cpu_H


#include <iostream>   // debugging output
#include <cmath>  

#include <math.h>      // log


#include <ctime>   // measuring execution time

using namespace std;





// Modifikuojamos koordinates, taip, kad nebutu jokiu istempimu,  kai yra naudojama  widescreen  rezoliucija
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

}   // CorrectRatio






//----------------------------------------------------------------------------------------------
        // Si funkcija gauna koordinates ir iteraciju skaiciu - ir su sia informacija skaiciuoja z^2 + C  iteracijas,  grazina iteracija ties kuria baige darba
double CalculatePoint(double x, double y, double xc, double yc, int iter){
        // xc, yc - Konstantos C Re ir Im dalys

    double x2 = 0;    // x - realioji dalis
    double y2 = 0; 	  // y - menamoji dalis

    for(int i = 0; i< iter    ; i++){   // ciklas per visas iteracijas

        x2 = x*x;       // skaiciuojame realiosios ir menamosio daliu kvadratus
        y2 = y*y;


        if(x2 + y2 < 4){    // Jeigu modulio kvadratas yra mazesnis uz 4,  tesiamas skaiciavimas iteracijom

            y =  2*x*y + yc;      // Skaiciuojame po iteracijos pasikeitusia  menamaja ir realiaja dalis atskirai
            x = x2 - y2 + xc;

        }		// Modulio kvadratas didesnis uz 4 -   grazinamas iteracijos skaicius, ties kuriuo sustojo
        else{
            return (   i  + 1- log2(  log( x*x + y*y ) / 10000000   )   )  ;    // iteracijos skaicius yra modifikuojamas, kad perejimas tarp spalvu butu tolydesnis
        }  // if


    }  // for i

    // po skaiciuotu ciklu, modulio kvadratas nebuvo didsnis uz 4
    return -1;                  //  x*x + y*y - 0.000000000001 ;

}  // CalculatePoint





//----------------------------------------------------------------------------------------------
// Duodamas rezultatas po iteraciju skaiciavimo,  is jo suskaiciuojamos spalvu vertes
int* GetColors(double Rez,   int* BGR){


if( Rez>0 ){   // Jei grazintoji verte yra didesne uz nuli  - mes piesiame uz mandelbrot figuros ribu

    Rez = fmod( Rez , 100 ) / 100  ;		// , 250 ) / 500 * 2;    // Rez = fmod( Rez , 1  );

    if( Rez < 0.1){	// 0.1  ->  10                  Melyneja        (is sviesiai melynos)
        Rez = 10*Rez;	//normalised 0-1

        BGR[ 0 ]  = 100 + Rez*100;    // B
        BGR[ 1 ] = 40;    	   // G
        BGR[ 2 ] = 40;   	  // R

    }else if(Rez < 0.4 ){	// 0.3  -> 10/3             geltoneja
        Rez = 10/3*(Rez-0.1);	//normalised 0-1

        BGR[ 0 ]  = (1 - Rez) *200 ;     	// B
        BGR[ 1 ] =  40+   Rez *215 ;    	   // G
        BGR[ 2 ] =  40+   Rez *215 ;   	       // R

        }else if(Rez<0.7){	// 0.3  ->  10/3            raudoneja
            Rez = 10/3*(Rez-0.4);	//normalised 0-1

            BGR[ 0 ]  = 0;                  	// B
            BGR[ 1 ] = 40 + (1-Rez)*215;      // G
            BGR[ 2 ] = 255;                    // R

        }else{  // 0.3  ->  10/3                        atgal i pradine sviesiai melyna
            Rez = 10/3*(Rez-0.7);	//normalised 0-1

            BGR[ 0 ]  = Rez*100;        	// B
            BGR[ 1 ] = 40;                  // G
            BGR[ 2 ] = 40 + (1-Rez)*215;    // R
         }

    }
    else{      // rezultatas mazesnis uz 0,  piesiame viduje mandelbrot figuros
        BGR[ 0 ]=  0; 		// 0, 0 ,0 - juoda spalva
        BGR[ 1 ]=  0;
        BGR[ 2 ]=  0;
    }

return BGR;
}










//----------------------------------------------------------------------------------------------
    // Kiekvieno pikselio spalvos pagrindine skaiciavimo funkcija
unsigned char* CalculateMadel(int iter, double* info,  unsigned char* ThePic , bool ToJulia ){

long t = clock();  					// bool T = true;


  double x1 = info[0];		// x1 - kairysis x asies sonas
  double x2 = info[1];		// x2 - desnysis x asies sonas
  double y1 = info[2];		// y1 - apatinis y asies sonas
  double y2 = info[3];		// y2 - virsutinis y asies sonas

  int rwidth = info[4];		// horizontali rezoliucija
  int rheight = info[5];    // vertikali rezoliucija

  double Jx = info[6];		// Julia C parametrai   realioji dalis
  double Jy = info[7];  // menamoji dalis


                //cout << "Estimation of memory needed for picture   " << 4 * rwidth * rheight / 1000 << " KB  " <<  4 * rwidth * rheight / 1000000 << " MB" << endl;

            // Suskaiciuojame, kiek pakis x ir y  per kiekviena iteracija - tai priklauso nuo rezoliucijos
    double xCh = abs(x1 - x2) / rwidth ;
    double yCh = abs(y1 - y2) / rheight ;



    for(int j = 0;  j < rheight ; j++){

        int BGR1[] = {0 ,0 , 0};             // inicializuojam spalvu masyva
        int* BGR = BGR1;

        double yc = y1 + yCh*j;              // dabartine y koordinate,  bus didinama nuo y1 iki y2
        double xc = x1;               // dabartine x korodinate ,  kuri bus didinama nuo x1 iki x2

        for(int i = 0;  i < rwidth ; i++){

                               // einame kas eilute, horizontaliai i virsu
                               // 4 *   nes yra alfa, R, G , B				  , i - I kuria verte irasome toje eiluteje
                                //  j -toji eilute dauginama is verciu skaicius toje eiluteje
            int pos = 4* (j*rwidth + i ) ;

                                 // ThePic[ pos + 3]=0;     // irasoma alfa verte (nebutina)

            double Rez = 0;		// inicializuojamas Rez

            if(ToJulia){                 //  Skaiciuojame Julia
                Rez = CalculatePoint( xc , yc, Jx , Jy, iter) ;
             }else{                      // SKaiciuojame Mandel
                Rez = CalculatePoint(0 , 0, xc, yc, iter) ;
             }
             xc += xCh ;                // didiname x verte


            BGR = GetColors(Rez  , BGR);        // perdave Rez, gauname atitinkamas spalvas

            ThePic[ pos ]  = BGR[0];   // B
            ThePic[ pos+1 ] = BGR[1];  // G
            ThePic[ pos+2 ] = BGR[2];  // R

        } // for j
    } // for i


 printf (" for:   %.ld ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );

    return ThePic;

}   // CalculateMadel



#endif
