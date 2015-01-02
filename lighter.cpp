#include "lighter.h"
#include "ui_lighter.h"
#include "avLabel.h"
#include <QPainter>
#include <QVector>
#include <math.h>

#include <iostream>
using namespace std;

struct pixel{  //пиксель без ограничений по разбросу значений
    int h;     // тон
    int s;     // насыщенность
    int v;     // яркость
    int al;    // альфа
};

struct ray{         // луч
    QPoint p1;      // лампа
    QPoint p2;      // сторона 1
    QPoint p3;      // сторона 2
    QPoint pp2;     // переceчение  стороны 1 с картинкой
    QPoint pp3;     // переceчение  стороны 2 с картинкой
    int    value;   // яркость
};

struct flash{
    QPoint  p1;     // центр вспышки
    int     pwr;    // сила вспышки
    int     lng;    // размах (радиус)
};

struct bolls{
    QPoint  lmp;    // лампа
    QPoint  cen;    // центр шара
    int     brt;    // яркость
    int     rds;    // радиус
};

QavLabel *lbl;          // картинка - предпросмотр
QavLabel *ecr;          // прозрачный лабел над картикой для отслеживания мышки
QPixmap  *tmPix;        // рабочий pixmap
QImage   *img;          // рабочий image
QColor   clr2;
QRgb     rgb2;
QPoint   pl1,pls1,pls2; // точки направления вектора луча света - вершины треугольника
QPoint   pl3,pl2;       // точки пересечения сектора луча света с дальним краем картинки (где выходит луч)
int      alf=20;        // угол (ширина) луча света
int      spoint=0;      // указатель на вводимую вершину треугольника 0-1-2
QavLabel *v1,*v2,*v3;   // картинки - точки вершин треугольника луча света
QPoint   plp1,plp2,plp3;// вершины треугольника луча света в координатах картинки
QPoint   pp2,pp3;       // точки пересечения сторон треугольника и краев картинки
bool     lampPos;       // положение лампы от основания треугольника
bool     retFlag=false; // флаг
bool     pr,pf,pb;      // флаги несохраненных луча, вспышки и шара

QPoint   fpn,fpn1;      // точка (центр) вспышки в координатах экрана и картинки

std::vector <std::vector<pixel> > tempPicture2;  // рабочий массив для преобразования картинки
std::vector <std::vector<pixel> > tempPicture3;  // массив для шага назад
std::vector <ray> rays;                          // массив, хранящий все наложенные на изображение лучи
std::vector <flash> flsh;                        // массив, хранящий вспышки
std::vector <bolls> bls;                         // массив, хранящий шары


lighter::lighter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lighter)
{
    ui->setupUi(this);
    if(lbl==0){
        lbl=new QavLabel(this);
        lbl->show();
        tmPix=new QPixmap(1,1);
        img=new QImage(tmPix->toImage());
        ecr = new QavLabel(this);
        ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
        ecr->setGeometry(QRect(0,0,520,420));
        ecr->show();
        connect(ecr,SIGNAL(mouse_up(int,int,int)),this,SLOT(setPoint(int,int)));
    }
    retFlag=true;
    ui->tabWidget->setCurrentIndex(0);
    retFlag=false;
}

lighter::~lighter()
{
    delete ui;
}

void lighter::loadPicture()
{
    resetSliders();
    double ratio=double(pix.width())/double(pix.height());
    if(ratio>1)lbl->setGeometry(QRect(49,25+(400-400/ratio)/2,400,400/ratio));
    else lbl->setGeometry(QRect(25+(400-400*ratio)/2,49,400*ratio,400));
    *tmPix=getPixOfSize(pix,lbl->geometry().size());
    lbl->setPixmap(*tmPix);
    loadInVector();
    reset();
    rays.resize(0);
    flsh.resize(0);
    bls.resize(0);
    ui->tabWidget->setCurrentIndex(0);
    pr=pf=pb=false;
    ui->pushButton_2->setEnabled(false);
}

