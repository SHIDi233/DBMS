#ifndef CONTROL_H
#define CONTROL_H
#include "QString"
#include "db.h"
#include "user.h"
#include "qdir.h"
#include "QVector"
#include "QDataStream"

static QDir Path("D:\\QTtest\\testDB");//数据库路径
//static User user("dba", "");//当前用户
//static User &user = user1;
static QDir rootPath("D:\\QTtest\\testDB");//数据库路径

static QString current_db;//当前选中数据库

static QVector<User*> users;

bool readUsers();
bool writeUsers();
bool login(QString name, QString pwd);

QString grantTo(QString name, Permission p);

#endif // CONTROL_H
