#include "myThread.h"
#include <QLabel>
#include <QPixmap>

void WorkerForCamThread::slot_set_flag(bool flagval)
{
    flag = flagval;
}

void WorkerForCamThread::end_thread()
{
    end_flag = false;
}

bool WorkerForCamThread::getflag()
{
    return flag;
}

bool WorkerForCamThread::getend()
{
    return end_flag;
}

WorkerForCamThread::WorkerForCamThread()
{
    flag = true;
    end_flag = true;
}

void create_database::slot_createDB_get_crop_image(QImage res)
{
    save_crop_image = res;
}

void create_database::slot_createDB_start(QString name_people)
{
    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        //---------------------
        QFile fileWithName("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/name_users.csv");
        if (!fileWithName.open(QIODevice::ReadOnly))
        {
            qDebug() << "Ошибка открытия для чтения";
        }
        else
        {
            int end_numb = 0;
            while(!fileWithName.atEnd())
            {
                QString str;
                str = fileWithName.readLine();
                QStringList list1 = str.split(' ');
                QString value = list1.at(1);
                if(value.toInt()>end_numb)
                    end_numb = value.toInt();
            }
            end_numb++;
            fileWithName.close();
            fileWithName.setFileName("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/name_users.csv");
            if (!fileWithName.open(QIODevice::Append | QIODevice::Text))
            {
                qDebug() << "Ошибка открытия для чтения";
            }
            else
            {
                QString tmp_str;
                tmp_str.setNum(end_numb);
                QString numberAndName = name_people + " "+tmp_str;
                QTextStream StreamFile(&fileWithName); // Создаем объект класса QTextStream
                StreamFile <<numberAndName + "\n"; // Посылаем строку в поток для записи
                fileWithName.close();

                //---------------------
                int numb_photo = 0;
                while(numb_photo<10)
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
                        writeStream <<path_to_file + ";"+tmp_str+"\n"; // Посылаем строку в поток для записи
                        fileCSV.close(); // Закрываем файл
                        save_value = false;
                        numb_photo++;
                        emit slot_numb_image(numb_photo);
                    }
                }
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
    delete res;
    emit sign_find_face_thread_send_img(img);
    for(int i = 0; i < position_face.size(); i++)
    {
        if(position_face[i].number_eyes()==2)
        {
            if(position_face[i].get_coord_eyes().at(1).x-position_face[i].get_coord_eyes().at(0).x>30)
            {
                SendImage_for_crop = CropFace(cropImg, position_face[i].get_coord_eyes().at(0).x,position_face[i].get_coord_eyes().at(0).y, position_face[i].get_coord_eyes().at(1).x, position_face[i].get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);
                sign_find_face_thread_send_crop_img(SendImage_for_crop);
                return;
            }
        }
    }
    return;
}

find_face_thread::find_face_thread()
{
    scale = 1;
}

void find_face_thread::slot_set_scale(int val)
{
    //пока повременить. Очень низкий sclare < 1 дает полную загрузку проца
    /*if(val!=0)
        scale = val/100.;
    else
        scale = 0.1;*/
    scale = 1;
}

void find_face_thread::set_image(QImage save)
{
    SaveImage = save;
    find_face();
}

void recognition_face::slot_recogn_face_detect(QImage imgForDetect)
{
    if(flag_workthread){
        Mat i = qimage2mat(imgForDetect);
        int numb = detect_object.get_numb_people(i);
        emit sign_getNamePeople(parserName(numb));
    }
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
    videoInput VI;
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
    if(worker.getflag())
    {
        IplImage* frame = cvQueryFrame(capture);
        emit sign_img_translation(convert_lpl_qimg(frame));
    }
}

void cameraThread::run()
{
    while(worker.getend())
    {
        functionThread();
    }
}

recognition_face::recognition_face()
{
    flag_workthread = false;
}
void recognition_face::setworkThread(bool val)
{
    flag_workthread = val;
}

void recognition_face::trainModel()
{
    detect_object.train_model();
}

QString recognition_face::parserName(int value)
{
    QFile fileWithName("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/name_users.csv");
    if (!fileWithName.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка открытия для чтения";
        return "Error";
    }
    else
    {
        while(!fileWithName.atEnd())
        {
            QString str = fileWithName.readLine();
            QStringList list1 = str.split(' ');
            QString val_str = list1.at(1);
            int str_numb = val_str.toInt();
            if(value == str_numb)
                return list1.at(0);
        }
        fileWithName.close();
    }
    return "NoName";
}
