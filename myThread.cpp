#include "myThread.h"

void create_database::slot_createDB_start(QString name_people)
{
    /*CvCapture* capture = cvCaptureFromCAM(0);
    assert(capture);*/
    IplImage* frame=0;

    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            // получаем кадр
            //frame = cvQueryFrame(capture);
            frame = lp_cvImage;
            //emit sign_createDB_online_img(convert_lpl_qimg(frame));

            QVector <face> position_face = center_faces(frame);
            //qDebug()<<position_face.number_eyes();

            QImage img = convert_lpl_qimg(frame);
            emit sign_createDB_send_img(img);

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

            //    numb_photo++;
            //}
        }
        // освобождаем ресурсы
        //cvReleaseCapture( &capture );
        //cvDestroyWindow("capture");
        qDebug()<<"Database was created!";
    }
}

void online_translation::slot_online_translation()
{
    CvCapture* capture = cvCaptureFromCAM(0);
    assert(capture);
    while(true){
    IplImage* frame = cvQueryFrame(capture);
    emit sign_img_translation(convert_lpl_qimg(frame));
    emit sign_img_translation1(frame);
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow("capture");
}

void create_database::slot_createDB_get_image(IplImage* img)
{
    lp_cvImage = img;
}
