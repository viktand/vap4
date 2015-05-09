#include "mainwindow.h"
#include <QApplication>
#ifdef HAVE_QT4
    #include <QTextCodec>
#endif
#include <QLocale>
#include <QSettings>
#include <QStyleFactory>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{

    cout << "Hello, World!" << endl;
    cout << "I'm vap! If you want to contact me at viktand@bk.ru" << endl;


    #ifdef HAVE_QT4
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    }
    #endif


    QSettings settu("vap", "vap");
    settu.beginGroup("Settings");
    int fSize=settu.value("font",0).toInt(); // font size
    QString lng_app=settu.value("lng", "Auto").toString(); // язык программы
    settu.endGroup();
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("gtk"));
    QLocale lc;
    QTranslator translator;
    cout << "Locale " << QLocale::countryToString(lc.country()).toStdString() << endl;
    if(((lc.country()==QLocale::RussianFederation) && (lng_app=="Auto"))
            || (lng_app=="Russian") || (lng_app=="Русский") )
        {
            if(translator.load(":/new/prefix1/vap_ru"))
              {
                a.installTranslator(&translator);
                cout << "Russian interface selected and connected" << endl;
              }
        }
    if(((lc.country()==QLocale::Ukraine) && (lng_app=="Auto"))
            || (lng_app=="Ukraine") || (lng_app=="Українська") )
        {
            if(translator.load(":/new/prefix1/vap_uk"))
              {
                a.installTranslator(&translator);
                cout << "Ukrainian interface selected and connected" << endl;
              }
        }
    MainWindow w;
    QFont font;
    if(fSize==0){
        font.setFamily(font.defaultFamily());
        fSize=font.family().size();
    } else {
        font.setFamily("Ubuntu");
        font.setPointSize(fSize);
    }
    w.setFont(font);
    w.cou_prm=argc;
    w.fn_size=fSize;
    bool go=true;
    if (argc>1){
        for (int i = 0; i < argc; i++) {
                // Сохраняем список аргументов
                w.prm<<argv[i];
            }
        if(w.prm[1]=="--version"){
                cout << "vap 3.8.3 Qt 5.4" << endl;
                cout << "Exit after the request version" << endl;
                go=false;

        }
        if(w.prm[1]=="--about"){
                cout << "vap - fast print" << endl;
                cout << "visit qvap.ru" << endl;
                cout << "Exit after the request about" << endl;
                go=false;

        }
    }
    if(go){
        w.show();
        w.if_show();
        return a.exec();
    }
}

