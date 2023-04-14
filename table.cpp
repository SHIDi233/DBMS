#include "table.h"
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

int Table::insert(int no,Basic_Data input){

}

int Table::insert(bool no[],Basic_Data input[]){
    for(int i=0;i<_fieldNum;i++){
        if(no[i]){
            //Basic_Data类型检测


        }
    }
}

int Table::insert(Row input){
    doList.append(WaitToDo_Table(WaitToDo_Table::INSERT,input));//将加入行放入缓存
}

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
