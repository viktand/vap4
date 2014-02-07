#include "openfiledialog.h"
#include "ui_openfiledialog.h"
#include "avLabel.h"

#include <QDir>
#include <QVector>
#include <QLabel>

#include <iostream>
using namespace std;

struct prw {
    QavLabel     *pct;        //картинка
    QString       path;       //путь
};

std::vector<prw> previwes;

OpenFileDialog::OpenFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenFileDialog)
{
    ui->setupUi(this);
    load_files("/home/and/Изображения");
}

OpenFileDialog::~OpenFileDialog()
{
    delete ui;
}

void OpenFileDialog::load_files(QString path)
{
    QDir dir(path);
    QStringList fls = dir.entryList(QStringList("*.*"));
    int i=0;
    foreach (QString file, fls)
    {
        if(file!="." && file!=".." && file!="...")
        {
            previwes.push_back(prw());
            //QavLabel::se;
            previwes[i].pct= new QavLabel(ui->scrollArea);
            QImage im(path+"/"+file); //(":/new/prefix1/text");
            previwes[i].pct->setPixmap(QPixmap::fromImage(im));
            previwes[i].pct->setGeometry(10, 10+i*70, 64, 64);
            previwes[i].pct->setScaledContents(true);
            cout << file.toStdString() << endl;
            i++;
        }
    }

}
