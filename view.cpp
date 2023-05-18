#include "view.h"
#include "QFile"
#include "QDataStream"

View::View()
{

}

View::View(QString name, bool isAll, const QVector<QString> &columnNames_a,
     const QVector<QString> &tableNames_a, QVector<BoolStat*>& boolStats_a,
     QString cov, QString tov, QString bov) :
    _isAll(isAll), columnNames(columnNames_a), tableNames(tableNames_a), boolStats(boolStats_a)
{
    strcpy(_name, name.toLatin1().data());
    strcpy(_cov, cov.toLatin1().data());
    strcpy(_tov, tov.toLatin1().data());
    strcpy(_bov, bov.toLatin1().data());

    writeColumns();
    writeTables();
    writeBoolStats();
}

int View::serialize(char buf[]) {

    int offset = 0;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, &_isAll, 1);
    offset += 1;
    memcpy(buf + offset, &_numOfTable, 4);
    offset += 4;
    memcpy(buf + offset, _cov, 256);
    offset += 256;
    memcpy(buf + offset, _tov, 256);
    offset += 256;
    memcpy(buf + offset, _bov, 256);
    offset += 256;

    return offset;
}

int View::deSerialize(char buf[]) {

    int offset = 0;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(&_isAll, buf + offset, 1);
    offset += 1;
    memcpy(&_numOfTable, buf + offset, 4);
    offset += 4;
    memcpy(_cov, buf + offset, 256);
    offset += 256;
    memcpy(_tov, buf + offset, 256);
    offset += 256;
    memcpy(_bov, buf + offset, 256);
    offset += 256;

    readColumns();
    readTables();
    readBoolStats();

    return offset;
}

bool View::writeColumns() {
    //创建文件操作对象
    QFile covFile(_cov);
    if(!covFile.open(QIODevice::WriteOnly)) { return false; }//判断文件是否存在
    QDataStream covOut(&covFile);

    for(auto &c : columnNames) {
        covOut << c;
    }

    return true;
}

bool View::readColumns() {
    //创建文件操作对象
    QFile covFile(_cov);
    if(!covFile.open(QIODevice::ReadOnly)) { return false; }//判断文件是否存在
    QDataStream covOut(&covFile);

    while(!covOut.atEnd()) {
        QString tem;
        covOut >> tem;
        columnNames.append(tem);
    }

    return true;
}

bool View::writeTables() {
    //创建文件操作对象
    QFile tovFile(_tov);
    if(!tovFile.open(QIODevice::WriteOnly)) { return false; }//判断文件是否存在
    QDataStream tovOut(&tovFile);

    for(auto &t : tableNames) {
        tovOut << t;
    }

    return true;
}

bool View::readTables() {
    //创建文件操作对象
    QFile tovFile(_tov);
    if(!tovFile.open(QIODevice::ReadOnly)) { return false; }//判断文件是否存在
    QDataStream tovOut(&tovFile);

    while(!tovOut.atEnd()) {
        QString tem;
        tovOut >> tem;
        tableNames.append(tem);
    }

    return true;
}

bool View::writeBoolStats() {
    //创建文件操作对象
    QFile bovFile(_bov);
    if(!bovFile.open(QIODevice::WriteOnly)) { return false; }//判断文件是否存在
    QDataStream bovOut(&bovFile);

    for(auto &b : boolStats) {
        b->writeToFile(bovOut);
    }

    return true;
}

bool View::readBoolStats() {
    //创建文件操作对象
    QFile bovFile(_bov);
    if(!bovFile.open(QIODevice::ReadOnly)) { return false; }//判断文件是否存在
    QDataStream bovOut(&bovFile);

    while(!bovOut.atEnd()) {
        QString type;
        bovOut >> type;
        if(type == "0") {
            QString columnName, value, ctype, connect;
            bool con;
            bovOut >> columnName >> value >> ctype >> connect;
            if(connect == "0") {
                con = false;
            } else {
                con = true;
            }
            BoolStat *b = new Compare(columnName, value, ctype, con);
            boolStats.append(b);
        } else if(type == "1") {
            QString columnName, value1, value2, connect;
            bool con;
            bovOut >> columnName >> value1 >> value2 >> connect;
            if(connect == "0") {
                con = false;
            } else {
                con = true;
            }
            BoolStat *b = new Between(columnName, value1, value2, con);
            boolStats.append(b);
        } else if(type == "2") {
            QString columnName, connect;
            int size;
            bool con;
            bovOut >> columnName >> size;
            QVector<QString> v(size);
            for(int i = 0; i < size; i++) {
                bovOut >> v[i];
            }
            bovOut >> connect;
            if(connect == "0") {
                con = false;
            } else {
                con = true;
            }
            BoolStat *b = new IsIn(columnName, v, con);
            boolStats.append(b);
        }
    }

    return true;
}

QString View::getName() {
    return _name;
}
