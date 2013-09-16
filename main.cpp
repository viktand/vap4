#include "mainwindow.h"
#include <QApplication>

#include <iostream> // временно !!!
using namespace std; // временно !!!


int main(int argc, char *argv[])
{
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
    w.on_show();
    return a.exec();

}
