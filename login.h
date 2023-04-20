#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_label_5_linkActivated(const QString &link);

    void on_pushButton_2_clicked();

private:
    Ui::Login *ui;

    int boundaryWidth;
    QPoint clickPos;

};

#endif // LOGIN_H
