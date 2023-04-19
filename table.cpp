﻿#include "table.h"
#include "QDataStream"
#include "QDir"
#include "QDateTime"

Table::Table(QString name, QString tdf, QString tic, QString trd, QString tid, QString crtime)
    : _recordNum(0), _fieldNum(0)
{
    strcpy(_name, name.toLatin1().data());
    strcpy(_tdf, tdf.toLatin1().data());
    strcpy(_tic, tic.toLatin1().data());
    strcpy(_trd, trd.toLatin1().data());
    strcpy(_tid, tid.toLatin1().data());
    strcpy(_crtime, crtime.toLatin1().data());
    strcpy(_mtime, crtime.toLatin1().data());

}

Table::Table() {

}

Table::~Table() {
    // TODO: 完成函数
}

int Table::insert(int no,Basic_Data input){

}

int Table::insert(bool no[],Basic_Data input[]){
    for(int i=0;i<_fieldNum;i++){
        if(no[i]){
            //Basic_Data类型检测


        }
    }
}

//int Table::insert(Row input){
//    doList.append(WaitToDo_Table(WaitToDo_Table::INSERT,input));//将加入行放入缓存
//}

int Table::serialize(char buf[]) {
    int offset = 0;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, &_recordNum, 4);
    offset += 4;
    memcpy(buf + offset, &_fieldNum, 4);
    offset += 4;
    memcpy(buf + offset, _tdf, 256);
    offset += 256;
    memcpy(buf + offset, _tic, 256);
    offset += 256;
    memcpy(buf + offset, _trd, 256);
    offset += 256;
    memcpy(buf + offset, _tid, 256);
    offset += 256;
    memcpy(buf + offset, _crtime, 32);
    offset += 32;
    memcpy(buf + offset, _mtime, 256);
    offset += 32;

    return offset;
}

int Table::deSerialize(char buf[]) {
    int offset = 0;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(&_recordNum, buf + offset, 4);
    offset += 4;
    memcpy(&_fieldNum, buf + offset, 4);
    offset += 4;
    memcpy(_tdf, buf + offset, 256);
    offset += 256;
    memcpy(_tic, buf + offset, 256);
    offset += 256;
    memcpy(_trd, buf + offset, 256);
    offset += 256;
    memcpy(_tid, buf + offset, 256);
    offset += 256;
    memcpy(_crtime, buf + offset, 32);
    offset += 32;
    memcpy(_mtime, buf + offset, 256);
    offset += 32;

    return 0;
}

bool Table::writeColumn(Column* c) {

    char buf[COLUMNBYTE];
    int cnt = c->serialize(buf);

    QFile dbFile(_tdf);
    if(!dbFile.open(QIODevice::Append)) { return false; };
    QDataStream dbOut(&dbFile);
    dbOut.writeRawData(buf, cnt);
    dbFile.close();
    return true;
}

bool Table::writeColumns() {

    char buf[COLUMNBYTE];

    QFile dbFile(_tdf);
    if(!dbFile.open(QIODevice::Append)) { return false; };
    QDataStream dbOut(&dbFile);

    //循环写入
    for(auto &c : columns) {
        c->serialize(buf);
        dbOut.writeRawData(buf, COLUMNBYTE);
    }
    dbFile.close();

    return true;
}

bool Table::readColumns() {

    //清空表并释放空间
    for(auto &c : columns) { delete c; }
    columns.clear();

    //创建文件操作对象
    QFile dbFile(_tdf);
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[TABLEBYTE];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, TABLEBYTE);
        Column *c = new Column();
        c->deSerialize(buf);
        columns.append(c);
    }
    dbFile.close();

    return true;
}

QString Table::addColumn(QString columnName, TYPE type, int typeLen, int integrity) {

    //创建column类
    Column *column = new Column(columnName, type, typeLen, integrity);
    columns.append(column);

    //将字段信息写入[数据库名].db文件中
    writeColumn(column);

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //修改其他内容
    _fieldNum++;
    strcpy(_mtime, current_date.toLatin1().data());
}

QString Table::getName() { return QString(_name); }

