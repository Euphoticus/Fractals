#include "the_image.h"

#include <iostream>
#include <fstream>

the_image::the_image(QWidget* parent) : QLabel(parent)
{

}


void the_image::mousePressEvent(QMouseEvent *M_event){      // If mouse is inside the image area

    QPoint mouse_pos = M_event->pos();

     if(   M_event->button() == Qt::RightButton   ){       // and there was a right click
        emit ActivateMouseCoords(   mouse_pos  );       // then show coordinates in a GUI element
    }
    else if(  M_event->button()  == Qt::LeftButton   ){  // and there was left click
        emit ActivateNewCenter(    mouse_pos  );     // mouse location becomes the center of the image
    }

}

void the_image::wheelEvent( QWheelEvent* wheel ){
    emit ActivateZoom(wheel);
}