void lighter::resetSliders()
{
    retFlag=true;
    ui->horizontalSlider->setValue(128);
    ui->horizontalSlider_2->setValue(128);
    ui->horizontalSlider_3->setValue(128);
    ui->horizontalSlider_4->setValue(128);
    ui->horizontalSlider_5->setValue(128);
    retFlag=false;
}

void lighter::reset()
{
    if(v1!=0)v1->hide();
    if(v2!=0)v2->hide();
    if(v3!=0)v3->hide();
    if(spoint<8)spoint=0;
    if(spoint>7 && spoint<11)spoint=8;
    if(spoint>10)spoint=11;
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
}

QPixmap  lighter::getPixOfSize(QPixmap p, QSize sz)
{
    QPixmap canv_pix(sz);                       // промежуточный pixmap для рисования в нем
    canv_pix.fill(Qt::transparent);             // залить пустотой
    QPainter pntr(&canv_pix);                   // живописец, привязанный к pixmap
    pntr.drawPixmap(QRect(QPoint(0,0),sz),p);   // нарисовать
    pntr.end();                                 // завершить работу живописца
    return canv_pix;
}

void lighter::loadInVector()  // загрузить картинку в вектор для быстрой обработки
{
    QSize sz;
    sz.setWidth(lbl->geometry().width());
    sz.setHeight(lbl->geometry().height());
    *img=tmPix->toImage();
    tempPicture2.resize(tmPix->width());
    for(int x=0;x<tmPix->width();x++)tempPicture2[x].resize(tmPix->height());
    load();
}

void lighter::load()
{
    int h=0,s=0,v=0,a=0;
    int x1=tmPix->width();
    int y1=tmPix->height();
    for(int x=0;x<x1;x++)
        for(int y=0;y<y1;y++){
            rgb2=img->pixel(x,y);
            clr2.setRgba(rgb2);
            clr2.getHsv(&h,&s,&v,&a);
            tempPicture2[x][y].h=h;
            tempPicture2[x][y].s=s;
            tempPicture2[x][y].v=v;
            tempPicture2[x][y].al=a;
        }
}

void lighter::setPoint(int x, int y) // обработка нажатия на картинку
{
    int dx,dy;
    switch (spoint++) {
    case 0:
        get1point(x,y);
        break;
    case 1:
        get2point(x,y);
        break;
    case 2:
        get3point(x,y);
        ecr->setCursor(Qt::ArrowCursor);
        tempPicture3=tempPicture2;
        lightRay();
        ui->pushButton_2->setEnabled(true);
        break;
    case 4:
        reput1(x,y);
        spoint=3;
        break;
    case 5:
        reput2(x,y);
        spoint=3;
        break;
    case 6:
        reput3(x,y);
        spoint=3;
        break;
    case 10:
        on_pushButton_2_clicked();
    case 8:
        dx=x-lighter::geometry().left()-ecr->x()-20;
        dy=y-lighter::geometry().top()-ecr->y()-20;
        setFlash(dx+8,dy+8);
        spoint=9;
        break;
    case 11:
        putBollPoint1(x,y);
        break;
    case 12:
        putBollPoint2(x,y);
        ecr->setCursor(Qt::ArrowCursor);
        tempPicture3=tempPicture2;
        paintBoll();
        ui->pushButton_2->setEnabled(true);
        break;
    case 14:
        reputLampBoll(x,y);
        break;
    case 15:
        reputCenBoll(x,y);
        break;
    }
}

void lighter::reputLampBoll(int x, int y)
{
    get1point(x,y);
    spoint=13;
    tempPicture2=tempPicture3;
    showPict();
    paintBoll();
    ui->pushButton_2->setEnabled(true);
}

void lighter::reputCenBoll(int x, int y)
{
    get2point(x,y);
    spoint=13;
    tempPicture2=tempPicture3;
    showPict();
    paintBoll();
    ui->pushButton_2->setEnabled(true);
}

