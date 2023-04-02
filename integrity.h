/**
 * created by Huang
 * 完整性描述类
 */
#ifndef INTEGRITY_H
#define INTEGRITY_H
#include "QString"

const int PRIMARYKEY = 10001,//主键约束
          FOREIGNKEY = 10002,//外键约束
          CHECK = 10003,//检查约束
          UNIQUE = 10004,//唯一约束
          NOTNULL = 10005,//非空约束
          DEFAULT = 10006,//默认约束
          IDENTITY = 10007;//自增

class Integrity
{
private:
    char _name[128];//约束名
    char _field[128];//被约束的字段名
    int _type;//约束类型
    char _param[256];//参数
public:
    Integrity(QString name, QString field, int type, QString param);
    Integrity(const char* name, const char* field, int type, const char* param);
};

#endif // INTEGRITY_H
