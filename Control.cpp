#include "Control.h"

bool readUsers() {
    QFile dbFile(Path.absoluteFilePath("ruanko.usr"));
    if(!dbFile.exists()) {
        auto user = User::getUser();
        users.append(user);
        writeUsers();
        return true;
    }
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
//    rootPath.cd("..");
    char buf[USERBYTE + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, USERBYTE);
        User *u = new User();
        u->deSerialize(buf);
//        rootPath.cd("..");
        users.push_back(u);
    }

    dbFile.close();
    User::getUser()->loadDB();
    return true;
}

bool writeUsers() {
    QFile dbFile(Path.absoluteFilePath("ruanko.usr"));
    if(!dbFile.open(QIODevice::WriteOnly)) { return false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[USERBYTE + 128];
    for(auto &u : users) {
        u->serialize(buf);
        dbOut.writeRawData(buf, USERBYTE);
    }
    dbFile.close();

    return true;
}

bool login(QString name, QString pwd) {
    for(auto &u : users) {
        if(u->log(name, pwd)) {
            User::getUser()->set(u);
            return true;
        }
    }
    return false;
}

QString grantTo(QString name, Permission p) {

    for(auto &u : users) {
        if(name == u->getName()) {
            u->setPer(p);
            writeUsers();
            return "权限赋予成功";
        }
    }
    return "未找到用户";
}