void lighter::get1point(int x, int y)
{
    int dx=x-lighter::geometry().left()-ecr->x()-20;
    int dy=y-lighter::geometry().top()-ecr->y()-20;
    pl1=QPoint(dx+8,dy+8);
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/vector"))));
    if(v1==0){
        v1=new QavLabel(ecr);
        v1->setScaledContents(true);
        v1->setPixmap(QPixmap::fromImage(QImage(":/new/prefix1/boll")));
        v1->setCursor(Qt::SizeAllCursor);
        v1->setToolTip(tr("Click for move"));
        connect(v1,SIGNAL(mouse_press(int,int,int)),this,SLOT(takeLamp()));
    }
    v1->setGeometry(QRect(dx,dy,16,16));
    v1->show();
}

void lighter::takeLamp()
{
    v1->hide();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
    switch (spoint) {
    case 3:
        spoint=4;
        break;
    case 9:
        spoint=10;
        break;
    case 13:
        spoint=14;
        break;
    default:
        break;
    }
}

void lighter::takeP1()
{
    v2->hide();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/vector"))));
    switch (spoint) {
    case 3:
        spoint=5;
        break;
    case 13:
        spoint=15;
        break;
    default:
        break;
    }
}

void lighter::takeP2()
{
    v3->hide();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/vector"))));
    spoint=6;
}

void lighter::reput1(int x, int y)
{
    get1point(x,y);
    tempPicture2=tempPicture3;
    showPict();
    lightRay();
    ecr->setCursor(Qt::ArrowCursor);
}

void lighter::reput2(int x, int y)
{
    get2point(x,y);
    tempPicture2=tempPicture3;
    showPict();
    lightRay();
    ecr->setCursor(Qt::ArrowCursor);
}

void lighter::reput3(int x, int y)
{
    get3point(x,y);
    tempPicture2=tempPicture3;
    showPict();
    lightRay();
    ecr->setCursor(Qt::ArrowCursor);
}

void lighter::get2point(int x, int y)
{
    int dx=x-lighter::geometry().left()-ecr->x()-20;
    int dy=y-lighter::geometry().top()-ecr->y()-20;
    pls1=QPoint(dx+8,dy+8);
    if(v2==0){
        v2=new QavLabel(ecr);
        v2->setScaledContents(true);
        v2->setPixmap(QPixmap::fromImage(QImage(":/new/prefix1/bollY")));
        v2->setCursor(Qt::SizeAllCursor);
        v2->setToolTip(tr("Click for move"));
        connect(v2,SIGNAL(mouse_press(int,int,int)),this,SLOT(takeP1()));
    }
    v2->setGeometry(QRect(dx,dy,16,16));
    v2->show();
}

void lighter::get3point(int x, int y)
{
    int dx=x-lighter::geometry().left()-ecr->x()-20;
    int dy=y-lighter::geometry().top()-ecr->y()-20;
    pls2=QPoint(dx+8,dy+8);
    if(v3==0){
        v3=new QavLabel(ecr);
        v3->setScaledContents(true);
        v3->setPixmap(QPixmap::fromImage(QImage(":/new/prefix1/bollY")));
        v3->setCursor(Qt::SizeAllCursor);
        v3->setToolTip(tr("Click for move"));
        connect(v3,SIGNAL(mouse_press(int,int,int)),this,SLOT(takeP2()));
    }
    v3->setGeometry(QRect(dx,dy,16,16));
    v3->show();
}

int lighter::to255(int i)
{
    if(i>255)return 255;
    if(i<0)return 0;
    return i;
}

