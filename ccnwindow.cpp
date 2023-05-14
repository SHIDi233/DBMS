#include "ccnwindow.h"
#include "ui_ccnwindow.h"
#include"mainwindow.h"
#include<QMouseEvent>
#include<QDesktopWidget>

CCNWindow::CCNWindow(Client* c,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CCNWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2-20);

    ui->gridLayoutWidget->setGeometry(1,30,this->width()-2,this->height()-32);

    ui->label->move(this->width()/2-ui->label->width()/2,15-ui->label->height()/2);

    MainWindow *mw;
    if(c==nullptr){
        mw = new MainWindow();//创建对象
    }
    else{
        mw = new MainWindow(c);//创建对象
    }
    ui->gridLayout->addWidget(mw);//添加到Layout中
    connect(this,SIGNAL(_showTableAll(QVector<QVector<QString>>)),mw,SLOT(showTableAll(QVector<QVector<QString>>)));


    QPixmap pix("://image/user_icon.png");
    pix.scaled(ui->label_2->size(), Qt::KeepAspectRatio);
    ui->label_2->setPixmap(pix);
    ui->label_2->setScaledContents(true);


    ui->pushButton->setEnter("QPushButton{background-color:rgb(255,0,0);color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");
    ui->pushButton->setLeave("QPushButton{color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");

    ui->pushButton_2->setEnter("QPushButton{background-color:rgb(200,200,200);color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");
    ui->pushButton_2->setLeave("QPushButton{color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");

    ui->pushButton_3->setEnter("QPushButton{background-color:rgb(200,200,200);color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");
    ui->pushButton_3->setLeave("QPushButton{color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");

}

CCNWindow::~CCNWindow()
{
    delete ui;
}

void CCNWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void CCNWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton  //左键点击并且移动
//            && e->pos().x()>=0      //范围在窗口的上面部分
//            && e->pos().y()>=0
//            && e->pos().x()<= geometry().width()
//            && e->pos().y() <= geometry().height()/10)
            )
    {
        move(e->pos()+pos()-clickPos);  //移动窗口
    }
}

void CCNWindow::on_pushButton_clicked()
{
    qApp->quit();
}


void CCNWindow::showTableAll(QVector<QVector<QString>> table){
    emit _showTableAll(table);
}
