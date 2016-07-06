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
    void find_face();
public slots:
    void slot_createDB_get_image(QImage);
    void slot_createDB_start(QString);
    void slot_set_scale(int);
    void slot_save_image();

signals:
    void sign_createDB_send_img(QImage);// отправка картинки с кругами
    void sign_createDB_send_crop_img(QImage);//отправка кропленой картинки
private:
    QImage SendImage;
    QImage SendImage_for_crop;
    bool save_value;

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


class recognition_face: public QObject
{
    Q_OBJECT
public slots:
    void slot_recogn_face_detect(QImage);
private:
    face_model detect_object;
};

#endif // THREAD_ONE_H
