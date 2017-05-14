#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "the_image.h"

#include "MadelBrotJulia_cpu.h"

#include <iostream>
#include <fstream>

// #include <string>  // to_string



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->Image_name ,    SIGNAL( ActivateMouseCoords(  QPoint&   ) ) ,
              this,             SLOT(  MouseCoords( QPoint&  )    )                   );       // added

    connect(ui->Image_name ,    SIGNAL(  ActivateZoom( QWheelEvent* ) ) ,
              this,             SLOT(  TheZoom(QWheelEvent* ) )                  );

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::GenerateImage(double x_C , double y_C, double Sc){      // mouse coordinates in pixels

    int rwidth = ui->RWidth->text().toInt();
    int rheight = ui->RHeight->text().toInt();
    double info1[] = { x_C - Sc,   x_C + Sc,    y_C - Sc , y_C + Sc,   (double)rwidth     , (double)rheight   };
                    // x1,               x2,                 y1,            y2,                width ,      height

    double* info = info1;
    info = CorrectRatio(  info  );  // correct the aspect ratio


    MainWindow::ARx = info[1] - (x_C + Sc);     // The amount that doesen't fit into a scale box gets saved there
    MainWindow::ARy = info[3] - (y_C + Sc);


    ui->Image_name->resize( rwidth , rheight );

    QImage img = QImage(  rwidth  ,  rheight  ,   QImage::Format_RGB32);        // initialise image, tell what format and height, width
    unsigned char* ThePic = img.bits();         // assign char array that holds pixels


    ThePic = CalculateMadel(500 , info, ThePic );   // calculate


    QPixmap p = QPixmap::fromImage(img);
    ui->Image_name->setPixmap(p);
    ui->Image_name->show();
}





void MainWindow::TheZoom(QWheelEvent * wheel){

    double Sc0 = ui->Scale->text().toDouble();          // Take current scale and multiplier
    int n0 =ui->Scale_multiplier->text().toInt();


    double Sc = 0;
    if( wheel->angleDelta().y() > 0){      // Determine weather it is scrolled outwards or toward the user
       Sc = Sc0/  ui->Zoom_speed->text().toDouble();   // zoom in
    }else{
       Sc = Sc0 * ui->Zoom_speed->text().toDouble();   // zoom out
    }

    int n = log10(Sc);        // Find out the order of magnitude   after affecting with Zoom in/out
    Sc = Sc * pow(10 ,-n);    // Scale number gets lowered/increased by that order of magnitude
    ui->Scale->setText(  QString("%1").arg( Sc  )   );   // Save normalised  scale number

    n =  n0  +  n;    // calculate the new Scale multiplier                  (order of magnitude increase/decrease)
   ui->Scale_multiplier->setText(   QString("%1").arg(n)     );    // save new   n






    double x01 = (double) wheel->x()  / (ui->Image_name->width()  -1);  // get the position of mouse in pixels and normalise it by dividing from image height or width
    double y01 = (double) wheel->y()  / (ui->Image_name->height() -1);  // left side is 0  the right size is 1    the middle is 0.5  (same to up = 0,   down = 1)


//std::cout << "01  " << x01 << "          " << y01 << std::endl; // works

    Sc0 = Sc0  * std::pow(10 , n0 );

//std::cout << Sc << std::endl;  // works

    double x_C0 = ui->CorX->text().toDouble();    // Coordinates of the new center (depends on where the mouse was)
    double y_C0 = ui->CorY->text().toDouble();    // prievious center + ( x_01 * 2 - 1) * scale

//std::cout << "C before   " << x_C << "         " <<y_C << std::endl;

    double x_C = x_C0  + (x01*2 - 1)*Sc0   +   MainWindow::ARx;
    double y_C = y_C0  + (y01*2 - 1)*Sc0   +   MainWindow::ARy;



    ui->CorX->setText(   QString::number( x_C , 'f' , -n+6)     );       // sace the new center
    ui->CorY->setText(   QString::number( y_C , 'f' , -n+6)     );

//std::cout << "+mul " << (x01*2 - 1)*Sc << "           " << (y01*(-2) + 1)*Sc << std::endl;

//std::cout << "C after   " << x_C << "          " <<y_C << std::endl;

    Sc = Sc*pow(10,n);

    GenerateImage(x_C , y_C, Sc);
}










void MainWindow::MouseCoords( QPoint & m_pos ){

 //   if( ui->Show_coords->isChecked() == true){          // Check weather user wants this feature
        double x = (double) m_pos.x() / (ui->Image_name->width()  -1);  // get the position of mouse in pixels and normalise it by dividing from image height or width
        double y = (double) m_pos.y() / (ui->Image_name->height() -1);  // left side is 0  the right size is 1    the middle is 0.5  (same to up = 0,   down = 1)

        double Sc = ui->Scale->text().toDouble()  * std::pow(10 , ui->Scale_multiplier->text().toInt() );   // Calculate current scale used



        ui->Coords->setText(    QString("x: %1  \ny: %2 ").arg(             // print position in the label
             QString::number(    Sc*( x*2 - 1) + ui->CorX->text().toDouble()   ),
             QString::number(    Sc*( y*2 - 1) + ui->CorY->text().toDouble()   )
                                    ) );      // Scale ( mouse * 2 - 1) + CurrentCoordinate
                                              // scale*2 * mouse - scale            where scale*2 is  scale Length
 //   }
}
