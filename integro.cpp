#include <stdlib.h>
#include <QProcess>
#include <dirent.h>
#include <QFile>
#include <QTextStream>
#include <QByteArray>

#include <iostream>
using namespace std;

QString GetNautilusVer()
{
    QProcess proc;
    proc.start("nautilus --version");
    proc.waitForFinished(-1);
    QByteArray btar;
    btar = proc.readAll();
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

bool nautilus_check()   // Nautilus
{
    char *home;
    QString fold;
    home=getenv("HOME");
    QString ver = GetNautilusVer();
    fold.append(home);
    if (ver.contains("nautilus 3", Qt::CaseInsensitive))
            fold.append("/.local/share/nautilus/scripts");
    else
            fold.append("/.gnome2/nautilus-scripts");
    fold.append("/Print(vap)");
    QFile file(fold);
    return file.exists();
}

void nautilus_set(bool ch) // to Nautilus
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
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
    fold.append("/Print(vap)");
    QFile file(fold);
    if (!ch)
        {
            file.remove();
            cout << "integration into the Nautilus is disabled" << endl;
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
    cout << "integration into Nautilus enabled" << endl;
}


void dolphin_set(bool ch)
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.kde/share/kde4/services/ServiceMenus");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append("/Print.desktop");
    QFile file(fold);
    if (!ch)
        {
            file.remove();
            cout << "integration into the Dolphin is disabled" << endl;
            return;
        }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Dolphin" << endl;
            return;
        }
    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Actions=vap\n";
    out << "Icon=vap\n";
    out << "ServiceTypes=KonqPopupMenu/Plugin,all/allfiles\n";
    out << "Type=Service\n";
    out << "X-KDE-Priority=TopLevel\n";
    out << "\n";
    out << "[Desktop Action vap]\n";
    out << "Exec=vap %F\n";
    out << "Icon=vap\n";
    out << "MimeType=image\n";
    out << "Name=Print(vap)\n";
    file.close();
    com.clear();
    com.append("chmod 777 ");
    com.append(fold);
    run(com);
    cout << "integration into Dolphin enabled" << endl;
}

bool dolphin_check()
{
    char *home;
    QString fold;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.kde/share/kde4/services/ServiceMenus");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) return false;  // папки нет, интеграции тоже
    fold.append("/Print.desktop");
    QFile file(fold);
    return file.exists();
}

void marlin_set(bool ch)
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.local/share/extended-actions");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append("/vap.desktop");
    QFile file(fold);
    if (!ch)
        {
            file.remove();
            cout << "integration into the Marlin is disabled" << endl;
            return;
        }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Marlin" << endl;
            return;
        }
    QTextStream out(&file);
    out << "[Extended Action Entry]\n";
    out << "MimeType=image\n";
    out << "Name=vap\n";
    out << "Exec=vap %F\n";
    out << "Icon=vap\n";
    out << "Comment=Fast printig pictures\n";
    out << "Comment[ru]=Быстрая печать миниатюр изображений\n";
    out << "Description=Print(vap)\n";
    file.close();
    com.clear();
    com.append("chmod 777 ");
    com.append(fold);
    run(com);
    cout << "integration into Marlin enabled" << endl;
}

bool marlin_check()
{
    char *home;
    QString fold;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.local/share/extended-actions");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) return false;  // папки нет, интеграции тоже
    fold.append("/vap.desktop");
    QFile file(fold);
    return file.exists();
}

void nemo_set(bool ch)  // to Nemo
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.gnome2/nemo-scripts");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append("/Print(vap)");
    QFile file(fold);
    if (!ch)
        {
            file.remove();
            cout << "integration into the Nemo is disabled" << endl;
            return;
        }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Nemo" << endl;
            return;
        }
    QTextStream out(&file);
    out << "#!/bin/sh\n";
    out << "\n";
    out << "/usr/bin/vap $NEMO_SCRIPT_SELECTED_FILE_PATHS\n";
    file.close();
    com.clear();
    com.append("chmod 777 ");
    com.append(fold);
    run(com);
    cout << "integration into Nemo enabled" << endl;
}

bool nemo_check()
{
    char *home;
    QString fold;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.gnome2/nemo-scripts");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) return false;  // папки нет, интеграции тоже
    fold.append("/Print(vap)");
    QFile file(fold);
    return file.exists();
}


void gnome_set(bool ch)
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.gnome-commander");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append("/fav-apps");
    QFile file;
    if (ch)
    {
        file.setFileName(fold);
        file.open(QIODevice::Append | QIODevice::Text);
        if(!file.isOpen())
            {
                cout << "error open file for Gnome Commander" << endl;
                return;
            }
        QTextStream out(&file);
        out << "%D0%9F%D0%B5%D1%87%D0%B0%D1%82%D1%8C(vap)	%2Fusr%2Fbin%2Fvap	%2Fusr%2Fshare%2Fpixmaps%2Fvap64.png	3	*.jpg%3B*.png%3B*.ico%3B*.bmp%3B*.jpeg%3B*.gif	0	0	0\n";
        file.close();
        cout << "integration into Gnome-Commander enabled" << endl;
    }
    else
    {
        QFile file2;
        QString fname2;
        QString line;
        fname2.append(fold);
        fname2.append("2");
        file.setFileName(fold);
        file2.setFileName(fname2);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        file2.open(QIODevice::WriteOnly | QIODevice::Text);
        if(!file.isOpen())
            {
                cout << "error open file for Gnome Commander (could not open setting file)" << endl;
                return;
            }
        if(!file2.isOpen())
            {
                cout << "error open file for Gnome Commander (could not open temp file)" << endl;
                return;
            }
        QTextStream out(&file2);
        QByteArray lin;
        while(!file.atEnd())
        {
            lin=file.readLine();
            line=QString(lin);
            if(!line.contains("%2Fusr%2Fbin%2Fvap"))
            {
                out << line;
                out << "\n";
            }
        }
        file.close();
        file2.close();
        file.remove();
        QFile::rename(fname2, fold);
        cout << "integration into the Gnome Commander is disabled" << endl;
     }
    com.clear();
    com.append("chmod 777 ");
    com.append(fold);
    run(com);
}

bool gnome_check()
{
    char *home;
    QString fold;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.gnome-commander");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) return false;
    fold.append("/fav-apps");
    QFile file;
    file.setFileName(fold);
    QString line;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Gnome Commander (get check info)" << endl;
            return false;
        }
    QByteArray lin;
    while(!file.atEnd())
    {
        lin=file.readLine();
        line=QString(lin);
        if(line.contains("%2Fusr%2Fbin%2Fvap")) return true;
    }
    return false;
}

bool caja_check()   // Caja
{
    char *home;
    QString fold;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.config/caja/scripts/Print(vap)");
    QFile file(fold);
    return file.exists();
}

void caja_set(bool ch) // to Caja
{
    char *home;
    QString fold;
    QString com;
    home=getenv("HOME");
    fold.append(home);
    fold.append("/.config/caja/scripts");
    DIR *dr = opendir(fold.toUtf8());
    if (!dr) // папки нет, надо создать
        {
            com.clear();
            com.append("mkdir ");
            com.append(fold);
            run(com);
        }
    fold.append("/Print(vap)");
    QFile file(fold);
    if (!ch)
        {
            file.remove();
            cout << "integration into the Caja is disabled" << endl;
            return;
        }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        {
            cout << "error open file for Caja" << endl;
            cout << fold.toStdString() << endl;
            cout << "Probably you are not the owner of the destination folder. Try sudo vap" << endl;
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
    cout << "integration into Caja enabled" << endl;
}
