#include "myThread.h"
#include <QLabel>
#include <QPixmap>

void WorkerForCamThread::slot_set_flag(bool flagval)
{
   flag = flagval;
}
bool WorkerForCamThread::getflag()
{
    return flag;
}

void create_database::slot_createDB_get_crop_image(QImage res)
{
    qDebug()<<"SAVE CROP GET";
    save_crop_image = res;
}


void create_database::slot_createDB_start(QString name_people)
{
    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            if((save_value)&&(!save_crop_image.isNull()))
            {
                Mat fordb = img_for_database(qimage2mat(save_crop_image));
                cvtColor(fordb, fordb,  CV_RGB2BGR);//Выставляем нужный колор
                save_crop_image = Mat2QImage(fordb);
                QPixmap pixmap;
                pixmap = pixmap.fromImage(save_crop_image.scaled(save_crop_image.width(),save_crop_image.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QString tmp2;
                tmp2.setNum(numb_photo);
                QString path_to_file = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg";
                //сохраним файл
                QFile file(path_to_file);
                file.open(QIODevice::WriteOnly);
                pixmap.save(&file, "jpg",100);
                file.close();
                //добавим сведения с CSV файл
                QFile fileCSV("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/database.csv");
                fileCSV.open(QIODevice::Append | QIODevice::Text);
                QTextStream writeStream(&fileCSV); // Создаем объект класса QTextStream
                writeStream <<path_to_file + "\n"; // Посылаем строку в поток для записи
                fileCSV.close(); // Закрываем файл

                save_value = false;
                numb_photo++;
            }
            else if(save_value)
            {
                qDebug()<<save_crop_image.isNull();
                save_value = false;
                qDebug()<<"IM HERE";
            }
        }
    }
}

void find_face_thread::find_face()
{
    QImage* res = new QImage;
    QVector <face> position_face = center_faces(SaveImage, res, scale);
    QImage cropImg = SaveImage;

    QImage img = *res;
    emit sign_find_face_thread_send_img(img);
    for(int i = 0; i < position_face.size(); i++)
    {

        if((position_face[i].number_eyes()==2)&&(position_face[i].get_radius_eyes()[0]>=20)&&(position_face[i].get_radius_eyes()[1]>=20))
        {
            SendImage_for_crop = CropFace(cropImg, position_face[i].get_coord_eyes().at(0).x,position_face[i].get_coord_eyes().at(0).y, position_face[i].get_coord_eyes().at(1).x, position_face[i].get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);
            sign_find_face_thread_send_crop_img(SendImage_for_crop);
        }
    }
}

find_face_thread::find_face_thread()
{
    scale = 0.71;
}

void find_face_thread::slot_set_scale(int val)
{
    if(val!=0)
        scale = val/100.;
    else
        scale = 0.1;
}

void find_face_thread::set_image(QImage save)
{
    SaveImage = save;
    find_face();
}

void recognition_face::slot_recogn_face_detect(QImage imgForDetect)
{
    Mat i = qimage2mat(imgForDetect);
    int numb = detect_object.get_numb_people(i);
    emit sign_getNumberPeople(numb);
}

create_database::create_database()
{
    save_value = false;
}


void create_database::slot_save_image()
{
    save_value = true;
}

cameraThread::cameraThread()
{
    capture = cvCaptureFromCAM(0);
    assert(capture);
}

cameraThread::~cameraThread()
{
    cvReleaseCapture( &capture );
    cvDestroyWindow("capture");
}

void cameraThread::functionThread()
{
    if(worker.getflag()){
        IplImage* frame = cvQueryFrame(capture);
        emit sign_img_translation(convert_lpl_qimg(frame));
    }
}

void cameraThread::run()
{
    while(true)
        functionThread();
    exec();
}
