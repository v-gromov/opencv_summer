#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    myThread thread_one;
    myThread thread_two;
    create_database obj_create_DB;
    online_translation obj_online;

    obj_create_DB.moveToThread(&thread_one);
    obj_online.moveToThread(&thread_two);

    //создание бд
    QObject::connect(&w, SIGNAL(signCreateDatabase(QString)), &obj_create_DB, SLOT(slot_createDB_start(QString)));
    QObject::connect(&obj_create_DB, SIGNAL(sign_createDB_send_img(QImage)), &w, SLOT(slotSetLabelImg(QImage)));
    //отображение видеопотока
    QObject::connect(&w, SIGNAL(signOnlineTransl()), &obj_online, SLOT(slot_online_translation()));
   // QObject::connect(&obj_online, SIGNAL(sign_img_translation(QImage)), &w, SLOT(slotSetLabelOnllineImg(QImage)));
    //передача видеокадров в класс создания бд
    QObject::connect(&obj_online, SIGNAL(sign_img_translation(QImage)), &obj_create_DB, SLOT(slot_createDB_get_image(QImage)));
    //передача sclare с скрол бара
    QObject::connect(&w, SIGNAL(signSend_scale(int)), &obj_create_DB, SLOT(slot_set_scale(int)), Qt::DirectConnection);
    //передача кропнутой картинки в лэйбл с кропом
    QObject::connect(&obj_create_DB, SIGNAL(sign_createDB_send_crop_img(QImage)), &w, SLOT(slotSetLableCropImg(QImage)));
    //передача сигнала о сохранении файла
    QObject::connect(&w, SIGNAL(signSendSave()), &obj_create_DB, SLOT(slot_save_image()), Qt::DirectConnection);

    thread_two.start();
    thread_one.start();
    w.show();
    return a.exec();
}