void lighter::lightRay() // наложение вектора общего освещения картинки
{
    getTriangle();
    QPoint sp;
    // пересчет вершин треугольника в координаты картинки
    sp.setX(lbl->geometry().left());
    sp.setY(lbl->geometry().top());
    plp1=pl1-sp;
    plp2=pl2-sp;
    plp3=pl3-sp;
    pp2-=sp;
    pp3-=sp;
    lampPos=side(plp1);  // где вершина, чтобы потом сравнивать
    int l=400;
    // просмотр точек картинки
    for(int x=0;x<tmPix->width();x++)
        for(int y=0;y<tmPix->height();y++){
            if(inRay(QPoint(x,y))){
                tempPicture2[x][y].v+=ui->horizontalSlider->value()-
                        double(ui->horizontalSlider->value()*getDist(plp1,QPoint(x,y)))/l;
                clr2.setHsv(tempPicture2[x][y].h,
                            tempPicture2[x][y].s,
                            to255(tempPicture2[x][y].v),
                            tempPicture2[x][y].al);
                img->setPixel(x,y,clr2.rgba());
            }
        }
    // обновить картинку
    *tmPix=QPixmap::fromImage(*img);
    lbl->setPixmap(*tmPix);
    ui->pushButton->setEnabled(true);
    pr=true;
}

bool lighter::side(QPoint ps)
{
    int d=(ps.x()-pp2.x())*(pp3.y()-pp2.y())-(ps.y()-pp2.y())*(pp3.x()-pp2.x());
    return d>=0;
}

void lighter::getTriangle() // расчет точек пересечения внешних сторон сектора луча со сторонами картинки pl2, pl3
{
    pl2=funPoint(pls1);
    pl3=funPoint(pls2);
    pp2=funPoint2(pls1);
    pp3=funPoint2(pls2);
}

QPoint lighter::funPoint(QPoint pls)
{
    // при х=520 или х=0 картинка точно попадает в треугольник
    QPoint rp;
    if(pl1.x()==pls.x()){   // если точки на одной верт. линии
        rp.setX(pl1.x());
        if(pl1.y()<pls.y())
            rp.setY(0);
        else
            rp.setY(420);// нижняя сторона области экрана - высота ecr
    }
    if(pl1.x()<pls.x()){    // если точка правее
        rp.setX(520);
        rp.setY(getCrossPoint(pl1,pls,520));
    }
    if(pl1.x()>pls.x()){    // если точка левее
        rp.setX(0);
        rp.setY(getCrossPoint(pl1,pls,0));
    }
    return rp;
}

QPoint lighter::funPoint2(QPoint pls)
{
    QPoint rp;
    // расчет сторон картинки
    int ty=lbl->geometry().top();       // верхняя сторона
    int lx=lbl->geometry().left();      // левая сторона
    int rx=lx+lbl->geometry().width();  // правая сторона
    int by=ty+lbl->geometry().height(); // нижняя сторона
    if(pl1.x()==pls.x()){   // если точки на одной верт. линии
        rp.setX(pl1.x());
        if(pl1.y()<pls.y())
            rp.setY(ty);
        else
            rp.setY(by);
    }
    if(pl1.x()<pls.x()){    // если точка правее
        rp.setX(rx);
        rp.setY(getCrossPoint(pl1,pls,rx));
    }
    if(pl1.x()>pls.x()){    // если точка левее
        rp.setX(lx);
        rp.setY(getCrossPoint(pl1,pls,lx));
    }
    return rp;
}

double lighter::getDist(QPoint p1, QPoint ps)
{
    return sqrt((ps.x()-p1.x())*(ps.x()-p1.x())+(ps.y()-p1.y())*(ps.y()-p1.y()));
}

int lighter::getCrossPoint(QPoint p1, QPoint p2, int x)
{
    return ((p1.y()-p2.y())*x+(p1.x()*p2.y()-p2.x()*p1.y()))/(p1.x()-p2.x());
}

