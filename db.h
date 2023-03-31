/**
 *
 * DB控制器, 创建/删除库, 加载表, 控制表
 */
#ifndef DB_H
#define DB_H


class DB
{
public:
    DB();

    bool CreateTable();//创建表
    bool DeleteTable();//删除表
};

#endif // DB_H
