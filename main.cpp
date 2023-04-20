﻿#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "login.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DBMS_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
//    MainWindow w;
//    w.show();
    Login l;
    l.show();
    return a.exec();
}
