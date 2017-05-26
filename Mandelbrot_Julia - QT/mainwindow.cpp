
#include "MadelBrotJulia_mcpu.h"
//#include "MadelBrotJulia_cpu.h"       // comment mcpu out and uncomment this - to launch single core version




#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "the_image.h"

#include <iostream> // output for debugging e.g
#include <fstream>


#include  <ctime>  // For saving of image - preventing overwrites
#include <iomanip>

#include <ctime>   // measuring execution time


// #include <string>  // to_string



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->Image_name ,    SIGNAL( ActivateMouseCoords(  QPoint&   ) ) ,
              this,             SLOT(  MouseCoords( QPoint&  )    )                   );       // added

    connect(ui->Image_name ,    SIGNAL(  ActivateZoom( QPoint& , bool) ) ,
              this,             SLOT(  TheZoom( QPoint& , bool )    )            );


    connect(ui->Image_name ,    SIGNAL( ActivateNewCenter(  QPoint& ) ) ,
              this,             SLOT(  NewCenter(  QPoint& )  )                  );

    GenerateImage(0 , 0, 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::GenerateImage(double x_C , double y_C, double Sc){     // This function generated image and shows it
              // mouse coordinates in pixels  Sc-  entire scale, entire square to calculate in

              // for Julia set
    double Jx, Jy = 0;
    if(ui->ToJulia->isChecked()){   Jx = ui->JulC_x->text().toDouble();
        Jy = ui->JulC_y->text().toDouble();
    }

             // Resolution info
    int rwidth = ui->RWidth->text().toInt();
    int rheight = ui->RHeight->text().toInt();

             // information that will be paseed to function that calculates each pixel
    double info1[] = { x_C - Sc,   x_C + Sc,    y_C - Sc , y_C + Sc,   (double)rwidth     , (double)rheight  , Jx , Jy  };
                    // x1,               x2,                 y1,            y2,                width ,      height

             // modify coordinates to take into account aspect ratio of resolution that may not be square
    double* info = info1;
    info = CorrectRatio(  info  );  // correct the aspect ratio

              // take the order of magnitude of scale   and    calculate it
    double n = ui->Scale_multiplier->text().toDouble();
    n = pow(10, n);
              // This is for aspect ratio - we save the part that doesen't fit into a scale box   - divide by n, so it's normalised just like   the Scale is  (that is for correct mouse possition coordinates later)
    MainWindow::ARx = ( info[1] - (x_C + Sc) ) / n;
    MainWindow::ARy = ( info[3] - (y_C + Sc) ) / n;

              // Resize the field that shows Image
    ui->Image_name->resize( rwidth , rheight );

              // initialise Image object        and     create a pointer to pixels that are initialised according to the resoltion
    QImage img = QImage(  rwidth  ,  rheight  ,   QImage::Format_RGB32);        // initialise image, tell what format and height, width
    unsigned char* ThePic = img.bits();         // assign char array that holds pixels

            // Take the iterations number    and   generate the pixels
    int iter = ui->Iterations->text().toInt();
    ThePic = CalculateMadel(   iter , info, ThePic  ,  ui->ToJulia->isChecked() );   // calculate  -  iterations, coordinates,  char array pointer,   Weather to calculate Julia or Mandel

// REMOVE LATER
//int pos = 4* (rheight/2*rwidth + rwidth/2 );                                            // FOR color smoothness         DELETE later
//printf("                B: %d  , G: %d  , R: %d \n" , ThePic[ pos  ] , ThePic[ pos+1], ThePic[pos+2] );

             // pass the image to QPixmap object     and  the  QPixmap object can pass it's data directly to the field which shows image
    QPixmap p = QPixmap::fromImage(img);
    ui->Image_name->setPixmap(p);
    ui->Image_name->show();

             // Save the image to file if user wants to   -  current time is used to prevent overwrites so entire journey is saved
    if(ui->ToSave->isChecked() ){       // every newly generated picture would be saved

        time_t T = time(NULL);       struct tm * currentT = localtime ( &T );
        std::string ST = asctime(currentT);

        ST = ST.substr(0 , ST.size() - 1 );
        ST = ST.substr(0 , 13) + ST.substr(14 , 2)+ ST.substr(17, ST.size()-17);

        ST = "Frac " + ST + ".png";         //   std::cout <<   ST  << std::endl ;
        p.save(  QString::fromUtf8( ST.data(), ST.size() )   );

    }
}




void MainWindow::TheZoom( QPoint &mouse_pos , bool ZoomIn){     // if scroll happened -  Zoom in/out and generate image

             //  Take current scale and multiplier
    double Sc0 = ui->Scale->text().toDouble();
            //prevent division by 0 in later parts
    if(Sc0==0){
        ui->Scale->setText("1");
        Sc0=1;
    }
    int n0 =ui->Scale_multiplier->text().toInt();

             // initialise new (after zoom in) scale   and  depending on mouse wheel movement - zoom in ir zoom out (multiply or divide by zoom factor)
    double Sc = 0;
    if( ZoomIn ){
       Sc = Sc0/  ui->Zoom_speed->text().toDouble();   // zoom in
    }else{
        Sc = Sc0 * ui->Zoom_speed->text().toDouble();   // zoom out
    }
             // We determine new scale's order of magnitude and update the  Scale without any order of magnitude
    int n = log10(Sc);
    Sc = Sc * pow(10 ,-n);                    // Scale number gets lowered/increased by that order of magnitude
    ui->Scale->setText(  QString("%1").arg( Sc  )   );   // Save normalised  scale number

             // update the Scale multiplier ,  because order of magnitude increased or decreased
    n =  n0  +  n;
   ui->Scale_multiplier->setText(   QString("%1").arg(n)     );    // save new   n


             // get the position of mouse in pixels and normalise it by dividing from image height or width
             // left side is 0  the right size is 1    the middle is 0.5     (same to   up = 0,   down = 1)
    double x01 = (double) mouse_pos.x()  / (ui->Image_name->width()  -1);
    double y01 = (double) mouse_pos.y()  / (ui->Image_name->height() -1);

            // Calculate what was the scale at the time, when mouse was clicked
   double Sc0x = (Sc0 + MainWindow::ARx) * std::pow(10 , n0 );
   double Sc0y = (Sc0 + MainWindow::ARy) * std::pow(10 , n0 );

            //depending on mouse position we move center coordinate
    double x_C0 = ui->CorX->text().toDouble()    +  (x01*2 - 1)*Sc0x;    // prievious center + ( x_01 * 2 - 1) * scale at that time
    double y_C0 = ui->CorY->text().toDouble()    +  (y01*2 - 1)*Sc0y;

            //We save the new center
    ui->CorX->setText(   QString::number( x_C0 , 'f' , -n+6)     );
    ui->CorY->setText(   QString::number( y_C0 , 'f' , -n+6)     );

            // Calculate current scale for Generate image   and   pass the current center
    Sc = Sc*pow(10,n);
    GenerateImage(x_C0 , y_C0, Sc);

}





void MainWindow::NewCenter(  QPoint & m_pos  ){     // if left click happened - recalculate new center and generate image

             // get the position of mouse in pixels and normalise it by dividing from image height or width
             // left side is 0  the right size is 1    the middle is 0.5     (same to   up = 0,   down = 1)
    double x = (double) m_pos.x() / (ui->Image_name->width()  -1);
    double y = (double) m_pos.y() / (ui->Image_name->height() -1);

             // get the information about the actual coordinates in a image field
    int n =  ui->Scale_multiplier->text().toInt();
    double Sc = ui->Scale->text().toDouble();
    double ScX = ( Sc  + MainWindow::ARx) * std::pow(10 , n);
    double ScY = ( Sc  + MainWindow::ARy) * std::pow(10 , n);

            // depending on the mouse position, we calculate the new center coordinates
    x = ScX*( x*2 - 1) + ui->CorX->text().toDouble();
    y = ScY*( y*2 - 1) + ui->CorY->text().toDouble();

            // We save the new coordinates of the center        (precision saved depends on the currenet order of magnitude)
    ui->CorX->setText(   QString::number( x , 'f' , -n+6)     );
    ui->CorY->setText(   QString::number( y , 'f' , -n+6)     );


             // Calculate current scale for Generate image   and   pass the current center
    Sc = Sc*pow(10,n);
    GenerateImage( x , y, Sc);

}





void MainWindow::MouseCoords( QPoint & m_pos ){     // If right click happened  show the mouse coordinates in the UI

            // get the position of mouse in pixels and normalise it by dividing from image height or width
            // left side is 0  the right size is 1    the middle is 0.5     (same to   up = 0,   down = 1)
    double x = (double) m_pos.x() / (ui->Image_name->width()  -1);
    double y = (double) m_pos.y() / (ui->Image_name->height() -1);


    int n =  ui->Scale_multiplier->text().toInt();
    double ScX = ( ui->Scale->text().toDouble()  + MainWindow::ARx) * std::pow(10 , n);   // Calculate current scale used
    double ScY = ( ui->Scale->text().toDouble()  + MainWindow::ARy) * std::pow(10 , n);


    ui->Coords->setText(    QString("x: %1  \ny: %2 ").arg(             // print position in the label
         QString::number(    ScX*( x*2 - 1) + ui->CorX->text().toDouble()   , 'f'  , -n +7),
         QString::number(    ScY*( y*2 - 1) + ui->CorY->text().toDouble()   , 'f'  , -n +7)
                                ) );      // Scale ( mouse * 2 - 1) + CurrentCoordinate
                                          // scale*2 * mouse - scale            where scale*2 is  scale Length

}

                // if( ui->Show_coords->isChecked() == true){          // Check weather user wants this feature

void MainWindow::on_ToJulia_clicked()       // when the checkbox "ToJulia" gets clicked - we switch what is held in the GUI   and call the generate image function to gerenate Julia or back to Mandelbrot
{
    if( ui->ToJulia->isChecked()  ){    // We  convert to Julia,  save the mandel info

                    // get mandel values
        double MxC = ui->CorX->text().toDouble();
        double MyC = ui->CorY->text().toDouble();
        int n = ui->Scale_multiplier->text().toInt();

                    // save mandel info
        MainWindow::MxC = MxC;
        MainWindow::MyC = MyC;
        MainWindow::MSc = ui->Scale->text().toDouble();
        MainWindow::MSc_mul = n;

                    // Jul UI appears,   assign mandel center point as a  C argument of Julia
        ui->JulC_x->show();
        ui->JulC_x->setText(  QString::number( MxC , 'f', -n+4 )  );
        ui->JulC_y->show();
        ui->JulC_y->setText(   QString::number( MyC , 'f', -n+4 )  );

                    // Set default values for Julia
        ui->CorX->setText("0");
        ui->CorY->setText("0");
        ui->Scale->setText("2");
        ui->Scale_multiplier->setText("0");
        GenerateImage( ui->CorX->text().toDouble(), ui->CorY->text().toDouble() ,
                      ui->Scale->text().toDouble() *pow(10 , ui->Scale_multiplier->text().toInt() )    );

    }else{
           //Set saved Manded values  to GUI
        int n = MainWindow::MSc_mul;
        ui->CorX->setText( QString::number( MainWindow::MxC , 'f', -n+4 )  );
        ui->CorY->setText( QString::number( MainWindow::MyC , 'f', -n+4 )  );
        ui->Scale->setText(   QString::number(   MainWindow::MSc  )    );
        ui->Scale_multiplier->setText(   QString::number(   n  )    );

            // hide unused boxes
        ui->JulC_x->hide();
        ui->JulC_y->hide();
        GenerateImage( ui->CorX->text().toDouble(), ui->CorY->text().toDouble() ,
                      ui->Scale->text().toDouble() *pow(10 , ui->Scale_multiplier->text().toInt() )    );
    }
}
