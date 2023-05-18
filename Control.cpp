#include "Control.h"

bool readUsers() {
    QFile dbFile(Path.absoluteFilePath("ruanko.usr"));
    if(!dbFile.open(QIODevice::ReadOnly)) { return false; }
    QDataStream dbOut(&dbFile);

    //循环将表信息读入列表中
    char buf[USERBYTE + 128];
    while(!dbOut.atEnd()) {
        dbOut.readRawData(buf, USERBYTE);
        User *u = new User();
        u->deSerialize(buf);
        users.push_back(u);
    }

    dbFile.close();

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