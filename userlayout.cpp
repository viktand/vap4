#include "userlayout.h"
#include "ui_userlayout.h"
#include "mainwindow.h"
#include <math.h>

int cou=9;                  // текущее кол-во превьюшек

struct prw {
    QLabel     *pct;        // картинка
};

std::vector<prw> prv;       // массив превьющек
bool ex=false;              // наличие картинки

userlayout::userlayout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userlayout)
{
    ui->setupUi(this);
    ui->spinBox->setValue(ul_hor);
    ui->spinBox_2->setValue(ul_ver);
}

userlayout::~userlayout()
{
    delete ui;
}

void userlayout::on_buttonBox_accepted() // Ok
{
    ul_hor=ui->spinBox->value();
    ul_ver=ui->spinBox_2->value();
    emit is_ok();
}

void userlayout::make_preview()
{
    kill_prv();
    QSize sz=set_size();
    QImage im(":/new/prefix1/nina");
    int x, y, x1, y1;
    float w = ui->label->width();
    float h = ui->label->height();
    x1=(w/ul_hor-sz.width())/2;
    y1=(h/ul_ver-sz.height())/2;
    int k=0; // просто для удобства
    ex=true;
    for(int i=0; i<ul_ver; i++)
        for(int j=0; j<ul_hor; j++)
        {
            k=i*ul_hor+j;
            // создать картинку
            prv.push_back(prw());
            prv[k].pct = new QLabel(ui->label);
            prv[k].pct->setBackgroundRole(QPalette::Base);
            prv[k].pct->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            prv[k].pct->setScaledContents(true);
            prv[k].pct->setPixmap(QPixmap::fromImage(im));
            //prv[k].pct->setText(QString::number(k));  // для отладки
            prv[k].pct->setAttribute(Qt::WA_DeleteOnClose);
            // установить размер и позицию картинки
            x=(k%ul_hor)*(w/ul_hor)+x1;
            y=ceil(k/ul_hor)*(h/ul_ver)+y1;
            prv[k].pct->setGeometry(x, y, sz.width(), sz.height());
            // показать
            prv[k].pct->show();
        }
    cou=ul_hor*ul_ver;
}

void userlayout::kill_prv()
{
    if (not ex) return;
    for(int i=0; i<cou; i++)
        prv[i].pct->deleteLater();
}

QSize userlayout::set_size()    // расчет размеров превьюшки
{
   QSize z;
   const float k = 1.4285714;   // отношение высоты к длине
   float h, w, h1, w1;
   w1=ui->label->width()/ul_hor;
   h1=ui->label->height()/ul_ver;
   h=h1;
   w=w1;
   if (w1*k>h1)   w=h1/k;
   else           h=w1*k;
   z.setWidth(w);
   z.setHeight(h);
   return z;
}


void userlayout::on_spinBox_valueChanged(int arg1)
{
    ul_hor=arg1;
    make_preview();
}

void userlayout::on_spinBox_2_valueChanged(int arg1)
{
    ul_ver=arg1;
    make_preview();
}

