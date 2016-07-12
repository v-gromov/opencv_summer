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
    flag = false;
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
    ui->Button_save_face->setEnabled(false);
    flag = !flag;
    emit setPlayOrPause(1);
    emit slotWorkRecogn(0);
    ui->Button_Find_face->setEnabled(false);
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

void MainWindow::slotPrintNamePeople(QString name)
{
    ui->Infolabel->setText(name);
}

void MainWindow::on_Button_Find_face_clicked()
{
    //Поставим значения баттонов на значения флага
    ui->Button_create_database->setEnabled(flag);
    ui->Button_ok->setEnabled(flag);
    ui->Button_save_face->setEnabled(flag);
    //и введем противоположенное значение флага
    flag = !flag;
    if(flag)
        emit signtrainModel();
    emit setPlayOrPause(flag);
    emit slotWorkRecogn(flag);
}

void MainWindow::slotGetNumbPhoto(int value)
{
    ui->lcdNumber_2->display(value);
    if(value>=10)
    {
        //и введем противоположенное значение флага
        flag = !flag;
        ui->Button_create_database->setEnabled(1);
        ui->Button_ok->setEnabled(0);
        ui->Button_save_face->setEnabled(0);
        ui->Button_Find_face->setEnabled(1);
        ui->lcdNumber_2->display(0);
    }
}
