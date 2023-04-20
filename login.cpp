#include "login.h"
#include "ui_login.h"
#include<QString>
#include <QMovie>
#include<QMouseEvent>
#include<QGraphicsDropShadowEffect>
#include"mainwindow.h"


QString button_start = "QPushButton{color:white;background-color:rgb(14 , 150 , 254);border-radius:5px;}";
QString button_press = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";
QString button_hover = "QPushButton{color:white;background-color:rgb(14 , 135 , 228);padding-left:3px;padding-top:3px;border-radius:5px;}";


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);


    QPixmap pix("://image/top.png");
    pix.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    QPixmap pix1("://image/user.png");
    pix1.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
    ui->label_2->setPixmap(pix1);
    ui->label_2->setScaledContents(true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label_4->setGraphicsEffect(shadow);
    ui->label_6->setGraphicsEffect(shadow);
    //QRect()
    ui->label_6->setGeometry(QRect(ui->label_6->geometry().left()+1,ui->label_6->geometry().top(),
                                   ui->label_6->geometry().width()-1,ui->label_6->geometry().height()));

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    MainWindow* mw = new MainWindow;
    mw->show();
    this->hide();
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



void Login::on_label_5_linkActivated(const QString &link)
{

}


void Login::on_pushButton_2_clicked()
{
    qApp->quit();
}

