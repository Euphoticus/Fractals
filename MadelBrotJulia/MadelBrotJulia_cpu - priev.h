#ifndef MadelBrotJulia_cpu_H
#define MadelBrotJulia_cpu_H


#include <iostream>
#include <cmath>
#include <vector>

// spalva pagal tai, kokioje iteracijoje sustojo -   paskutineje juoda,   per viduri, kazkokia spalvota spalva,    is pradziu - balta

// galima issaugoti praeita iteracija,  ir skaiciuojant prizoominta skaiciuoti daugiau tasku

//#include <tuple>    // return multiple arguments

#include "EasyBMP.h"    // for bmp files

//#include <chrono>   // measuring execution time
#include <ctime>

// Right click on project ->  Build options  ->   Linker settngs  -> (in the "other linker settings" field,   type   EasyBMP.cpp     if code doesen't run)

using namespace std;

//  It would be faster to pass large array possibly by reference, than to make a copy - but i need to initialise it correctly  http://www.learncpp.com/cpp-tutorial/74a-returning-values-by-value-reference-and-address/


/*
tuple<double, double, double, double> CorrectRatio(    double x1, double x2, double y1, double y2, int rwidth , int rheight  ){



    double x1en = rwidth  / abs(x1 - x2);
    double yLen = rheight / abs(y1 - y2);


    if(x1en > yLen){        // small number means that every2hing fits,  larger that it needs more pixels for that interval
        double Points  = rwidth / yLen ;
        Points = ( Points - abs(x1 - x2) ) / 2 ;
        x1 = x1 - Points;       x2 = x2 + Points;
    }
    else{
        double Points  = rheight / x1en ;
        Points = ( Points - abs(y1 - y2) ) / 2 ;
        y1 = y1 - Points;       y2 = y2 + Points;
    }  // if


    return make_tuple(x1, x2, y1, y2);

}  // CorrectRatio
*/


double CalculatePoint(double xc, double yc, int iter,  int InfinityValue){
    double x, y = 0;	// x is Real part,	y is imaginary part


    for(int i = 0; i< iter    ; i++){	// cycle through amount of iterations

        double x2 = x*x;
        double y2 = y*y;

        if(x2 + y2 < 4){
            y =  2*x*y + yc;
            x = x2 - y2 + xc;
        }
        else{
            return InfinityValue;
            break;
        }  // if

    }  // for i

    return  x*x + y*y;

}  // CalculatePoint




void CalculateMadel(int iter, double x1, double x2, double y1, double y2,   int rwidth , int rheight  ,  string ImageName){
    // x1 - x left,   x2 - x right,  y1 - y bottom,   y2 - y top,      iter - iterations,    rwidth,rheight - picture resolution

 long t = clock();   bool T = true;


    printf ("x: %.4f %.4f  y: %.4f %.4f       %.dx%.d \n", x1, x2, y1, y2, rwidth, rheight );

//    tie(x1, x2, y1, y2) = CorrectRatio( x1, x2 , y1, y2,  rwidth , rheight);	// correct coordinates and aspect


    double x1en = rwidth  / abs(x1 - x2);
    double yLen = rheight / abs(y1 - y2);


    if(x1en > yLen){        // small number means that every2hing fits,  larger that it needs more pixels for that interval
        double Points  = rwidth / yLen ;
        Points = ( Points - abs(x1 - x2) ) / 2 ;
        x1 = x1 - Points;       x2 = x2 + Points;
    }
    else{
        double Points  = rheight / x1en ;
        Points = ( Points - abs(y1 - y2) ) / 2 ;
        y1 = y1 - Points;       y2 = y2 + Points;
    }  // if


    printf ("x: %.4f %.4f  y: %.4f %.4f       %.dx%.d \n", x1, x2, y1, y2, rwidth, rheight );

    int InfinityValue = 9;  // placeholder  value  that is assigned for infinity points



// Initialise things:
   t = clock();
    vector< vector<double> > ThePic(rwidth ,  vector<double>(rheight ) );		// array that holds each pixel, point

    double xCh = abs(x1 - x2) / rwidth ;  // the change of x for each iteration
    double yCh = abs(y1 - y2) / rheight ;


    double xc = x1;    // x current coordinate    ,  it goes from x1,  up to x2

    for(int i = 0;  i < rwidth ; i++){

        double yc = y1;  // y current
        for(int j = 0;  j < rheight ; j++){

            if(xc*xc + yc+yc >= 4){      //  if current point is outside of circle of r = 2,  then it is no longer Madelbrot set
                ThePic[i][j] = InfinityValue ;
            }
            else{			// The point is within circle of r=2,   we calculate weather that point diverges or stays under 2
                ThePic[i][j] = CalculatePoint(xc, yc, iter , InfinityValue) ;
            }

            yc += yCh;  // y increase
        } // for j

        xc += xCh ;  // x increase
    } // for i

 if(T==true){printf (" for:   %.d ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );}


 t = clock();

    BMP Image;
    Image.SetSize( rwidth , rheight );
    Image.SetBitDepth( 24 );	// color depth, 24 is quite high


	// assign color values for each pixel
    for(int i = 0;  i < rwidth ; i++){
            for(int j = rheight-1;  j >= 0 ; j--){	// for bmp files we go from the top towards bottom  - otherwise image is inverted

            int j1 = rheight - j;
            if(ThePic[i][ j1 ] > 4  ){          //||  i == 2 || j1 == 2){  // == InfinityValue){         //i == j ){          //(ThePic[i][j] == InfinityValue){
                Image(i,j)->Red=255;
                Image(i,j)->Green=255;
                Image(i,j)->Blue=255;
//                Image(i,j)->Alpha=255;
            }else{
				// Many colors version
                Image(i,j)->Red=  fmod(ThePic[i][j1] * 20 ,2) *255 ; //(ThePic[i][j] * 200000 % 255   ;          // up to 128
                Image(i,j)->Green= fmod(ThePic[i][j1]* 30 ,0.3) *255 ;
                Image(i,j)->Blue= fmod(ThePic[i][j1]* 8 ,1.4) *255  ;
				// black and white version
//                Image(i,j)->Alpha=0;
/*              Image(i,j)->Red=0;
                Image(i,j)->Green=0;
                Image(i,j)->Blue=0;
//                Image(i,j)->Alpha=0;  */
            }
        } // j
    } // i

 cout << ImageName << endl;

 if(T==true){printf (" image arrays:   %.d ms \n", 1000* ( clock() - t)/CLOCKS_PER_SEC );}

 t = clock();		// Save the image
    Image.WriteToFile( ImageName.c_str() );// "output.bmp");        // c_str() to convert a string to const char *
 if(T==true){printf (" image file:   %.d ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );}

}   // CalculateMadel



#endif
