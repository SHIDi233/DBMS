#include "table.h"
#include "QDataStream"
#include "QDir"
#include "QDateTime"
#include "db.h"

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

Table::Table(QString name) {
    strcpy(_name, name.toLatin1().data());
}

Table::~Table() {
    for(auto &r : rows) {
        delete r;
        r = nullptr;
    }
    rows.clear();
    for(auto &c : columns) {
        delete c;
        c = nullptr;
    }
    columns.clear();
}

QVector<Column*>& Table::getColumns() {
    return columns;
}

QList<Row*>& Table::getRows() {
    return rows;
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
    memcpy(buf + offset, _mtime, 32);
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
    memcpy(_mtime, buf + offset, 32);
    offset += 32;

    readColumns();
    readRecord();
    readInteg();

    return offset;
}

bool Table::readInteg() {

    //清空容器并释放空间
    for(auto &i : integrities) { delete i; }
    integrities.clear();

    //创建文件操作对象
    QFile dbFile(_tic);
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[INTEGBYTE + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, INTEGBYTE);
        Integrity *i = new Integrity();
        i->deSerialize(buf);
        integrities.append(i);
    }
    dbFile.close();

    return true;
}

bool Table::writeColumn(Column* c) {

    char buf[COLUMNBYTE + 128];
    int cnt = c->serialize(buf);

    QFile dbFile(_tdf);
    if(!dbFile.open(QIODevice::Append)) { return false; };
    QDataStream dbOut(&dbFile);
    dbOut.writeRawData(buf, cnt);
    dbFile.close();
    return true;
}

bool Table::writeColumns() {

    char buf[COLUMNBYTE + 128];

    QFile dbFile(_tdf);
    if(!dbFile.open(QIODevice::WriteOnly)) { return false; };
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
    char buf[COLUMNBYTE + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, COLUMNBYTE);
        Column *c = new Column();
        c->deSerialize(buf);
        c->setTable(getName());
        columns.append(c);
    }
    dbFile.close();

    return true;
}

int Table::getRowByte() {
    int res = 0;
    for(auto &c : columns) {
        res += c->getTypeLen();
    }
    return res;
}

bool Table::readRecord() {
    //清空表并释放空间
    for(auto &r : rows) { delete r; }
    rows.clear();

    //创建文件操作对象
    QFile dbFile(_trd);
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);
    //计算一个记录的字节大小
    int size = 0;
    for(auto &c : columns) {
        size += c->getTypeLen();
    }
    //循环将表信息读入列表中
    char *buf = new char[size + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, size);
        Row *row = new Row();
        row->deSerialize(buf, columns);
        rows.append(row);
    }
    dbFile.close();

    return true;
}

QString Table::addColumn(QString columnName, TYPE type, int typeLen, int integrity) {

    //创建column类
    Column *column = new Column(columnName, type, typeLen, integrity);
    column->setTable(getName());
    columns.append(column);

    //将字段信息写入[数据库名].db文件中
    writeColumn(column);

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    for(auto &r : rows) {
        Basic_Data *b = nullptr;
        if(type == TYPE::INTEGER) {
            b = new Integer();
        } else if(type == TYPE::VARCHAR) {
            b = new Varchar(typeLen);
        } else if(type == TYPE::BOOL) {
            b = new Bool();
        } else if(type == TYPE::DOUBLE) {
            b = new Double();
        } else {
            b = new DateTime();
        }
        r->addData(b);
    }

    //修改其他内容
    _fieldNum++;
    strcpy_s(_mtime, current_date.toLatin1().data());
    return "增加列成功";
}

QString Table::dropColumn(QString columnName) {
    bool found = false;
    for(int i = 0; i < columns.size(); i++) {
        if(columns[i]->getName().compare(columnName) == 0) {
            columns.remove(i);
            found = true;
            break;
        }
    }
    if(!found) { return "未找到字段"; }
    writeColumns();
    return "删除成功";
}