bool lighter::inRay(QPoint xy) //определить принадлежность точки лучу (внутри треугольника plp1,plp2,plp3)
{
    // попадание в треугольник, все три выражения имеют одинаковый знак:
    //(x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0)
    //(x2 - x0) * (y3 - y2) - (x3 - x2) * (y2 - y0)
    //(x3 - x0) * (y1 - y3) - (x1 - x3) * (y3 - y0)
    int d1=(plp1.x()-xy.x())*(plp2.y()-plp1.y())-(plp2.x()-plp1.x())*(plp1.y()-xy.y());
    int d2=(plp2.x()-xy.x())*(plp3.y()-plp2.y())-(plp3.x()-plp2.x())*(plp2.y()-xy.y());
    int d3=(plp3.x()-xy.x())*(plp1.y()-plp3.y())-(plp1.x()-plp3.x())*(plp3.y()-xy.y());
    bool r1=((d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0));
    bool r2=false;
    // Если точка ниже основания треугольника
    // то она тоже попадает:
    if(!r1){ // только если точка не внутри треугольника
        r2=(lampPos!=side(xy));
    }
    return r1 || r2;
}

void lighter::on_pushButton_clicked() // еще луч
{
    // сохранить текущий луч в массиве лучей
    rayToVector();
    reset();
}

void lighter::rayToVector() // сохранить текущий луч в массиве лучей
{
    ray r;
    r.p1=plp1;
    r.p2=plp2;
    r.p3=plp3;
    r.pp2=pp2;
    r.pp3=pp3;
    r.value=ui->horizontalSlider->value();
    rays.push_back(r);
    pr=false;
}

void lighter::resultToMain() // завершение работы, результат в главную форму
{
    double k=double(pix.width())/double(tmPix->width()); // пропорция между исходным изображением и черновиком
    *img=pix.toImage();
    int h,s,v,a;
    double l,b,r;
    // лучи
    for(int i=0;i<int(rays.size());i++){
        plp1=rays[i].p1*k;
        plp2=rays[i].p2*k;
        plp3=rays[i].p3*k;
        pp2=rays[i].pp2*k;
        pp3=rays[i].pp3*k;
        lampPos=side(plp1);  // где вершина, чтобы потом сравнивать
        int l=400.0*k;        
        // просмотр точек картинки
        for(int x=0;x<pix.width();x++)
            for(int y=0;y<pix.height();y++){
                if(inRay(QPoint(x,y))){
                    rgb2=img->pixel(x,y);
                    clr2.setRgba(rgb2);
                    clr2.getHsv(&h,&s,&v,&a);
                    v+=rays[i].value-
                            double(rays[i].value*getDist(plp1,QPoint(x,y)))/l;
                    clr2.setHsv(h,s,to255(v),a);
                    img->setPixel(x,y,clr2.rgba());
                }
            }
    }
    // вспышки
    for(int i=0;i<int(flsh.size());i++){
        plp1=fpn1=flsh[i].p1*k;
        double lng=flsh[i].lng*k;
        for(int x=0;x<pix.width();x++)
            for(int y=0;y<pix.height();y++){
                l=getDist(plp1,QPoint(x,y));
                if(l<=lng){
                    rgb2=img->pixel(x,y);
                    clr2.setRgba(rgb2);
                    clr2.getHsv(&h,&s,&v,&a);
                    v+=flsh[i].pwr-double(flsh[i].pwr*l)/(lng);
                    clr2.setHsv(h,s,to255(v),a);
                    img->setPixel(x,y,clr2.rgba());
                }
            }
    }
    // шары
    for(int i=0;i<int(bls.size());i++){
        plp1=bls[i].lmp*k;
        plp3=bls[i].cen*k;
        b=bls[i].brt;
        r=bls[i].rds*k;
        for(int x=0;x<pix.width();x++)
            for(int y=0;y<pix.height();y++){
                l=getDist(plp3,QPoint(x,y));
                  if(l<=r){
                      rgb2=img->pixel(x,y);
                      clr2.setRgba(rgb2);
                      clr2.getHsv(&h,&s,&v,&a);
                      v+=b-(b*getDist(plp1,QPoint(x,y)))/r;
                      clr2.setHsv(h,s,to255(v),a);
                      img->setPixel(x,y,clr2.rgba());
                }
            }
    }
    // результат
    pix=QPixmap::fromImage(*img);
    emit endLighter(pix);
    close();
}

