#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    double ARx, ARy = 0;    // for aspect ratio,  scale is square only
    double MxC, MyC, MSc, MSc_mul = 0;  // info to save for switching between Julia and mandel   -  Coordinates of center and scale info

public slots:
    void MouseCoords( QPoint& m_pos );     // A signal from the_image calls this function (since it is connected to that signal)
    void TheZoom( QPoint &mouse_pos , bool ZoomIn );     // it passes information about mouse position    and to zoom or not to zoom
    void NewCenter(  QPoint & m_pos  );
    void GenerateImage(double x_C , double y_C, double Sc);


private slots:
    void on_ToJulia_clicked();

private:
    Ui::MainWindow *ui;


};



#endif // MAINWINDOW_H
