#include "ccnbutton.h"

CCNButton::CCNButton(QWidget* parent) :QPushButton(parent)
{
}

CCNButton::~CCNButton()
{
}

void CCNButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{background-color:rgb(255,0,0);color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");
}

void CCNButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{color: rgb(255, 255, 255);selection-background-color: rgb(255, 0, 0);border:0px;}");
}

