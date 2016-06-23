#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->lineEdit->setText("user");
    bool bOk;
    QString name_people = ui->lineEdit->text();//QInputDialog::getText( 0, "Your name", "Enter your name:", QLineEdit::Normal, "user", &bOk);

    //QString name_people = QInputDialog::getText( 0, "Your name", "Enter your name:", QLineEdit::Normal, "user", &bOk);
   //qDebug()<<name_people;

    //QString name_people = "user";
    CvCapture* capture = cvCaptureFromCAM(0);//cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 );
    assert( capture );
    IplImage* frame=0;
    if (!name_people.isEmpty()){
        QDir().mkdir(name_people);
        int numb_photo = 0;
        while(numb_photo<=10)
        {
            // получаем кадр
            frame = cvQueryFrame( capture );
            // показываем
            //cvShowImage("capture", frame);
            QString tmp2;
            tmp2.setNum(numb_photo);
            QString tmp = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image"+tmp2+".jpg";

            std::string myString = tmp.toStdString();
            const char* myChar = myString.c_str();
            cvSaveImage(myChar, frame);
            face position_face = center_faces(tmp);
            qDebug()<<position_face.number_eyes();
            if(position_face.number_eyes()==2)
            {
                QImage img(tmp);
                QImage crop = CropFace(img, position_face.get_coord_eyes().at(0).x,position_face.get_coord_eyes().at(0).y, position_face.get_coord_eyes().at(1).x, position_face.get_coord_eyes().at(1).y, 0.3, 0.3, 200, 200);

                QPixmap pixmap;
                pixmap = pixmap.fromImage(crop.scaled(crop.width(),crop.height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
                QFile file("/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/"+name_people+"/image_crop"+tmp2+".jpg");
                file.open(QIODevice::WriteOnly);
                pixmap.save(&file, "jpg",100);
                file.close();
                numb_photo++;
            }

        }
        // освобождаем ресурсы
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
        destroyWindow("result");
        qDebug()<<"Database was created!";
    }
}
