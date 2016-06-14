#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/highgui.h>

#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Button_create_database_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
