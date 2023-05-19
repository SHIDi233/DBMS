#include "db.h"
#include "cstring"
#include "QDir"
#include "QDateTime"
#include "QDataStream"
#include "Control.h"

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
    for(auto &t : tables) { delete t; }
    tables.clear();

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

    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法删除字段";
    }

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
                path.absoluteFilePath(tableName + ".tic"),
                path.absoluteFilePath(tableName + ".trd"),
                path.absoluteFilePath(tableName + ".tid"), current_date);

    //将表信息写入[数据库名].db文件中
    tables.append(table);
    writeTable(table, path.absoluteFilePath(QString(_name) + ".tb"));

    return "表格创建成功";
}

QString DB::dropTable(QString tableName) {

    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法删除表";
    }

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
    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法创建字段";
    }

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
    return "增加字段成功";
}

QString DB::modifyColumn(QString tableName, QString columnName, TYPE type, int typeLen, Integrity *integrity) {
    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法创建字段";
    }

    int integ = 0;
    if(integrity == nullptr) {
        integ = 0;
    }
//    QDir path(_filePath);
    bool found = false;//表示是否找指定表

    //readTables(path.absoluteFilePath(QString(_name) + ".tb"));//读取表信息
    for(auto &t : tables) {
        if(t->getName().compare(tableName) == 0) {
            found = true;
            t->modifyColumn(columnName, type, typeLen, integ);
            break;
        }
    }

    if(!found) { return "未找到表"; }

//    writeTables(path.absoluteFilePath(QString(_name) + ".tb"));//将表信息重新写入
    return "修改字段成功";
}

QString DB::dropColumn(QString tableName, QString columnName) {
    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法删除字段";
    }
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
    readViews(path.absoluteFilePath(QString(_name) + ".viw"));

    return 0;
}

QString DB::insertRecord(QString tableName, const QVector<QString>& columnNameList, const QVector<QString>& valueList) {

    //判断用户权限
    if(User::getUser()->getPer() < Permission::USER) {
        return "权限不足，无法删除字段";
    }

    for(auto &t : tables) {
        if(tableName.compare(t->getName()) == 0) {
            return t->insertRecord(columnNameList, valueList);
        }
    }
    return "未找到表";
}

QString DB::updateRecord(QString tableName, const QVector<QString>& columnNameList, const QVector<QString> &valueList, QVector<BoolStat*> &boolStats) {

    //判断用户权限
    if(User::getUser()->getPer() < Permission::USER) {
        return "权限不足，无法删除字段";
    }

    for(auto &t : tables) {
        if(tableName.compare(t->getName()) == 0) {
            return t->updateRecord(columnNameList, valueList, boolStats);
        }
    }
    return "未找到表";
}

QString DB::deleteRecord(QString tableName, QVector<BoolStat*>& boolStats) {
    //判断用户权限
    if(User::getUser()->getPer() < Permission::USER) {
        return "权限不足，无法删除字段";
    }

    for(auto &t : tables) {
        if(tableName.compare(t->getName()) == 0) {
            return t->deleteRecord(boolStats);
        }
    }
    return "未找到表";
}

QVector<QVector<QString>> DB::select(bool isAll, const QVector<QString>& column_names,
                                     const QVector<QString>& tableName, QVector<BoolStat*> boolStats) {
    Table *table_tem = new Table();
    for(auto &t : tableName) {
        bool isFound = false;
        for(auto &t2 : tables) {
            if(t.compare(t2->getName()) == 0) {
                isFound = true;
                table_tem = table_tem->operator+(t2);
                break;
            }
        }
        if(!isFound) {
            for(auto &v : views) {
                if(t.compare(v->getName()) == 0) {
                    isFound = true;
                    table_tem = table_tem->operator+(viewToTable(v));
                    break;
                }
            }
            if(!isFound) {
                QVector<QVector<QString>> res;
                return res;
            }
        }
    }
    return table_tem->select(isAll, column_names, boolStats);
}

