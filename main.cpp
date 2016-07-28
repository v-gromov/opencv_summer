
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    cameraThread thread_online_img;//Поток для трансляции изображения с WEB камеры. Он без QObject
    myThread thread_crop_img; //Поток для обрезки картинок, которые получены от thread_online_img
    myThread thread_recogn_face; //Поток для распознавания лица, находящегося в обрезанной картинке
    myThread thread_createDB;//Поток для создания базы данных из обрезанных картинок (не распознанных)

    //Поскольку определенные слоты в потоке будут выполняться в потоке родителя, т.е. в MainWindow. Тут создаются объекты, слоты которых будут выполняться в отдельных потоках
    find_face_thread obj_crop_face(&thread_online_img); //Объект для обрезки картинок.
    recognition_face obj_recogn;    //Объект для распознавания лиц
    create_database obj_create_DB;  //Объект для создания БД
    //Поместим найденные объекты в потоки. Их слоты уже будут выполняться в потоках, куда их поместим
    obj_create_DB.moveToThread(&thread_createDB);
    obj_recogn.moveToThread(&thread_recogn_face);
    obj_crop_face.moveToThread(&thread_crop_img);

    //Объявление сигналов для взаимодействия потоков
    QObject::connect(&thread_online_img, SIGNAL(sign_img_translation(QImage)), &obj_crop_face, SLOT(set_image(QImage))); //передача картинки с камеры в поток для обрезки картинок
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &w, SLOT(slotSetLableCropImg(QImage))); //передача обрезанной картинки в MainWindow
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &obj_recogn, SLOT(slot_recogn_face_detect(QImage))); //передача обрезанной картинки в функцию для распознавания лица
    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_crop_img(QImage)), &obj_create_DB, SLOT(slot_createDB_get_crop_image(QImage)), Qt::DirectConnection); //передача обрезанной картинки в функцию для создани БД

    QObject::connect(&obj_crop_face, SIGNAL(sign_find_face_thread_send_img(QImage)), &w, SLOT(slotSetLabelImg(QImage)));//Этот сигнал передает картинку с кругами в MainWindow
   // QObject::connect(&w, SIGNAL(signSend_scale(int)), &obj_crop_face, SLOT(slot_set_scale(int)), Qt::DirectConnection); //передача sclare с скрол бара в поток с трансляции картинок

    QObject::connect(&w, SIGNAL(signCreateDatabase(QString)), &obj_create_DB, SLOT(slot_createDB_start(QString)));  //По этому сигналу будет запущена функция в потоке для создания БД
    QObject::connect(&w, SIGNAL(signSendSave()), &obj_create_DB, SLOT(slot_save_image()), Qt::DirectConnection);//Передача сигнала, чтобы была сохранена картинка для базы данных
    QObject::connect(&obj_create_DB, SIGNAL(slot_numb_image(int)),&w, SLOT(slotGetNumbPhoto(int)));//получение номера картинки, которая добавляется в БД (картинок должно быть 10)

    QObject::connect(&w, SIGNAL(signtrainModel(int, int)), &obj_recogn, SLOT(trainModel(int,int)));//Сигнал для "тренировки" модели распознавания лиц. Поскольку могут добавляться новые лица
    QObject::connect(&obj_recogn, SIGNAL(sign_getNamePeople(QString)), &w, SLOT(slotPrintNamePeople(QString))); //возвращение имени обнаруженного лица с потока обнаружения лиц

    QObject::connect(&w, SIGNAL(setPlayOrPause(bool)), &thread_online_img.worker, SLOT(slot_set_flag(bool)), Qt::DirectConnection); //позволяет ставить на "паузу" передачу Online картинок в поток для обрезки картинки
    QObject::connect(&w, SIGNAL(slotWorkRecogn(bool)), &obj_recogn, SLOT(setworkThread(bool)), Qt::DirectConnection); //позволяет ставить на "паузу" функцию с распознаванием лиц

    QObject::connect(&w, SIGNAL(end_threads()), &thread_online_img.worker, SLOT(end_thread()), Qt::DirectConnection);//Отправка сигнала о завершении работы Mainwindow для потока с трансляции с камеры


    thread_online_img.start(); //Запуск потока с трансляцией online картинки
    thread_crop_img.start();    //запуск потока для создания обрезанных картинок с лицом
    thread_createDB.start();    //запуск потока для создания базы данных с обрезанных картинок
    thread_recogn_face.start(); //запускпотока для распознавания лиц с обрезанных картинок

    w.setWindowTitle("Program for detected face");
    w.show();   //просмотр MainWindow
    a.exec();   //Работа MainWindow

    /*---Далее идет код, когда закрыт mainWindow ---*/
    thread_crop_img.signalsBlocked();
    w.send_sign(); //отправка сигнала для завершения потокока с камерой
    //thread_online_img.quit();
    thread_crop_img.quit();
    thread_online_img.wait(); //ожидание завершения потока, который завершается по этому сигналу
    thread_crop_img.quit();
    thread_crop_img.wait();
    thread_recogn_face.quit();
    thread_recogn_face.wait();
    thread_createDB.quit();
    thread_createDB.wait();
    return 0;
}
