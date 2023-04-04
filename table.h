/**
 *
 * 表类，表的增删改查
 */
#ifndef TABLE_H
#define TABLE_H
#include<QString>


class Table
{
private:
    char _name[128];//表格名称
    int n;

    int _recordNum;//记录数
    int _fieldNum;//字段数
    char _tdf[256];//表格定义文件路径
    char _tic[256];//表格完整性文件路径
    char _trd[256];//表格记录文件路径
    char _tid[256];//表格索引文件路径

    char _crtime[32];//创建时间
    char _mtime[32];//最后修改时间
public:
    Table(QString name, QString tdf, QString tic, QString trd, QString tid, QString crtime);//创建表时调用这个函数

};

#endif // TABLE_H
