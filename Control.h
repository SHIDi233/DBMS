#ifndef CONTROL_H
#define CONTROL_H
#include "QString"
#include "db.h"
#include "user.h"
#include "qdir.h"
#include "QVector"
#include "QDataStream"

static QDir Path("D:\\QTtest\\testDB");//数据库路径
static User user("dba", "");//当前用户
static QDir rootPath(Path.absolutePath() + QString("\\") + user.getName());//数据库路径

static QVector<User*> users;

bool readUsers();
bool writeUsers();
bool login(QString name, QString pwd);

#endif // CONTROL_H
