/**
 * created by Huang
 * 索引描述类，存储索引信息
 */
#ifndef INDEX_H
#define INDEX_H


class Index
{
private:
    char _name[128];//索引名
    bool _unique;//是否唯一索引，true 为唯一索引，false 为非唯一索引
    bool _asc;//排序方式：true 为升序 asc, false 为降序
    int _filedNum;//字段数，最多可以保存 2 个
    char _filedName[128][2];//字段值，最多可以保存 2 个
    char _recordFile[256];//索引对应记录文件的路径
    char _indexFile[256];//索引数据文件的路径
public:
    Index();
};

#endif // INDEX_H
