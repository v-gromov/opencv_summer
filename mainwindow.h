#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QDebug>

#include "myThread.h"

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
    void on_Button_ok_clicked();
    void slotSetLabelImg(QImage);
    void slotSetLabelOnllineImg(QImage);
signals:
    void signCreateDatabase(QString);
    void signOnlineTransl();
private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
