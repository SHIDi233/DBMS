/**
 *
 * 表类，表的增删改查
 */
#ifndef TABLE_H
#define TABLE_H
#include<QString>


class Table
{
public:
    Table();

    char name[128];//表格名称
    int n;

    int record_num;//记录数
    int field_num;//字段数
    char tdf[256];//表格定义文件路径
    char tic[256];//表格完整性文件路径
    char trd[256];//表格记录文件路径
    char tid[256];//表格索引文件路径

    QString crtime;//创建时间
    QString mtime;//最后修改时间

};

#endif // TABLE_H
