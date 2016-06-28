#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/highgui.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <QString>
#include "find_face_and_eyes.h"
#include "crop_image.h"
#include <QDebug>

#include <QApplication>
#include <math.h>
#include <QDebug>
#include <QLabel>
#include <QThread>
#include <QDesktopWidget>

QImage convert_lpl_qimg(IplImage* frame);
namespace Ui {
class MainWindow;
}

class workGUI : public QThread
{
    Q_OBJECT
    public:
    workGUI();
    ~workGUI();
    void run();
    void stop_sycle();
    void setImage(QPixmap img);
    QPixmap img;
private:
    int value_if_cycle;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void open_image_in_lable(IplImage* frame);
    ~MainWindow();
    workGUI thread;
private slots:
    void on_Button_create_database_clicked();

    void on_Button_ok_clicked();

private:
    Ui::MainWindow *ui;

};




#endif // MAINWINDOW_H
