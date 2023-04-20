#include "db.h"
#include "cstring"
#include "QDir"
#include "QDateTime"
#include "QDataStream"

DB::DB(QString name, bool type, QString filePath, QString crtime)
{
    //赋值
    strcpy(_name, name.toLatin1().data());
    _type = type;
    strcpy(_filePath, filePath.toLatin1().data());
    strcpy(_crtime, crtime.toLatin1().data());
}

DB::DB() {
}

QString DB::getName() {
    return _name;
}

QList<Table*>& DB::getTable() {
    return tables;
}

bool DB::writeTable(Table* t, QString filePath) {

    char buf[TABLEBYTE+128];
    int cnt = t->serialize(buf);

    //创建文件操作对象
    QFile dbFile(filePath);
    if(!dbFile.open(QIODevice::Append)) { return false; }//判断文件是否存在
    QDataStream dbOut(&dbFile);
    dbOut.writeRawData(buf, cnt);

    return true;
}

bool DB::readTables(QString filePath) {

    //清空表并释放空间
    //for(auto &t : tables) { delete t; }
    //tables.clear();

    //创建文件操作对象
    QFile dbFile(filePath);
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);

//    bool found = false;//表示是否在文件中找到表
//    //在文件中寻找表
//    char nameBuf[128];
//    while(!dbOut.atEnd()) {
//        dbOut.readRawData(nameBuf, 128);
//        dbOut.skipRawData(-128);
//        if(t.getName().compare(nameBuf) == 0) {
//            found = true;
//            break;
//        }
//        dbOut.skipRawData(TABLEBYTE);
//    }
//    if(!found) { return false; }

    //循环将表信息读入列表中
    char buf[TABLEBYTE + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, TABLEBYTE);
        Table *t = new Table();
        t->deSerialize(buf);
        tables.push_back(t);
    }
//    t.deSerialize(buf);

    return true;
}

bool DB::writeTables(QString filePath) {

    char buf[TABLEBYTE + 128];
    int cnt;

    //创建文件操作对象
    QFile dbFile(filePath);
    if(!dbFile.open(QIODevice::WriteOnly)) { return false; }//判断文件是否存在
    QDataStream dbOut(&dbFile);

    //循环写入数据
    for(auto &t : tables) {
        cnt = t->serialize(buf);
        dbOut.writeRawData(buf, cnt);
    }

    return true;
}

QString DB::createTable(QString tableName){

    QDir path(_filePath);

    //创建表定义文件
    QFile tdfFile(path.absoluteFilePath(tableName + ".tdf"));
    if(!tdfFile.open(QIODevice::WriteOnly)) { return "表定义文件创建失败"; };
    tdfFile.close();

    //创建记录文件
    QFile trdFile(path.absoluteFilePath(tableName + ".trd"));
    if(!trdFile.open(QIODevice::WriteOnly)) { return "记录文件创建失败"; };
    trdFile.close();

    //创建完整性描述文件
    QFile ticFile(path.absoluteFilePath(tableName + ".tic"));
    if(!ticFile.open(QIODevice::WriteOnly)) { return "完整性文件创建失败"; };
    ticFile.close();

    //创建索引描述文件
    QFile tidFile(path.absoluteFilePath(tableName + ".tid"));
    if(!tidFile.open(QIODevice::WriteOnly)) { return "索引描述文件创建失败"; };
    tidFile.close();

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //创建表类
    Table *table = new Table(tableName, path.absoluteFilePath(tableName + ".tdf"),
                path.absoluteFilePath(tableName + ".trd"),
                path.absoluteFilePath(tableName + ".tic"),
                path.absoluteFilePath(tableName + ".tid"), current_date);

    //将表信息写入[数据库名].db文件中
    tables.append(table);
    writeTable(table, path.absoluteFilePath(QString(_name) + ".tb"));

    return "表格创建成功";
}

QString DB::dropTable(QString tableName) {
    //寻找数据库
    bool found = false;
    for(int i = 0; i < tables.size(); i++) {
        if(tables[i]->getName().compare(tableName) == 0) {
            tables.removeAt(i);
            found = true;
        }
    }
    if(!found) { return "未找到表"; }

    //重新写入
    QDir path(_filePath);
    writeTables(path.absoluteFilePath(QString(_name) + ".tb"));

    return "表格删除成功";
}

QString DB::addColumn(QString tableName, QString columnName, TYPE type, int typeLen, Integrity *integrity) {
    // TODO: 判断约束类型
    int integ = 0;
    if(integrity == nullptr) {
        integ = 0;
    }
    QDir path(_filePath);
    bool found = false;//表示是否找指定表

    //readTables(path.absoluteFilePath(QString(_name) + ".tb"));//读取表信息
    for(auto &t : tables) {
        if(t->getName().compare(tableName) == 0) {
            found = true;
            t->addColumn(columnName, type, typeLen, integ);
            break;
        }
    }

    if(!found) { return "未找到表"; }

    writeTables(path.absoluteFilePath(QString(_name) + ".tb"));//将表信息重新写入
    return "修改表成功";
}

QString DB::dropColumn(QString tableName, QString columnName) {

    bool found = false;//表示是否找指定表
    for(auto &t : tables) {
        if(t->getName().compare(tableName)) {
            found = true;
            t->dropColumn(columnName);
            break;
        }
    }
    if(!found) { return "未找到表"; }
    return "字段删除成功";
}

int DB::serialize(char buf[]) {
    int offset = 0;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, &_type, 1);
    offset += 1;
    memcpy(buf + offset, _filePath, 256);
    offset += 256;
    memcpy(buf + offset, _crtime, 32);
    offset += 32;

    return offset;
}

int DB::deSerialize(char buf[]) {
    int offset = 0;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(&_type, buf + offset, 1);
    offset += 1;
    memcpy(_filePath, buf + offset, 256);
    offset += 256;
    memcpy(_crtime, buf + offset, 32);
    offset += 32;

    //加载table
    QDir path(_filePath);
    readTables(path.absoluteFilePath(QString(_name) + ".tb"));

    return 0;
}

QString DB::insertRecord(QString tableName, const QVector<QString>& columnNameList, const QVector<QString>& valueList) {
    for(auto &t : tables) {
        if(tableName.compare(t->getName()) == 0) {
            return t->insertRecord(columnNameList, valueList);
        }
    }
    return "未找到表";
}

QVector<QVector<QString>> DB::select(bool isAll, const QVector<QString>& column_names,
                                     QString tableName, const QVector<BoolStat>& boolStats) {
    for(auto &t : tables) {
        if(tableName.compare(t->getName()) == 0) {
            return t->select(isAll, column_names, boolStats);
        }
    }
    QVector<QVector<QString>> res;
    return res;
}
