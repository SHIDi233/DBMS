#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->label_4->setOpenExternalLinks(true);
    ui->label_4->setText("<a href = https://github.com/SHIDi233/DBMS>https://github.com/SHIDi233/DBMS");
}

About::~About()
{
    delete ui;
}
