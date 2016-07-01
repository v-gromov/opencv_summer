#ifndef THREAD_ONE_H
#define THREAD_ONE_H
#include "mainwindow.h"
#include "find_face_and_eyes.h"
#include "crop_image.h"
#include "highgui.h"

#include <QObject>
#include <QThread>

#include <QFile>
#include <QFileDialog>
#include <atomic>

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
public:
    create_database();
public slots:
    void slot_createDB_get_image(QImage);
    void slot_createDB_start(QString);
    void slot_set_scale(int);

signals:
    void sign_createDB_send_img(QImage);// отправка корректированой картинки
private:
    QImage SendImage;
    std::atomic<float> scale;
};

class online_translation: public QObject
{
    Q_OBJECT
public:
    online_translation();
    ~online_translation();
public slots:
    void slot_online_translation();
signals:
    void sign_img_translation(QImage);
private:
    CvCapture* capture;

};

#endif // THREAD_ONE_H
