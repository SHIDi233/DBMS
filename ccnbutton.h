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

    void setEnter(QString s);
    void setLeave(QString s);

private:
    QString s_close_1;
    QString s_close_2;
};

#endif // CCNBUTTON_H
