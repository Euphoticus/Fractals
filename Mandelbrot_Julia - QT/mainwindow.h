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


public slots:
    void MouseCoords( QPoint& m_pos );     // A signal from the_image calls this function (since it is connected to that signal)
    void TheZoom( QWheelEvent* wheel );     // it passes information about mouse position e.g
    void GenerateImage(double x_C , double y_C, double Sc);

private:
    Ui::MainWindow *ui;


};



#endif // MAINWINDOW_H
