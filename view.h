/**
 * created by Huangyipu
 * 视图类
 */
#ifndef VIEW_H
#define VIEW_H

#include <QVector>
#include "boolstat.h"

const int VIEWBYTE = 128 + 1 + 4 + 256 + 256 + 256;

class DB;

class View
{
private:
    char _name[128];//视图名
    bool _isAll;//是否是所有的表
    int _numOfTable;//查询的表数量
    char _cov[256];//视图查询列名存储位置
    char _tov[256];//视图查询表名存储位置
    char _bov[256];//视图中判断语句存储位置

    QVector<QString> columnNames;//列名们
    QVector<QString> tableNames;//表名们
    QVector<BoolStat*> boolStats;//判断语句
public:
    friend class DB;

    int serialize(char buf[]);//序列化
    int deSerialize(char buf[]);//反序列化

    View(QString name, bool isAll, const QVector<QString> &columnNames_a,
         const QVector<QString> &tableNames_a, QVector<BoolStat*>& boolStats_a,
         QString cov, QString tov, QString bov);
    View();

    bool writeColumns();
    bool readColumns();

    bool writeTables();
    bool readTables();

    bool writeBoolStats();
    bool readBoolStats();

    QString getName();
};

#endif // VIEW_H
