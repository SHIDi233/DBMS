#include "login.h"
#include "ui_login.h"
#include<QString>
#include <QMovie>


QString button_start = "QPushButton{color:white;background-color:rgb(14 , 150 , 254);border-radius:5px;}";
QString button_press = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";
QString button_hover = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    QMovie* movie = new QMovie("://image/sh.gif");
    ui->label->setMovie(movie);
    movie->start();
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    //ui->pushButton->setStyleSheet(button_press);
}


void Login::on_pushButton_pressed()
{
    ui->pushButton->setStyleSheet(button_press);
}


void Login::on_pushButton_released()
{
    ui->pushButton->setStyleSheet(button_start);
}

