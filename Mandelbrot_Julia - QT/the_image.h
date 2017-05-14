#ifndef THE_IMAGE_H
#define THE_IMAGE_H



#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>


class the_image : public QLabel
{
    Q_OBJECT

public:
    the_image(QWidget* parent = 0);

protected:
//    void mouseMoveEvent(QMouseEvent* M_event);
    void mousePressEvent(QMouseEvent* M_event);     // We watch for the mouse events - if one happened this calls "mouseMoveEvent" function
    void wheelEvent(  QWheelEvent* wheel );
 //   void mouseReleaseEvent(QMouseEvent* M_event);

signals:
    void ActivateMouseCoords(  QPoint & m_pos );      // The mouse event that called "mouseMoveEvent" function, activates this signal so it is sent to main window
    void ActivateZoom( QWheelEvent* wheel );
    void ActivateNewCenter(  QPoint & m_pos  );

};

#endif // THE_IMAGE_H



