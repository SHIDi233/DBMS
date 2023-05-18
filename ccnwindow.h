#ifndef CCNWINDOW_H
#define CCNWINDOW_H

#include <QMainWindow>
#include"client.h"

namespace Ui {
class CCNWindow;
}

class CCNWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void _showTableAll(QVector<QVector<QString>>);//显示表信息
public:
    explicit CCNWindow(Client* c = nullptr,QWidget *parent = nullptr);
    ~CCNWindow();

public slots:
    void showTableAll(QVector<QVector<QString>>);//显示表信息
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CCNWindow *ui;

    QPoint clickPos;
};

#endif // CCNWINDOW_H
