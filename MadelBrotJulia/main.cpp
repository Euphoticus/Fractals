
#include "MadelBrotJulia_cpu.h"

#include "MadelBrotJulia_gpu.h"

#include <string>  // to string

#include "EasyBMP.h"    // for bmp files



using namespace std;

// Right click on project ->  Build options  ->   Linker settngs  -> (in the "other linker settings" field,   type   EasyBMP.cpp     if code doesen't run)


void PrintImage(int iter, double* info, string ImageName ){
    // info  - x1, x2, y1, y2 , rwidth , rheight

    printf ("x: %.4f %.4f  y: %.4f %.4f       %.f x %.f \n",  info[0],  info[1],  info[2],  info[3],  info[4],  info[5]  );

long t = clock();
    info = CorrectRatio(info);      // i get corrected coordinates,  so i can update UI now
printf (" CorrectRatio:   %.ld ms \n", 1000* ( clock() - t)/CLOCKS_PER_SEC );

    printf ("x: %.4f %.4f  y: %.4f %.4f       %.f x %.f \n",  info[0],  info[1],  info[2],  info[3],  info[4],  info[5]  );

    int rwidth = info[4];
    int rheight = info[5];

//    unsigned char* ThePic[ 4 * rwidth * rheight ];   // 4 because it holds RGP and alpha
    unsigned char* ThePic = new unsigned char[ 4 * rwidth * rheight ];   // 4 because it holds RGP and alpha
    ThePic = CalculateMadel(iter , info, ThePic );


t = clock();
    BMP Image;
    Image.SetSize( rwidth , rheight );
    Image.SetBitDepth( 32 );	// color depth, 24 is quite high

// assign color values for each pixel
    for(int i = 0;  i < rwidth ; i++){
        for(int j =0;  j < rheight;  j++){

                int pos = 4* (i*rheight + j)  ;

                int j1 = rheight - j - 1;   // for bmp files we go from the top towards bottom  - otherwise image is inverted
                Image(i,j1)->Blue =int(  ThePic[ pos    ]);
                Image(i,j1)->Green=int(  ThePic[ pos + 1  ]);
                Image(i,j1)->Red = int(  ThePic[ pos + 2  ]);
                Image(i,j1)->Alpha=int(  ThePic[ pos + 3  ]);
        } // j
    } // i

cout << ImageName << endl;
printf (" image arrays:   %.ld ms \n", 1000* ( clock() - t)/CLOCKS_PER_SEC );

t = clock();		// Save the image
    Image.WriteToFile( ImageName.c_str() );// "output.bmp");        // c_str() to convert a string to const char *
printf (" image file:   %.ld ms \n", 1000* ( clock() - t )/CLOCKS_PER_SEC );

 }  // PrintImage



int main()
{
//   0.250105 , 0.250115 , -0.000000001 , 0.000000001
//   -0.13 , -0.125 , 0.98500 , 0.98883
//    -2.0 , 0.6 , -1.1 , 1.1

// double info[] = { -2.0 , 0.6 , 0 , 1.1 ,       1200,  900 };
                //  x1,  x2,  y1,  y2,         rwidth , rheight

 //  double info[] = { -0.1 , 0.1 , -0.1 , 0.1 ,       6000,  3000 };

  double info[] = { 0.2501 , 0.6 , -0.0000001 , 0.0000001,       1200,  900 };



    PrintImage(   300      , info ,   "output.bmp" );
                // iter    ,        ,   Image name


}

//  SEPERATE h FILE FOR GRAPHICAL PROCESSES calc


