#ifndef CONTROL_H
#define CONTROL_H
#include "QString"
#include "db.h"
#include "user.h"
#include "qdir.h"
#include "QVector"

static QDir rootPath("D:\\QTtest\\testDB");//数据库路径
static User user;//当前用户
static QVector<DB*> dbs;
static DB* db;//当前数据库


#endif // CONTROL_H
