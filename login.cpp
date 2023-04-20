#include "login.h"
#include "ui_login.h"
#include<QString>
#include <QMovie>
#include<QMouseEvent>


QString button_start = "QPushButton{color:white;background-color:rgb(14 , 150 , 254);border-radius:5px;}";
QString button_press = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";
QString button_hover = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
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

void Login::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void Login::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton  //左键点击并且移动
            && e->pos().x()>=0      //范围在窗口的上面部分
            && e->pos().y()>=0
            && e->pos().x()<= geometry().width()
            && e->pos().y() <= geometry().height()/10)
    {
        move(e->pos()+pos()-clickPos);  //移动窗口
    }
}


