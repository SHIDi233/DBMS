/**
 * Created by Huang
 * 用户类，存储用户信息
 */
#ifndef USER_H
#define USER_H
#include "QString"
#include "db.h"

const int USERBYTE = 128 + 128 + 4;

//权限
enum Permission
{
    DBA = 3,//最高权限，可以创建删除数据库
    AD = 2,//管理员，可以插入删除数据并修改表结构
    USER = 1,//可以查询表，插入并删除数据
    VISITOR = 0//只可以查询表
};

class User
{
private:
    static User* user_one1;

    char _name[128];//用户名
    char _pwd[128];//密码
    Permission _permission;//权限

    QVector<DB*> dbs;
public:
    User() {};
    User(QString name, QString pwd);
    QString createDb(QString name);//创建数据库
    QString dropDB(QString name);//删除数据库
    QString createUser(QString name, QString pwd);//创建普通用户
    QString grant(QString name, Permission p);//给予权限

    DB* getDB(QString dbName);
    bool loadDB();//加载数据库
    QVector<DB*>& getDbs();
    Permission getPer();
    QString getName();
    void setPer(Permission p){ _permission = p; };

    bool log(QString name, QString pwd);

    int serialize(char buf[]);
    int deSerialize(char buf[]);

    static User* getUser()
            {
            if(user_one1 == nullptr)
                user_one1 = new User("dba", "");
            return user_one1;
        }
    void set(User* u) {
        strcpy(_name, u->_name);
        _permission = u->_permission;
//        user_one1 = u;
    }
};

#endif // USER_H