void lighter::on_horizontalSlider_valueChanged(int value)
{
    int v=(double(value)/256.0)*100.0;
    ui->label_2->setText(QString::number(v)+"%");
    if(retFlag)return;
    if(spoint==3){
        tempPicture2=tempPicture3;
        showPict();
        lightRay();
    }
}

void lighter::on_pushButton_2_clicked() // шаг назад
{
    ui->pushButton_2->setEnabled(false);
    tempPicture2=tempPicture3;
    showPict();
    reset();
    if(spoint==0 && int(rays.size())>0)rays.resize(rays.size()-1);
    if(spoint==8 && int(flsh.size())>0)flsh.resize(flsh.size()-1);
}

void lighter::showPict()
{
    for(int x=0;x<tmPix->width();x++)
        for(int y=0;y<tmPix->height();y++){
            clr2.setHsv(tempPicture2[x][y].h,
                        tempPicture2[x][y].s,
                        to255(tempPicture2[x][y].v),
                        tempPicture2[x][y].al);
            img->setPixel(x,y,clr2.rgba());
        }
    *tmPix=QPixmap::fromImage(*img);
    lbl->setPixmap(*tmPix);
}

void lighter::on_pushButton_3_clicked()
{
    if(pr)rayToVector();
    if(pf)saveFlash();
    if(pb)saveBoll();
    reset();
    resultToMain();
}

void lighter::on_pushButton_4_clicked() // перезагрузить картинку
{
    int s=spoint;
    loadPicture();
    if(s<8)ui->tabWidget->setCurrentIndex(0);
    if(s>7 && s<11)ui->tabWidget->setCurrentIndex(1);
    if(s>10)ui->tabWidget->setCurrentIndex(2);
    ui->pushButton->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
}

void lighter::on_tabWidget_currentChanged(int index) // переключение вкладок
{
    if(retFlag)return;
    switch (index) {
    case 0:
        spoint=0;
        break;
    case 1:
        spoint=8;
        break;
    case 2:
        spoint=11;
        break;
    default:
        break;
    }
    reset();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
}

void lighter::setFlash(int x, int y) // вспышка в точке x,y
{
    fpn=QPoint(x,y);
    QPoint sp;
    tempPicture3=tempPicture2; // копия для отката
    // пересчет в координаты картинки
    sp.setX(lbl->geometry().left());
    sp.setY(lbl->geometry().top());
    plp1=fpn1=fpn-sp;
    double l;
    for(int x=0;x<tmPix->width();x++)
        for(int y=0;y<tmPix->height();y++){
            l=getDist(plp1,QPoint(x,y));
            if(l<=ui->horizontalSlider_3->value()){
                tempPicture2[x][y].v+=ui->horizontalSlider_2->value()-
                        double(ui->horizontalSlider_2->value()*l)/ui->horizontalSlider_3->value();
                clr2.setHsv(tempPicture2[x][y].h,
                            tempPicture2[x][y].s,
                            to255(tempPicture2[x][y].v),
                            tempPicture2[x][y].al);
                img->setPixel(x,y,clr2.rgba());
            }
        }
    // обновить картинку
    *tmPix=QPixmap::fromImage(*img);
    lbl->setPixmap(*tmPix);
    ui->pushButton_2->setEnabled(true);
    if(v1==0){
        v1=new QavLabel(ecr);
        v1->setScaledContents(true);
        v1->setPixmap(QPixmap::fromImage(QImage(":/new/prefix1/boll")));
        v1->setCursor(Qt::SizeAllCursor);
        v1->setToolTip(tr("Click for move"));
        connect(v1,SIGNAL(mouse_press(int,int,int)),this,SLOT(takeLamp()));
    }
    v1->setGeometry(QRect(fpn.x()-8,fpn.y()-8,16,16));
    v1->show();
    ecr->setCursor(Qt::ArrowCursor);
    ui->pushButton_5->setEnabled(true);
    pf=true;
}

int lighter::sgn(double x)
{
    if(x>0)return 1;
    if(x<0)return -1;
    return 0;
}

