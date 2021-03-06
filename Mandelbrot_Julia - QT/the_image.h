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
    void mousePressEvent(QMouseEvent* M_event);     // We watch for the mouse events
    void wheelEvent(  QWheelEvent* wheel );
 //   void mouseReleaseEvent(QMouseEvent* M_event);

signals:
    void ActivateMouseCoords(  QPoint & m_pos );      // The mouse event that called "mousePressEvent" function, activates this signal so it is sent to main window
    void ActivateZoom( QPoint &mouse_pos , bool ZoomIn);
    void ActivateNewCenter(  QPoint & m_pos  );

};

#endif // THE_IMAGE_H



