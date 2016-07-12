
#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    myThread thread_one;//для create DB
    cameraThread thread_two;//для online трансляции
    myThread thread_three; //для распознавания лица
    myThread thread_four; //Для кропа картинок
    create_database obj_create_DB;
    recognition_face obj_recogn;
    find_face_thread obj_crop_face;

    obj_create_DB.moveToThread(&thread_one);
    obj_recogn.moveToThread(&thread_three);
    obj_crop_face.moveToThread(&thread_four);

    //создание бд
    QObject::connect(&w, SIGNAL(signCreateDatabase(QString)), &obj_create_DB, SLOT(slot_createDB_start(QString)));
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_img(QImage)), &w, SLOT(slotSetLabelImg(QImage)));
    //передача кропнутой картинки в создать базу данных
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &obj_create_DB, SLOT(slot_createDB_get_crop_image(QImage)), Qt::DirectConnection);

    //передача sclare с скрол бара в поток с видео
    QObject::connect(&w, SIGNAL(signSend_scale(int)), &obj_crop_face, SLOT(slot_set_scale(int)), Qt::DirectConnection);

    //Передача сигнала о сохранении картинки из MainWindow в поток для сохранения картинки
    QObject::connect(&w, SIGNAL(signSendSave()), &obj_create_DB, SLOT(slot_save_image()), Qt::DirectConnection);

    //поиск лиц в recogn_face
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &obj_recogn, SLOT(slot_recogn_face_detect(QImage)));
    //Получение номера обнаруженного человека
    QObject::connect(&obj_recogn, SIGNAL(sign_getNamePeople(QString)), &w, SLOT(slotPrintNamePeople(QString)));

    //Обрезаем картинку. Для этого из потока камеры передаем картинку в поток для кропа имаджи.
    QObject::connect(&thread_two, SIGNAL(sign_img_translation(QImage)), &obj_crop_face, SLOT(set_image(QImage)));
    //передача кропнутой картинки в лэйбл с кропом
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &w, SLOT(slotSetLableCropImg(QImage)));

    //Добавляет возможность ставить на паузу трансляцию видео.
    QObject::connect(&w, SIGNAL(setPlayOrPause(bool)), &thread_two.worker, SLOT(slot_set_flag(bool)), Qt::DirectConnection);
    //Добавляет возможность ставить на паузу трансляцию видео.
    QObject::connect(&w, SIGNAL(slotWorkRecogn(bool)), &obj_recogn, SLOT(setworkThread(bool)), Qt::DirectConnection);

    //получение отфотканой картинки в MainWindow
    QObject::connect(&obj_create_DB, SIGNAL(slot_numb_image(int)),&w, SLOT(slotGetNumbPhoto(int)));
    QObject::connect(&w, SIGNAL(signtrainModel()), &obj_recogn, SLOT(trainModel()));//Тренировка модели

    thread_two.start();
    thread_four.start();
    thread_one.start();
    thread_three.start();
    w.show();
    return a.exec();
}