void lighter::on_horizontalSlider_2_valueChanged(int value)
{
    int v=(double(value)/256.0)*100.0;
    ui->label_4->setText(QString::number(v)+"%");
    if(retFlag)return;
    if(spoint==9){
        tempPicture2=tempPicture3;
        setFlash(fpn.x(),fpn.y());
    }
}

void lighter::on_horizontalSlider_3_valueChanged(int value)
{
    int v=(double(value)/256.0)*100.0;
    ui->label_6->setText(QString::number(v)+"%");
    if(retFlag)return;
    if(spoint==9){
        tempPicture2=tempPicture3;
        setFlash(fpn.x(),fpn.y());
    }
}

void lighter::on_pushButton_5_clicked() // следующая вспышка
{
    spoint=8;
    v1->hide();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
    saveFlash();
}

void lighter::saveFlash()
{
    flash f;
    f.lng=ui->horizontalSlider_3->value();
    f.pwr=ui->horizontalSlider_2->value();
    f.p1=fpn1;
    flsh.push_back(f);
    pf=false;
}

void lighter::putBollPoint1(int x, int y)
{
    get1point(x,y);
}

void lighter::putBollPoint2(int x, int y)
{
    get2point(x,y);
}

void lighter::paintBoll() //нарисовать шар между точками
{
    QPoint sp;
    tempPicture3=tempPicture2; // копия для отката
    // пересчет в координаты картинки
    sp.setX(lbl->geometry().left());
    sp.setY(lbl->geometry().top());
    plp1=pl1-sp;       // лама
    plp2=pls1-sp;      // опорная точка
    // пропорция между заданным растоянием и радиусом
    double k=getDist(plp1,plp2)/double(ui->horizontalSlider_5->value());
    int dx=(plp2.x()-plp1.x())/k;
    int dy=(plp2.y()-plp1.y())/k;
    plp3=QPoint(plp1.x()+dx,plp1.y()+dy);   // центр шара
    double l;
    for(int x=0;x<tmPix->width();x++)
        for(int y=0;y<tmPix->height();y++){
            l=getDist(plp3,QPoint(x,y));
              if(l<=ui->horizontalSlider_5->value()){
                tempPicture2[x][y].v+=ui->horizontalSlider_4->value()-
                        double(ui->horizontalSlider_4->value()*getDist(plp1,QPoint(x,y)))/
                               ui->horizontalSlider_5->value();
                clr2.setHsv(tempPicture2[x][y].h,
                            tempPicture2[x][y].s,
                            to255(tempPicture2[x][y].v),
                            tempPicture2[x][y].al);
                img->setPixel(x,y,clr2.rgba());
            }
        }
    // обновить картинку
    *tmPix=QPixmap::fromImage(*img);
    lbl->setPixmap(*tmPix);
    ecr->setCursor(Qt::ArrowCursor);
    ui->pushButton_6->setEnabled(true);
    pb=true;
}


void lighter::on_horizontalSlider_4_valueChanged(int value)
{
    int v=(double(value)/256.0)*100.0;
    ui->label_9->setText(QString::number(v)+"%");
    if(retFlag)return;
    if(spoint==13){
        tempPicture2=tempPicture3;
        paintBoll();
    }
}

void lighter::on_horizontalSlider_5_valueChanged(int value)
{
    ui->label_10->setText(QString::number(value));
    if(retFlag)return;
    if(spoint==13){
        tempPicture2=tempPicture3;
        showPict();
        paintBoll();
    }
}

void lighter::on_pushButton_6_clicked() // следующий шар
{
    saveBoll();
    spoint=11;
    v1->hide();
    v2->hide();
    ecr->setCursor(QCursor(QPixmap::fromImage(QImage(":/new/prefix1/lamp"))));
}

void lighter::saveBoll() // сохранить шар в массив
{
    bolls bl;
    bl.brt=ui->horizontalSlider_4->value();
    bl.rds=ui->horizontalSlider_5->value();
    bl.lmp=plp1;
    bl.cen=plp3;
    bls.push_back(bl);
    pb=false;
}
