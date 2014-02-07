#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
//#include <QWidget>
#include <QLocale>
#include <QSettings>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{

    cout << "Hello, World!" << endl;
    cout << "I'm vap! If you want to contact me at viktand@bk.ru" << endl;

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); //изменения
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); //изменения

    QApplication a(argc, argv);
    QLocale lc;
    QTranslator translator;
    cout << "Locale " << QLocale::countryToString(lc.country()).toStdString() << endl;
    if(lc.country()==QLocale::RussianFederation)
        {
            if(translator.load(":/new/prefix1/vap_ru"))
              {
                a.installTranslator(&translator);
                cout << "Russian interface selected and connected" << endl;
              }
        }
    MainWindow w;
    QFont font("Ubuntu");
    int fSize;   // font size
    QSettings settu("vap", "vap");
    settu.beginGroup("Settings");
    fSize=settu.value("font",7).toInt();
    settu.endGroup();
    font.setPointSize(fSize);
    w.setFont(font);
    w.cou_prm=argc;
    w.fn_size=fSize;
    if (argc>1)
    for (int i = 0; i < argc; i++) {
            // Сохраняем список аргументов
            w.prm<<argv[i];
        }
    w.show();
    w.if_show();

    return a.exec();
}

