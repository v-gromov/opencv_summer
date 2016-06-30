#ifndef THREAD_ONE_H
#define THREAD_ONE_H
#include "mainwindow.h"
#include "find_face_and_eyes.h"
#include "crop_image.h"

#include <QObject>
#include <QThread>

#include <QFile>
#include <QFileDialog>

#include <opencv/cv.h>

class myThread: public QThread
{
    Q_OBJECT
protected:
    void run(){exec();}
};

class create_database: public QObject
{
    Q_OBJECT
public slots:

    void slot_createDB_get_image(IplImage*);
    void slot_createDB_start(QString);
signals:
    void sign_createDB_send_img(QImage);// отправка корректированой картинки
private:
    IplImage* lp_cvImage;
};

class online_translation: public QObject
{
    Q_OBJECT
public slots:
    void slot_online_translation();
signals:
    void sign_img_translation(QImage);
    void sign_img_translation1(IplImage*);
};

#endif // THREAD_ONE_H
