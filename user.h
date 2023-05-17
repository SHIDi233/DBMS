/**
 * Created by Huang
 * 用户类，存储用户信息
 */
#ifndef USER_H
#define USER_H
#include "QString"
#include "db.h"

//权限
enum Permission
{
    DBA = 3,//最高权限，可以创建删除数据库
    AD = 2,//管理员，可以插入删除数据并修改表结构
    USER = 1,//可以查询表，插入并删除数据
    VISITOR = 0//只可以查询表
    // TODO: 设计权限并增加
};

class User
{
private:
    char _name[128];//用户名
    char _pwd[128];//密码
    bool _UserType;//用户类型, true:管理员, false:普通用户
    Permission _permission;//权限

    QVector<DB*> dbs;
public:
    User();
    QString createDb(QString name);//创建数据库
    QString dropDB(QString name);//删除数据库
    QString createUser(QString name, QString pwd);//创建普通用户
    QString grant(Permission);//给予权限

    DB* getDB(QString dbName);
    bool loadDB();//加载数据库
    QVector<DB*>& getDbs();
    Permission getPer();
};

#endif // USER_H