int Table::commit() {
    int cnt = 0;
    //计算一个记录的字节大小
    int size = 0;
    for(auto &c : columns) {
        size += c->getTypeLen();
    }
    //创建文件操作对象
    QFile dbFile(_trd);
    if(!dbFile.open(QIODevice::WriteOnly)) { return false; };
    QDataStream recordOut(&dbFile);
    //创建缓冲区
    char *buf = new char[size];
    for(auto &r : rows) {
        r->serialize(buf, columns);
        recordOut.writeRawData(buf, size);
        cnt++;
    }
    dbFile.close();

    return cnt;
}

QString Table::insertRecord(const QVector<QString>& columnNameList, const QVector<QString>& valueList) {

    //先判断字段名是否都正确
    bool isIn;
    QVector<bool> isRead(columns.size(), false);//为防止传入字段名有重复
    for(auto &c : columnNameList) {
        isIn = false;
        for(int i = 0; i < columns.size(); i++) {//在字段里寻找是否有同名的
            if(columns[i]->getName().compare(c) == 0 && !isRead[i]) {//找到了就退出
                isIn = true;
                isRead[i] = true;
                break;
            }
        }
        if(!isIn) {//没找到直接退出寻找
            break;
        }
    }
    if(!isIn) { return "传入字段名有误"; }

    //创建新的插入
    Row *newRow = new Row();
    for(int i = 0; i < columns.size(); i++) {
//        if(!isRead[i]) {
//            Basic_Data *data = new NullData(columns[i]->getTypeLen());
//            newRow->addData(data);
//            continue;
//        }

        Basic_Data *data = nullptr;
        if(columns[i]->getType() == TYPE::BOOL) {
            data = new Bool();
        } else if (columns[i]->getType() == TYPE::VARCHAR) {
            data = new Varchar(columns[i]->getTypeLen());
        } else if (columns[i]->getType() == TYPE::DATETIME) {
            data = new DateTime();
        } else if (columns[i]->getType() == TYPE::DOUBLE) {
            data = new Double();
        } else if (columns[i]->getType() == TYPE::INTEGER) {
            data = new Integer();
        }
        for(int j = 0; j < columnNameList.size(); i++) {//寻找字段在哪一位
            if(columns[i]->getName().compare(columnNameList[j]) == 0) {
                data->setValue(valueList[j]);
                break;
            }
        }
        newRow->addData(data);
    }
    rows.push_back(newRow);
    return "插入成功";
}

QVector<QVector<QString>> Table::select(bool isAll, const QVector<QString>& column_names,
                                         const QVector<BoolStat>& boolStats) {
    QVector<QVector<QString>> res;
    if(isAll) {
        //将列名存入第一行
        QVector<QString> columnName;
        for(auto &c : columns) {
            columnName.push_back(c->getName());
        }
        //将每一行依次插入
        for(auto &r : rows) {
            // TODO: 判断此行是否符合要求

            QVector<QString> row;
            for(int i = 0; i < columns.size(); i++) {
                row.push_back(r->getValue(i));
            }
            res.push_back(row);
        }
        return res;
    }

    //先判断字段名是否都正确
    bool isIn;
    QVector<int> indexes(column_names.size());//记录查询字段所在位置
    QVector<bool> isRead(columns.size(), false);//为防止传入字段名有重复
    for(int j = 0; j < column_names.size(); j++) {
        isIn = false;
        for(int i = 0; i < columns.size(); i++) {//在字段里寻找是否有同名的
            if(columns[i]->getName().compare(column_names[j]) == 0 && !isRead[i]) {//找到了就退出
                isIn = true;
                isRead[i] = true;
                indexes[j] = i;
                break;
            }
        }
        if(!isIn) {//没找到直接退出寻找
            break;
        }
    }
    if(!isIn) { return res; }
    //将每一行插入
    for(auto &r : rows) {
        // TODO: 判断此行是否符合要求

        QVector<QString> row;
        for(auto &i : indexes) {
            row.push_back(r->getValue(i));
        }
        res.push_back(row);
    }
    return res;
}
