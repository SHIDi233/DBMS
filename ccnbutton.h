#ifndef CCNBUTTON_H
#define CCNBUTTON_H


#include<QPushButton>
#include<QEvent>
class CCNButton :public QPushButton
{
    Q_OBJECT;

public:
    CCNButton(QWidget *parent = 0);
    ~CCNButton();

public:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
};

#endif // CCNBUTTON_H
