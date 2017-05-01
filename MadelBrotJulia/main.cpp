
#include "MadelBrotJulia_cpu.h"

#include <string>  // to string


using namespace std;

// Right click on project ->  Build options  ->   Linker settngs  -> (in the "other linker settings" field,   type   EasyBMP.cpp     if code doesen't run)


int main()
{
     //cout << "Hello world!" << endl;
    //return 0;

 //   CalculateMadel( 300,  -2.0, 0.6 , -1.1 , 1.1 , 900,      900    ,  "output.bmp" );
              //    iter,   x1,  x2,    y1,   y2,   rwidth ,  rheight  ,  name of output file

    CalculateMadel( 300, -0.13 , -0.125 , 0.98500 , 0.98883  , 2000 ,     1000 ,  "output.bmp" );
                      // 0.250105 , 0.250115 , -0.000000001 , 0.000000001

/*   for(int i = 256; i < 400 ; i=i+1   ){
        CalculateMadel( i,  0.250105 , 0.250115 , -0.000000001 , 0.000000001  , 1360 ,      720 ,  "t" + to_string(i) + ".bmp" );
    }
*/
// tick tock

}

//  SEPERATE h FILE FOR GRAPHICAL PROCESSES calc


