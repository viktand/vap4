#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include <iostream>
using namespace std;



int main(int argc, char *argv[])
{

    cout << "Hello, World!" << endl;
    cout << "I'm vap! If you want to contact me at viktand@bk.ru" << endl;


    QApplication a(argc, argv);
    MainWindow w;
    w.cou_prm=argc;
    if (argc>1)
    for (int i = 0; i < argc; i++) {
            // Выводим список аргументов в цикле
            cout << "Argument " << i << " : " << argv[i] << endl;
            w.prm<<argv[i];
        }
    w.show();
    w.if_show();

    return a.exec();
}

// 1. все строки пишем по английски, и заключаем их в tr()
// 2. в *.pro файле пишем TRANSLATIONS = прога_ru.ts
// 3. запускаем lupdate
// 4. переводим в Qt Translator'е
// 5. запускаем lrelease
// 6. копируем из документации кусок кода (4 строки) подключения файла с переводами.
