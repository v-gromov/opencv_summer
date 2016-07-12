#ifndef THREAD_ONE_H
#define THREAD_ONE_H

#include "find_face_and_eyes.h"
#include "crop_image.h"
#include "detecting_face.h"

#include <QObject>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <atomic>

Mat img_for_database(Mat);

class myThread: public QThread
{
    Q_OBJECT
protected:
    void run(){exec();}
};

class WorkerForCamThread: public QObject
{
    Q_OBJECT
public:
    bool getflag();
public slots:
    void slot_set_flag(bool);
private:
    bool flag;
};

class cameraThread: public QThread
{
    Q_OBJECT
public:
    cameraThread();
    ~cameraThread();
    WorkerForCamThread worker;
protected:
    void run();
    void functionThread();
    CvCapture* capture;
signals:
    void sign_img_translation(QImage);

};


class create_database: public QObject
{
    Q_OBJECT
public:
    create_database();
    void find_face();
public slots:
    void slot_createDB_get_crop_image(QImage);
    void slot_createDB_start(QString);
    void slot_save_image();
private:
    bool save_value;
    QImage save_crop_image;
signals:
    void slot_numb_image(int);
};


class find_face_thread: public QObject
{
    Q_OBJECT
public:
    find_face_thread();
    void find_face();
private:
    QImage SendImage;
    QImage SendImage_for_crop;
    QImage SaveImage;
    std::atomic<float> scale;
signals:
    void sign_find_face_thread_send_img(QImage);// отправка картинки с кругами
    void sign_find_face_thread_send_crop_img(QImage);//отправка кропленой картинки
public slots:
     void slot_set_scale(int);
     void set_image(QImage);
};

class recognition_face: public QObject
{
    Q_OBJECT
signals:
    void sign_getNumberPeople(int);
public slots:
    void setworkThread(bool);
    void trainModel();
public:
    recognition_face();
public slots:
    void slot_recogn_face_detect(QImage);
private:
    face_model detect_object;
    bool flag_workthread;
};


#endif // THREAD_ONE_H
