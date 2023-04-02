/**
 * Created by Huang
 * 用户类，存储用户信息
 */
#ifndef USER_H
#define USER_H
#include "QString"

//权限
enum Permission
{
    DBA,//最高权限
    RESOURSE,//只可以创建实体，不可以创建数据库结构
    CONNECT//只可以登录Oracle，不可以创建实体，不可以创建数据库结构
    // TODO: 设计权限并增加
};

class User
{
private:
    char _name[128];//用户名
    char _pwd[128];//密码
    bool _UserType;//用户类型, true:管理员, false:普通用户
    bool _permission[10];//权限
public:
    User();
    QString createDb(QString name);//创建数据库
    QString createUser(QString name, QString pwd);//创建普通用户
    QString grant(Permission);//给予权限
};

#endif // USER_H
