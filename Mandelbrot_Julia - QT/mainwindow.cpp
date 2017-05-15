#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "the_image.h"

#include "MadelBrotJulia_cpu.h"

#include <iostream> // output for debugging e.g
#include <fstream>


#include  <ctime>  // For saving of image - preventing overwrites
#include <iomanip>

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




void MainWindow::GenerateImage(double x_C , double y_C, double Sc){      // mouse coordinates in pixels  Sc-  entire scale

    double Jx, Jy = 0;
    if(ui->ToJulia->isChecked()){   Jx = ui->JulC_x->text().toDouble();
        Jy = ui->JulC_y->text().toDouble();
    }
    int rwidth = ui->RWidth->text().toInt();
    int rheight = ui->RHeight->text().toInt();
    double info1[] = { x_C - Sc,   x_C + Sc,    y_C - Sc , y_C + Sc,   (double)rwidth     , (double)rheight  , Jx , Jy  };
                    // x1,               x2,                 y1,            y2,                width ,      height

    double* info = info1;
    info = CorrectRatio(  info  );  // correct the aspect ratio

    double n = ui->Scale_multiplier->text().toDouble();
    n = pow(10, n);

    MainWindow::ARx = ( info[1] - (x_C + Sc) ) / n;     // The amount that doesen't fit into a scale box gets saved there
    MainWindow::ARy = ( info[3] - (y_C + Sc) ) / n;


    ui->Image_name->resize( rwidth , rheight );

    QImage img = QImage(  rwidth  ,  rheight  ,   QImage::Format_RGB32);        // initialise image, tell what format and height, width
    unsigned char* ThePic = img.bits();         // assign char array that holds pixels


    int iter = ui->Iterations->text().toInt();
    ThePic = CalculateMadel(   iter , info, ThePic  ,  ui->ToJulia->isChecked() );   // calculate  -  iterations, coordinates,  char array pointer,   Weather to calculate Julia or Mandel

    int pos = 4* (rheight/2*rwidth + rwidth/2 );                                            // FOR color smoothness         DELETE later
    printf("                B: %d  , G: %d  , R: %d \n" , ThePic[ pos  ] , ThePic[ pos+1], ThePic[pos+2] );

    QPixmap p = QPixmap::fromImage(img);
    ui->Image_name->setPixmap(p);
    ui->Image_name->show();

    if(ui->ToSave->isChecked() ){
  //      time_t T = time(NULL);       struct tm * currentT = localtime ( &T );

 //       std::cout <<   std::put_time(&currentT,  "%F %R %S")  << std::endl ;

  //     QString name = QString("Frac %1 .png").arg(    std::put_time(&currentT,  "%F %R %S")   );
     //   std::cout <<  name.toStdString()  << std::endl;

//        p.save(  name   ,0 , 50    );
        //printf("to save!");
        p.save( "Frac.jpg" );  //, 0, 50);

    }
}




