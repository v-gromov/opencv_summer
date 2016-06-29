#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_create_database_clicked()
{
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->clear();
    ui->Button_ok->setEnabled(true);
}

void MainWindow::on_Button_ok_clicked()
{
    ui->lineEdit->setEnabled(false);
    ui->Button_ok->setEnabled(false);
    QString name_people = ui->lineEdit->text();
    qDebug()<<"part one";
    emit signCreateDatabase(name_people);
    qDebug()<<"part two";
}

void MainWindow::slotSetLabelImg(QImage imdisplay)
{
    ui->image_label->setPixmap(QPixmap::fromImage(imdisplay));
}

void workGUI::createDatabase(QString name_people)
{
    CvCapture* capture = cvCaptureFromCAM(0);
    assert(capture);
    IplImage* frame=0;
    if (!name_people.isEmpty())
    {
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            // получаем кадр
            frame = cvQueryFrame(capture);

            QString tmp2;
            tmp2.setNum(numb_photo);
            QString tmp = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image"+tmp2+".jpg";

            std::string myString = tmp.toStdString();
            const char* myChar = myString.c_str();
            cvSaveImage(myChar, frame);

            face position_face = center_faces(frame);
            //
            QImage img = convert_lpl_qimg(frame);
            qDebug()<<"bbb";
            //QImage crop = CropFace(img, position_face.get_coord_eyes().at(0).x,position_face.get_coord_eyes().at(0).y, position_face.get_coord_eyes().at(1).x, position_face.get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);
            qDebug()<<"aaa";
            //QPixmap pixmap;
            //pixmap = pixmap.fromImage(crop.scaled(crop.width(),crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
            sendImage(img);//отправляем сигнал
            //
            /*
            qDebug()<<position_face.number_eyes();
            if(position_face.number_eyes()==2)
            {
                QImage img = convert_lpl_qimg(frame);
                QImage crop = CropFace(img, position_face.get_coord_eyes().at(0).x,position_face.get_coord_eyes().at(0).y, position_face.get_coord_eyes().at(1).x, position_face.get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);

                QPixmap pixmap;
                pixmap = pixmap.fromImage(crop.scaled(crop.width(),crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                sendImage(pixmap);//отправляем сигнал
                QFile file("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg");
                file.open(QIODevice::WriteOnly);
                pixmap.save(&file, "jpg",100);
                file.close();
                numb_photo++;
            }*/
            numb_photo++;


        }
        // освобождаем ресурсы
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
        qDebug()<<"Database was created!";
    }
}

/*void workGUI::run()
{
    exec();
}*/

QImage workGUI::convert_lpl_qimg(IplImage* frame)
{
    Mat img = cvarrToMat(frame);
    cv::cvtColor(img,img,CV_BGR2RGB); //Qt reads in RGB whereas CV in BGR
    QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    return imdisplay;
}
