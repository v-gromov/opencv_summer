#include "myThread.h"

void create_database::slot_createDB_start(QString name_people)
{
    /*while(1)
    {

    }*/
    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            QImage* res = new QImage;
            QVector <face> position_face = center_faces(SendImage, res, scale);
            QImage i = *res;
            emit sign_createDB_send_img(i);
            /*if(position_face.number_eyes()==2)
            {


                QImage crop = CropFace(img, position_face.get_coord_eyes().at(0).x,position_face.get_coord_eyes().at(0).y, position_face.get_coord_eyes().at(1).x, position_face.get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);

                QPixmap pixmap;
                pixmap = pixmap.fromImage(crop.scaled(crop.width(),crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));



                /*
                QFile file("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg");
                file.open(QIODevice::WriteOnly);
                pixmap.save(&file, "jpg",100);
                file.close();
                */
                //потом сохранить фото
                /*QString tmp2;
                tmp2.setNum(numb_photo);
                QString tmp = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image"+tmp2+".jpg";
                std::string myString = tmp.toStdString();
                const char* myChar = myString.c_str();
                cvSaveImage(myChar, frame);*/

                //numb_photo++;
            }
        //}
        // освобождаем ресурсы
        //cvReleaseCapture( &capture );
        //cvDestroyWindow("capture");
       // qDebug()<<"Database was created!";
    }
}

create_database::create_database()
{
    scale = 0.35;
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
    scale = val/100.;
    qDebug()<<scale;
}

void create_database::slot_createDB_get_image(QImage img)
{
    SendImage = img;
}
