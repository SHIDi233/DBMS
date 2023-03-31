QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basic_data.cpp \
    boolstat.cpp \
    column.cpp \
    db.cpp \
    file.cpp \
    main.cpp \
    mainwindow.cpp \
    table.cpp \
    tableinfo.cpp

HEADERS += \
    basic_data.h \
    boolstat.h \
    column.h \
    db.h \
    file.h \
    mainwindow.h \
    table.h \
    tableinfo.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    DBMS_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