void MainWindow::TheZoom( QPoint &mouse_pos , bool ZoomIn){
//void MainWindow::TheZoom(QWheelEvent * wheel){

    double Sc0 = ui->Scale->text().toDouble();      // Take current scale and multiplier
    if(Sc0==0){                                    // prevent errors that are only fixed after a restart
        ui->Scale->setText("1");
        Sc0=1;
    }
    int n0 =ui->Scale_multiplier->text().toInt();


    double Sc = 0;
    if( ZoomIn ){
       Sc = Sc0/  ui->Zoom_speed->text().toDouble();   // zoom in
    }else{
        Sc = Sc0 * ui->Zoom_speed->text().toDouble();   // zoom out
    }

    int n = log10(Sc);                        // Find out new order of magnitude   after affecting with Zoom in/out
    Sc = Sc * pow(10 ,-n);                    // Scale number gets lowered/increased by that order of magnitude
    ui->Scale->setText(  QString("%1").arg( Sc  )   );   // Save normalised  scale number

    n =  n0  +  n;                           // calculate the new Scale multiplier       (order of magnitude increase/decrease)
   ui->Scale_multiplier->setText(   QString("%1").arg(n)     );    // save new   n




    double x01 = (double) mouse_pos.x()  / (ui->Image_name->width()  -1);  // get the position of mouse in pixels and normalise it by dividing from image height or width
    double y01 = (double) mouse_pos.y()  / (ui->Image_name->height() -1);  // left side is 0  the right size is 1    the middle is 0.5  (same to up = 0,   down = 1)



   double Sc0x = (Sc0 + MainWindow::ARx) * std::pow(10 , n0 );
   double Sc0y = (Sc0 + MainWindow::ARy) * std::pow(10 , n0 );


    double x_C0 = ui->CorX->text().toDouble()    +  (x01*2 - 1)*Sc0x;    // Coordinates of the new center (depends on where the mouse was)
    double y_C0 = ui->CorY->text().toDouble()    +  (y01*2 - 1)*Sc0y;    // prievious center + ( x_01 * 2 - 1) * scale


    ui->CorX->setText(   QString::number( x_C0 , 'f' , -n+6)     );       // save the new center
    ui->CorY->setText(   QString::number( y_C0 , 'f' , -n+6)     );

    Sc = Sc*pow(10,n);

    GenerateImage(x_C0 , y_C0, Sc);
                      //std::cout << "C after   " << x_C << "          " <<y_C << std::endl;

}





void MainWindow::NewCenter(  QPoint & m_pos  ){

    double x = (double) m_pos.x() / (ui->Image_name->width()  -1);  // get the position of mouse in pixels and normalise it by dividing from image height or width
    double y = (double) m_pos.y() / (ui->Image_name->height() -1);  // left side is 0  the right size is 1    the middle is 0.5  (same to up = 0,   down = 1)

    int n =  ui->Scale_multiplier->text().toInt();
    double Sc = ui->Scale->text().toDouble();
    double ScX = ( Sc  + MainWindow::ARx) * std::pow(10 , n);   // Calculate current scale used
    double ScY = ( Sc  + MainWindow::ARy) * std::pow(10 , n);

    x = ScX*( x*2 - 1) + ui->CorX->text().toDouble();       // Coordinates of the new center
    y = ScY*( y*2 - 1) + ui->CorY->text().toDouble();

    ui->CorX->setText(   QString::number( x , 'f' , -n+6)     );       // save the new center
    ui->CorY->setText(   QString::number( y , 'f' , -n+6)     );

    Sc = Sc*pow(10,n);

    GenerateImage( x , y, Sc);

}





void MainWindow::MouseCoords( QPoint & m_pos ){

 //   if( ui->Show_coords->isChecked() == true){          // Check weather user wants this feature
        double x = (double) m_pos.x() / (ui->Image_name->width()  -1);  // get the position of mouse in pixels and normalise it by dividing from image height or width
        double y = (double) m_pos.y() / (ui->Image_name->height() -1);  // left side is 0  the right size is 1    the middle is 0.5  (same to up = 0,   down = 1)

        int n =  ui->Scale_multiplier->text().toInt();
        double ScX = ( ui->Scale->text().toDouble()  + MainWindow::ARx) * std::pow(10 , n);   // Calculate current scale used
        double ScY = ( ui->Scale->text().toDouble()  + MainWindow::ARy) * std::pow(10 , n);


        ui->Coords->setText(    QString("x: %1  \ny: %2 ").arg(             // print position in the label
             QString::number(    ScX*( x*2 - 1) + ui->CorX->text().toDouble()   , 'f'  , -n +7),
             QString::number(    ScY*( y*2 - 1) + ui->CorY->text().toDouble()   , 'f'  , -n +7)
                                    ) );      // Scale ( mouse * 2 - 1) + CurrentCoordinate
                                              // scale*2 * mouse - scale            where scale*2 is  scale Length
 //   }
}





void MainWindow::on_ToJulia_clicked()
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
            // hide unneeded variables
        ui->JulC_x->hide();
        ui->JulC_y->hide();
        GenerateImage( ui->CorX->text().toDouble(), ui->CorY->text().toDouble() ,
                      ui->Scale->text().toDouble() *pow(10 , ui->Scale_multiplier->text().toInt() )    );
    }
}
