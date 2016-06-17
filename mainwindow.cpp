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
    bool bOk;
    QString name_people = QInputDialog::getText( 0, "Your name", "Enter your name:", QLineEdit::Normal, "user", &bOk);
    CvCapture* capture = cvCaptureFromCAM(0);//cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 );
    assert( capture );
    IplImage* frame=0;
    if (bOk){
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
            const char* myChar = tmp.toStdString().c_str();
            cvSaveImage(myChar, frame);
            face position_face = center_faces(tmp);
            //qDebug()<<position_face.number_eyes();
            if(position_face.number_eyes()==2)
            {
                numb_photo++;
            }

        }
        // освобождаем ресурсы
        cvReleaseCapture( &capture );
        cvDestroyWindow("capture");
    }
}
