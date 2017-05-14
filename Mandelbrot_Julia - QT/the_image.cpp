#include "the_image.h"

#include <iostream>
#include <fstream>

the_image::the_image(QWidget* parent) : QLabel(parent)
{

}


void the_image::mousePressEvent(QMouseEvent *M_event){      // If mouse is inside the image area


     if(   M_event->button() == Qt::RightButton   ){       // and there was a right click
        QPoint mouse_pos = M_event->pos();
        emit ActivateMouseCoords(   mouse_pos  );       // then show coordinates in a GUI element
    }
    else if(  M_event->button()  == Qt::LeftButton   ){  // and there was left click

    }

}

void the_image::wheelEvent( QWheelEvent* wheel ){
    emit ActivateZoom(wheel);
}




/*
void the_image::mouseMoveEvent(QMouseEvent *M_event)        // when mouse moves in the Image area - we sand the messege that it does
{                                                            // this is mostly because we cannot reach UI elements from this file
   QPoint mouse_pos = M_event->pos();
   emit ActivateMouseCoords(   mouse_pos  );
}
*/

