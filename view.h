/**
 * created by Huangyipu
 * 视图类
 */
#ifndef VIEW_H
#define VIEW_H

#include <QVector>
#include "boolstat.h"

class View
{
private:
    char _name[128];//视图名
    bool isAll;//是否是所有的表
    int numOfTable;//查询的表数量
    char _path[256];//视图文件路径
    char _tov[256];//视图查询表名存储位置
    char _bov[256];//视图中判断语句存储位置

    QVector<QString> tableNames;//表名们
    QVector<BoolStat*> boolStats;//判断语句
public:
    int serialize(char buf[]);//序列化
    int deSerialize(char buf[]);//反序列化
    View(QString name, bool isAll, const QVector<QString> &tableNames_a, QVector<BoolStat*>& boolStats_a);
    View();
};

#endif // VIEW_H
