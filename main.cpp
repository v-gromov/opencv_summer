#include "mainwindow.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    workGUI thread;
    thread.start();
    QObject::connect(&w, SIGNAL(signCreateDatabase(QString)), &thread, SLOT(createDatabase(QString)));
    QObject::connect(&thread, SIGNAL(SignImage(QImage)), &w, SLOT(slotSetLabelImg(QImage)));
    w.show();
    return a.exec();
}
