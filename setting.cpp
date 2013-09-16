#include "setting.h"
#include <QDesktopWidget>
#include <mainwindow.h>
#include <stdlib.h>
#include <QProcess>
#include <QDebug>
#include <dirent.h>
#include <QFile>
#include <QTextStream>

#include <iostream>
using namespace std;

bool flag;

setting::setting(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    flag=false;
    check_intg();
}

void setting::check_intg() // проверка интеграции
{
    // Nautilus
    char *home;
    QString fold;

    home=getenv("HOME");
    QString ver = GetNautilusVer();
    fold.append(home);
    if (ver.contains("nautilus 3", Qt::CaseInsensitive))
            fold.append("/.local/share/nautilus/scripts");
    else
            fold.append("/.gnome2/nautilus-scripts");
    fold.append(tr("/Print"));
    QFile file(fold);
    checkBox->setChecked(file.exists());

    flag=true;
}

QString setting::GetNautilusVer()
{
    QProcess proc;
    proc.start("nautilus --version");
    proc.waitForFinished(-1);
    QByteArray btar;
    btar = proc.readAll();
    if (rap) qDebug() << "found: " << btar << endl;
    QString ver;
    ver.append(btar);
    return ver;
}

void run(QString s) // выполнить команду в командной строке
{
   QProcess proc;
   proc.start(s);
   proc.waitForFinished(-1);
}

void setting::on_checkBox_clicked(bool checked) // to Nautilus
{
    if(! flag) return;
    char *home;
    QString fold;
    QString com;

    home=getenv("HOME");
    if (rap) cout << "home: " << home << endl;
    QString ver = GetNautilusVer();
    fold.append(home);
    if (ver.contains("nautilus 3", Qt::CaseInsensitive))
            fold.append("/.local/share/nautilus/scripts");
    else
            fold.append("/.gnome2/nautilus-scripts");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append(tr("/Print"));
    QFile file(fold);
    if (!checked)
        {
            file.remove();
            if (rap) cout << "integration into the Nautilus is disabled" << endl;
            return;
        }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Nautilus" << endl;
            return;
        }
    QTextStream out(&file);
    out << "#!/bin/sh";
    out << "\n";
    out << "/usr/bin/vap $NAUTILUS_SCRIPT_SELECTED_FILE_PATHS";
    file.close();
    com.clear();
    com.append("chmod 777 ");
    com.append(fold);
    run(com);
    if (rap) cout << "integration into Nautilus enabled" << endl;
}