QString DB::createView(QString viewName, bool isAll, const QVector<QString>& column_names,
                       const QVector<QString>& tableName, QVector<BoolStat*> boolStats) {

    QDir path(_filePath);

    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法删除字段";
    }

    //创建视图列名记录文件
    QFile covFile(path.absoluteFilePath(viewName + ".cov"));
    if(!covFile.open(QIODevice::WriteOnly)) { return "视图列记录文件创建失败"; };
    covFile.close();

    //创建视图表名记录文件
    QFile tovFile(path.absoluteFilePath(viewName + ".tov"));
    if(!tovFile.open(QIODevice::WriteOnly)) { return "视图表名记录文件创建失败"; };
    tovFile.close();

    //创建视图判断信息存储文件
    QFile bovFile(path.absoluteFilePath(viewName + ".bov"));
    if(!bovFile.open(QIODevice::WriteOnly)) { return "视图判断信息存储文件创建失败"; };
    bovFile.close();

    //创建视图类
    View *view = new View(viewName, isAll, column_names, tableName, boolStats,
                          path.absoluteFilePath(viewName + ".cov"),
                          path.absoluteFilePath(viewName + ".tov"),
                          path.absoluteFilePath(viewName + ".bov"));
    views.append(view);
    writeView(view, path.absoluteFilePath(QString(_name) + ".viw"));

    return "视图创建成功";
}

bool DB::writeView(View* v, QString filePath) {
    char buf[VIEWBYTE + 128];
    int cnt = v->serialize(buf);

    //创建文件操作对象
    QFile viewFile(filePath);
    if(!viewFile.open(QIODevice::Append)) { return false; }//判断文件是否存在
    QDataStream dbOut(&viewFile);
    dbOut.writeRawData(buf, cnt);

    return true;
}

bool DB::readViews(QString filePath) {

    views.clear();
    //创建文件操作对象
    QFile viewFile(filePath);
    if(!viewFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream viewOut(&viewFile);

    //循环读入
    char buf[VIEWBYTE + 128];
    while(!viewOut.atEnd()) {
        viewOut.readRawData(buf, VIEWBYTE);
        View *v = new View();
        v->deSerialize(buf);
        views.push_back(v);
    }

    return true;
}

bool DB::writeViews(QString filePath) {

    char buf[VIEWBYTE + 128];
    int cnt;

    //创建文件操作对象
    QFile viewFile(filePath);
    if(!viewFile.open(QIODevice::WriteOnly)) { return false; }//判断文件是否存在
    QDataStream viewOut(&viewFile);

    //循环写入数据
    for(auto &v : views) {
        cnt = v->serialize(buf);
        viewOut.writeRawData(buf, cnt);
    }

    return true;
}

Table* DB::viewToTable(View *view) {
    Table *newTable = new Table(view->_name);
    QVector<QVector<QString>> values = select(view->_isAll, view->columnNames, view->tableNames, view->boolStats);

    if(view->_isAll) {
        QVector<QString> columnNameAll;
        for(auto &tn : view->tableNames) {
            for(auto &t : tables) {
                if(t->_name == tn) {
                    for(auto &c : t->columns) {
                        newTable->addColumn(c->getName(), c->getType(), c->getTypeLen(), c->getIntegrities());
                        columnNameAll.append(c->getName());
                    }
                }
            }
        }
        //添加记录
        for(int i = 1; i < values.size(); i++) {
            newTable->insertRecord(columnNameAll, values[i]);
        }
    } else {
        //添加列
        for(auto &t : tables) {
            for(auto &tn : view->tableNames) {
                if(t->_name == tn) {
                    for(auto &cn : view->columnNames) {
                        if(cn.contains('.')) {
                            if(cn.split('.')[0] == t->getName()) {
                                for(auto & c : t->columns) {
                                    if(c->getName() == cn.split('.')[1]) {
                                        newTable->addColumn(cn, c->getType(), c->getTypeLen(), c->getIntegrities());
                                    }
                                }
                            }
                        } else {
                            for(auto & c : t->columns) {
                                if(c->getName() == cn) {
                                    newTable->addColumn(cn, c->getType(), c->getTypeLen(), c->getIntegrities());
                                }
                            }
                        }
                    }
                }
            }
        }
        //添加记录
        for(int i = 1; i < values.size(); i++) {
            newTable->insertRecord(view->columnNames, values[i]);
        }
    }
    return newTable;
}

QString DB::commit() {
    for(auto &t : tables) {
        t->commit();
    }
    return "提交成功";
}

QString DB::addPK(QString tableName, QString columnName) {
    //判断用户权限
    if(User::getUser()->getPer() < Permission::AD) {
        return "权限不足，无法修改表属性";
    }

    for(auto &t : tables) {
        if(t->getName() == tableName) {
            return t->addIntegrity(QString("PK_") + columnName, columnName, ITGTYPE::PRIMARYKEY, "");
        }
    }
    return "未找到表";
}
