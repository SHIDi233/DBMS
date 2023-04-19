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
    index.cpp \
    integrity.cpp \
    main.cpp \
    mainwindow.cpp \
    row.cpp \
    sqlanalysis.cpp \
    table.cpp \
    tableinfo.cpp \
    user.cpp \
    waittodo.cpp

HEADERS += \
    Control.h \
    basic_data.h \
    boolstat.h \
    column.h \
    db.h \
    file.h \
    index.h \
    integrity.h \
    mainwindow.h \
    row.h \
    sqlanalysis.h \
    table.h \
    tableinfo.h \
    user.h \
    waittodo.h

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

RESOURCES += \
    pic.qrc