QString Table::modifyColumn(QString columnName, TYPE newType, int newTypeLen, int integrity) {
    bool found = false;
    for(int i = 0; i < columns.size(); i++) {
        if(columns[i]->getName().compare(columnName) == 0) {
            columns[i]->modify(newType, newTypeLen, integrity);
            found = true;
            break;
        }
    }
    if(!found) { return "未找到字段"; }
    writeColumns();//重新写入字段到文件
    return "字段修改成功";
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
    for(int k = 0; k < columnNameList.size(); k++) {
        isIn = false;
        for(int i = 0; i < columns.size(); i++) {//在字段里寻找是否有同名的
            if(columns[i]->getName().compare(columnNameList[k]) == 0 && !isRead[i]) {//找到了就退出
                isIn = true;
                isRead[i] = true;

                //判断是否符合主键约束
                if(columns[i]->getIntegrities() == ITGTYPE::PRIMARYKEY) {
                    for(auto &r : rows) {
                        if(valueList[k] == r->getValue(i)) {
                            return "违反主键约束";
                        }
                    }
                }

                break;
            }
        }
        if(!isIn) {//没找到直接退出寻找
            break;
        }
    }
    if(!isIn) { return "传入字段名有误"; }

    //判断是否符合约束
    QVector<int> uniqueIndex;
    for(int i = 0; i < columnNameList.size(); i++) {
        for(int j = 0; j < integrities.size(); j++) {
            if(integrities[j]->getField() == columnNameList[i] && integrities[j]->getType() == ITGTYPE::UNIQUE) {
                uniqueIndex.push_back(i);
            }
        }
    }
    if(!uniqueIndex.empty()) {
        QVector<BoolStat*> b;
        QVector<QVector<QString>> d = select(false, columnNameList, b);
        for(auto &i : uniqueIndex) {
            for(auto &d1 : d) {
                if(d1[i] == valueList[i]) {
                    return "违反完整性约束";
                }
            }
        }
    }

    //创建新的插入
    Row *newRow = new Row();
    Basic_Data *data = nullptr;
    for(int i = 0; i < columns.size(); i++) {
//        if(!isRead[i]) {
//            Basic_Data *data = new NullData(columns[i]->getTypeLen());
//            newRow->addData(data);
//            continue;
//        }
//        if(!isRead[i]) continue;
        data = nullptr;
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
        for(int j = 0; j < columnNameList.size(); j++) {//寻找字段在哪一位
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

QString Table::updateRecord(const QVector<QString>& columnNameList,
                            const QVector<QString> valueList, QVector<BoolStat*>& boolStats) {
    //寻找符合要求的行
    QVector<int> rowIndex;
    for(int i = 0; i < rows.size(); i++) {
        bool isOk = true;
        for(auto &b : boolStats) {
            for(int j = 0; j < columns.size(); j++) {
                if(columns[j]->getName().compare(b->getColumnName()) == 0) {
                    if(b->getConnect()) {
                        isOk &= b->judge(rows[i]->getData(j));
                    } else {
                        isOk |= b->judge(rows[i]->getData(j));
                    }
                    break;
                }
            }
        }
        if(isOk) {
            rowIndex.push_back(i);
        }
    }
    //寻找符合要修改的列
    bool isIn;
    QVector<int> columnIndex(columnNameList.size());
    QVector<bool> isRead(columns.size(), false);//为防止传入字段名有重复
    for(int j = 0; j < columnNameList.size(); j++) {
        isIn = false;
        for(int i = 0; i < columns.size(); i++) {//在字段里寻找是否有同名的
            if(columns[i]->getName().compare(columnNameList[j]) == 0 && !isRead[i]) {//找到了就退出
                isIn = true;
                isRead[i] = true;
                columnIndex[j] = i;

                //判断是否符合主键约束
                if(columns[i]->getIntegrities() == ITGTYPE::PRIMARYKEY) {
                    for(auto &r : rows) {
                        if(valueList[j] == r->getValue(i)) {
                            return "违反主键约束";
                        }
                    }
                }
                break;
            }
        }
        if(!isIn) {//没找到直接退出寻找
            return "未找到字段";
        }
    }

    //修改数据
    bool res = true;
    for(auto &r : rowIndex) {
        for(int i = 0; i < columnIndex.size(); i++) {
            res &= rows[r]->setData(columnIndex[i], valueList[i]);
        }
        if(!res) { return "数据类型错误, 修改失败"; }
    }
    return "修改成功";
}

QString Table::deleteRecord(QVector<BoolStat*> &boolStats) {

    //判断该行是否符合要求
    QVector<int> rowIndex;
    for(int i = 0; i < rows.size(); i++) {
        bool isOk = true;
//        for(auto &b : boolStats) {
//            for(int j = 0; j < columns.size(); j++) {
//                QString columnName_tem;
//                if(b->getColumnName().contains('.')) {
//                    columnName_tem = columns[j]->getTable()+"."+columns[j]->getName();
//                } else {
//                    columnName_tem = columns[j]->getName();
//                }
//                if(columnName_tem.compare(b->getColumnName()) == 0) {
//                    QString value_tem = b->getValue();
//                    if(b->getType() == BoolType::COMPARE) {
//                        if(value_tem.contains('.')) {
//                            for(int k = 0; k < columns.size(); k++) {
//                                if(value_tem == columns[k]->getTable() + "." + columns[k]->getName()) {
//                                    b->setValue(rows[i]->getValue(k));
//                                    break;
//                                }
//                            }
//                        } else {
//                            for(int k = 0; k < columns.size(); k++) {
//                                if(value_tem == columns[k]->getName()) {
//                                    b->setValue(rows[i]->getValue(k));
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                    if(b->getConnect()) {
//                        isOk &= b->judge(rows[i]->getData(j));
//                    } else {
//                        isOk |= b->judge(rows[i]->getData(j));
//                    }
//                    b->setValue(value_tem);
//                    break;
//                }
//            }
//        }
        for(auto &b : boolStats) {
            for(int j = 0; j < columns.size(); j++) {
                if(columns[j]->getName().compare(b->getColumnName()) == 0) {
                    if(b->getConnect()) {
                        isOk &= b->judge(rows[i]->getData(j));
                    } else {
                        isOk |= b->judge(rows[i]->getData(j));
                    }
                    break;
                }
            }
        }
        if(isOk) {
            rowIndex.push_back(i);
        }
    }

    for(auto &i : rowIndex) {
        rows.removeAt(i);
    }
    return "修改成功";
}

QVector<QVector<QString>> Table::select(bool isAll, const QVector<QString>& column_names,
                                        QVector<BoolStat*>& boolStats) {
    QVector<QVector<QString>> res;

    //判断该行是否符合要求
    QVector<int> rowIndex;
    for(int i = 0; i < rows.size(); i++) {
        bool isOk = true;
        for(auto &b : boolStats) {
            for(int j = 0; j < columns.size(); j++) {
                QString columnName_tem;
                if(b->getColumnName().contains('.')) {
                    columnName_tem = columns[j]->getTable()+"."+columns[j]->getName();
                } else {
                    columnName_tem = columns[j]->getName();
                }
                if(columnName_tem.compare(b->getColumnName()) == 0) {
                    QString value_tem = b->getValue();
                    if(b->getType() == BoolType::COMPARE) {
                        if(value_tem.contains('.')) {
                            for(int k = 0; k < columns.size(); k++) {
                                if(value_tem == columns[k]->getTable() + "." + columns[k]->getName()) {
                                    b->setValue(rows[i]->getValue(k));
                                    break;
                                }
                            }
                        } else {
                            for(int k = 0; k < columns.size(); k++) {
                                if(value_tem == columns[k]->getName()) {
                                    b->setValue(rows[i]->getValue(k));
                                    break;
                                }
                            }
                        }
                    }
                    if(b->getConnect()) {
                        isOk &= b->judge(rows[i]->getData(j));
                    } else {
                        isOk |= b->judge(rows[i]->getData(j));
                    }
                    b->setValue(value_tem);
                    break;
                }
            }
        }
        if(isOk) {
            rowIndex.push_back(i);
        }
    }

    if(isAll) {
        //将列名存入第一行
        QVector<QString> columnName;
        for(auto &c : columns) {
            columnName.push_back(c->getName());
        }
        res.push_back(columnName);
        //将每一行依次插入
        for(auto &c : rowIndex) {
//            //判断该行是否符合要求
//            bool isOk = true;
//            for(auto &b : boolStats) {
//                for(int i = 0; i < columns.size(); i++) {
//                    if(columns[i]->getName().compare(b.getColumnName())) {
//                        if(b.getConnect()) {
//                            isOk &= b.judge(*r->getData(i));
//                        } else {
//                            isOk |= b.judge(*r->getData(i));
//                        }
//                        break;
//                    }
//                }
//                if(!isOk) { break; }
//            }
//            if(!isOk) { continue; }//不符合继续检查下一行

            QVector<QString> row;
            for(int i = 0; i < columns.size(); i++) {
                row.push_back(rows[c]->getValue(i));
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
            QString columnName_tem;
            if(column_names[j].contains('.')) {
                columnName_tem = columns[i]->getTable()+"."+columns[i]->getName();
            } else {
                columnName_tem = columns[i]->getName();
            }
            if(columnName_tem.compare(column_names[j]) == 0 && !isRead[i]) {//找到了就退出
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

    //第一行插入列名
    QVector<QString> columnName;
    for(auto &c : column_names) {
        columnName.push_back(c);
    }
    res.push_back(columnName);
    //将每一行插入
    for(auto &c : rowIndex) {
//        //判断该行是否符合要求
//        bool isOk = true;
//        for(auto &b : boolStats) {
//            for(int i = 0; i < columns.size(); i++) {
//                if(columns[i]->getName().compare(b.getColumnName())) {
//                    if(b.getConnect()) {
//                        isOk &= b.judge(*r->getData(i));
//                    } else {
//                        isOk |= b.judge(*r->getData(i));
//                    }
//                    break;
//                }
//            }
//            if(!isOk) { break; }
//        }
//        if(!isOk) { continue; }//不符合继续检查下一行

        QVector<QString> row;
        for(auto &i : indexes) {
            row.push_back(rows[c]->getValue(i));
        }
        res.push_back(row);
    }
    return res;
}

//约束管理
QString Table::addIntegrity(QString integName, QString filed, ITGTYPE type, QString param) {

    bool isFound = false;
    if(type == ITGTYPE::PRIMARYKEY) {
        for(auto &c : columns) {
            if(c->getName() == filed) {
                c->setPK();
                isFound = true;
            }
        }
    } else {
        for(auto &c : columns) {
            if(c->getName() == filed) {
                isFound = true;
            }
        }
    }
    if(!isFound) { return "未找到列"; }
    //创建column类
    Integrity *i = new Integrity(integName, filed, type, param);

    //将字段信息写入完整性文件中
    char buf[INTEGBYTE + 128];
    int cnt = i->serialize(buf);

    QFile dbFile(_tic);
    if(!dbFile.open(QIODevice::Append)) { return "完整性文件丢失"; };
    QDataStream dbOut(&dbFile);
    dbOut.writeRawData(buf, cnt);
    dbFile.close();

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //修改其他内容
    strcpy_s(_mtime, current_date.toLatin1().data());
    integrities.append(i);

    return "增加约束成功";
}
