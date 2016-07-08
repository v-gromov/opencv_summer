#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalScrollBar->setValue(71);
    ui->Button_save_face->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_create_database_clicked()
{
    emit signOnlineTransl();
    ui->lineEdit->setEnabled(true);
    ui->lineEdit->clear();
    ui->Button_ok->setEnabled(true);
    ui->Button_save_face->setEnabled(false);
}

void MainWindow::on_Button_ok_clicked()
{
    ui->lineEdit->setEnabled(false);
    ui->Button_ok->setEnabled(false);
    ui->Button_save_face->setEnabled(true);
    QString name_people = ui->lineEdit->text();

    emit signCreateDatabase(name_people);
}

void MainWindow::slotSetLabelImg(QImage imdisplay)
{
    ui->image_label->setPixmap(QPixmap::fromImage(imdisplay));
}


void MainWindow::slotSetLableCropImg(QImage imdisplay)
{
    ui->image_crop_label->setPixmap(QPixmap::fromImage(imdisplay));
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    ui->lcdNumber->display(position*0.01);
    emit signSend_scale(position);
}

void MainWindow::on_Button_save_face_clicked()
{
    emit signSendSave();
}


//Оно просто транслирует видеопоток
void MainWindow::slotSetLabelOnllineImg(QImage imdisplay)
{
    ui->online_video_label->setPixmap(QPixmap::fromImage(imdisplay));
}
