#include "myThread.h"

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
                qDebug()<<"sssddsad";
                QPixmap pixmap;
                pixmap = pixmap.fromImage(SendImage_for_crop.scaled(SendImage_for_crop.width(),SendImage_for_crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QString tmp2;
                tmp2.setNum(numb_photo);
                QFile file("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg");
                file.open(QIODevice::WriteOnly);
                pixmap.save(&file, "jpg",100);
                file.close();
                save_value = false;
                numb_photo++;
            }

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
                    //QPixmap pixmap;
                    //pixmap = pixmap.fromImage(crop.scaled(crop.width(),crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                }

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
    qDebug()<<scale;
}

void create_database::slot_createDB_get_image(QImage img)
{
    SendImage = img;
}

void create_database::slot_save_image()
{
    save_value = true;
}
