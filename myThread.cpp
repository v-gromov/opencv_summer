#include "myThread.h"
#include <QLabel>
#include <QPixmap>
void create_database::slot_createDB_start(QString name_people)
{
    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            if((!SendImage.isNull())&&(save_value))
            {
                QPixmap pixmap;
                pixmap = pixmap.fromImage(SendImage_for_crop.scaled(SendImage_for_crop.width(),SendImage_for_crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QString tmp2;
                tmp2.setNum(numb_photo);
                QString path_to_file = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg"+";" + name_people;
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
            find_face();
        }
    }
}

void create_database::find_face()
{
    QImage* res = new QImage;
    QVector <face> position_face = center_faces(SendImage, res, scale);
    QImage cropImg = SendImage;

    QImage img = *res;
    emit sign_createDB_send_img(img);
    for(int i = 0; i < position_face.size(); i++)
    {

        if((position_face[i].number_eyes()==2)&&(position_face[i].get_radius_eyes()[0]>=20)&&(position_face[i].get_radius_eyes()[1]>=20))
        {
            SendImage_for_crop = CropFace(cropImg, position_face[i].get_coord_eyes().at(0).x,position_face[i].get_coord_eyes().at(0).y, position_face[i].get_coord_eyes().at(1).x, position_face[i].get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);
            sign_createDB_send_crop_img(SendImage_for_crop);
        }
    }
}

void recognition_face::slot_recogn_face_detect(QImage imgForDetect)
{
    Mat i = qimage2mat(imgForDetect);
    imshow( "Display window", i);
    int numb = detect_object.get_numb_people(i);
    qDebug()<<"Numb people: "<<numb;
}

create_database::create_database()
{
    scale = 0.35;
    save_value = false;
}

void online_translation::slot_online_translation()
{

    while(true){
        IplImage* frame = cvQueryFrame(capture);
        emit sign_img_translation(convert_lpl_qimg(frame));
    }
}

online_translation::online_translation()
{
    capture = cvCaptureFromCAM(0);
    assert(capture);
}

online_translation::~online_translation()
{
    cvReleaseCapture( &capture );
    cvDestroyWindow("capture");
}

void create_database::slot_set_scale(int val)
{
    if(val!=0)
        scale = val/100.;
    else
        scale = 0.1;
}

void create_database::slot_createDB_get_image(QImage img)
{
    SendImage = img;
}

void create_database::slot_save_image()
{
    save_value = true;
}
