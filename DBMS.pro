QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Control.cpp \
    about.cpp \
    basic_data.cpp \
    boolstat.cpp \
    btree.cpp \
    ccnbutton.cpp \
    ccnwindow.cpp \
    client.cpp \
    column.cpp \
    db.cpp \
    file.cpp \
    highlighter.cpp \
    index.cpp \
    integrity.cpp \
    log.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    newserver.cpp \
    row.cpp \
    server.cpp \
    sqlanalysis.cpp \
    sqldebug.cpp \
    table.cpp \
    tableinfo.cpp \
    user.cpp \
    view.cpp \
    waittodo.cpp \
    wktcpserver.cpp

HEADERS += \
    Control.h \
    about.h \
    basic_data.h \
    boolstat.h \
    btree.h \
    ccnbutton.h \
    ccnwindow.h \
    client.h \
    column.h \
    db.h \
    file.h \
    highlighter.h \
    index.h \
    integrity.h \
    log.h \
    login.h \
    mainwindow.h \
    newserver.h \
    row.h \
    server.h \
    sqlanalysis.h \
    sqldebug.h \
    table.h \
    tableinfo.h \
    user.h \
    view.h \
    waittodo.h \
    wktcpserver.h

FORMS += \
    about.ui \
    ccnwindow.ui \
    login.ui \
    mainwindow.ui

TRANSLATIONS += \
    DBMS_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
