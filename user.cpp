#include "user.h"
#include "Control.h"
#include "db.h"
#include "QDateTime"
#include "QDataStream"

User* User::user_one1 = nullptr;

User::User(QString name, QString pwd)
{
    _permission = Permission::DBA;
    strcpy(_name, name.toLatin1().data());
    strcpy(_pwd, pwd.toLatin1().data());
}

QVector<DB*>& User::getDbs() {
    return dbs;
}

QString User::createDb(QString name)
{
    //判断用户权限
    if(_permission != Permission::DBA) {
        return "权限不足，无法创建数据库";
    }

    if(name.size() > 127) { return "数据库名过长"; }//判断名字长度是否符合要求

    //判断数据库是否已经存在
    for(auto &d : dbs) {
        if(name == d->getName()) {
            return "数据库已存在";
        }
    }

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //创建数据库路径
    QDir path(Path);
    if(!path.cd("data")) {
        path.mkdir("data");
        path.cd("data");
    }
    if(!path.cd(name)) {
        path.mkdir(name);
        path.cd(name);
    }
    DB *database = new DB(name, false, path.absolutePath(), current_date);

    // 创建表描述文件
    QFile tbFile(path.absoluteFilePath(name + ".tb"));
    if(!tbFile.open(QIODevice::WriteOnly)) { return "表描述文件创建失败"; };
    tbFile.close();

    // 创建表描述文件
    QFile viwFile(path.absoluteFilePath(name + ".viw"));
    if(!viwFile.open(QIODevice::WriteOnly)) { return "视图描述文件创建失败"; };
    viwFile.close();

    // 创建日志文件
    QFile logFile(path.absoluteFilePath(name + ".log"));
    if(!logFile.open(QIODevice::WriteOnly)) { return "日志文件创建失败"; };
    logFile.close();

    //写入文件
    QFile dbFile(rootPath.absoluteFilePath("ruanko.db"));
    dbFile.open(QIODevice::Append);
    QDataStream dbOut(&dbFile);
    char buf[DBBYTE];
    int len = database->serialize(buf);
    dbOut.writeRawData(buf, len);

    dbs.push_back(database);
    return "数据库创建成功";
}

QString User::dropDB(QString name) {
    //判断用户权限
    if(_permission != Permission::DBA) {
        return "权限不足，无法删除数据库";
    }

    //寻找数据库
    bool found = false;
    for(int i = 0; i < dbs.size(); i++) {
        if(dbs[i]->getName().compare(name) == 0) {
            dbs.remove(i);
            found = true;
        }
    }
    if(!found) { return "未找到数据库"; }

    //重新写入文件
    QFile dbFile(rootPath.absoluteFilePath("ruanko.db"));
    dbFile.open(QIODevice::WriteOnly);
    QDataStream dbOut(&dbFile);
    char buf[DBBYTE];
    for(auto &d : dbs) {
        int len = d->serialize(buf);
        dbOut.writeRawData(buf, len);
    }
    return "数据库删除成功";
}

DB* User::getDB(QString name) {
    for(auto &d : dbs) {
        if(name.compare(d->getName()) == 0) {
            return d;
        }
    }
    return nullptr;
}

bool User::loadDB() {

    dbs.clear();
    //加载数据库
    //创建文件操作对象
    bool isRead = true;
    QFile dbFile(rootPath.absoluteFilePath("ruanko.db"));
    if(!dbFile.open(QIODevice::ReadOnly)) { isRead = false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[DBBYTE];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, DBBYTE);
        DB *d = new DB();
        d->deSerialize(buf);
        dbs.append(d);
    }

    return isRead;
}

Permission User::getPer() {
    return _permission;
}

int User::serialize(char buf[]) {
    int offset = 0;
    memcpy(buf + offset, _name, 128);
    offset += 128;
    memcpy(buf + offset, _pwd, 128);
    offset += 128;
    memcpy(buf + offset, &_permission, 4);
    offset += 4;

    return offset;
}

int User::deSerialize(char buf[]) {
    int offset = 0;
    memcpy(_name, buf + offset, 128);
    offset += 128;
    memcpy(_pwd, buf + offset, 128);
    offset += 128;
    memcpy(&_permission, buf + offset, 4);
    offset += 4;

//    rootPath.cd(_name);
//    loadDB();

    return offset;
}

QString User::createUser(QString name, QString pwd) {
    User* newUser = new User(name, pwd);
    newUser->_permission = Permission::VISITOR;

    //写入文件
    QFile dbFile(Path.absoluteFilePath("ruanko.usr"));
    dbFile.open(QIODevice::Append);
    QDataStream dbOut(&dbFile);
    char buf[USERBYTE];
    int len = newUser->serialize(buf);
    dbOut.writeRawData(buf, len);

//    Path.mkdir(name);
    users.push_back(newUser);
//    newUser->createDb("root");

    return "创建用户成功";
}

QString User::grant(QString name, Permission p) {
    //判断权限
    if(_permission < Permission::DBA) {
        return "权限不足，无法赋予权限";
    }
    return grantTo(name, p);
}

QString User::getName() {
    return _name;
}

bool User::log(QString name, QString pwd) {
    if(name == _name && pwd == _pwd) {
        return true;
    }
    return false;
}
