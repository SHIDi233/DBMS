#include "ccnbutton.h"
#include<QString>


CCNButton::CCNButton(QWidget* parent) :QPushButton(parent)
{
}

CCNButton::~CCNButton()
{
}

void CCNButton::enterEvent(QEvent *)
{
    setStyleSheet(s_close_1);
}

void CCNButton::leaveEvent(QEvent *)
{
    setStyleSheet(s_close_2);
}

void CCNButton::setEnter(QString s){
    s_close_1 = s;
}

void CCNButton::setLeave(QString s){
    s_close_2 = s;
}
