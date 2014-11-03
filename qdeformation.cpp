#include "qdeformation.h"
#include "ui_qdeformation.h"
#include "avLabel.h"
#include <QPainter>
#include <QMovie>

#include <iostream>
using namespace std;

QavLabel    *lTop,*lBot,*rTop,*rBot;   // "прихватки" по углам
QPoint      lt,lb,rt,rb;               // координаты "прихваток"
QPoint      p1,p2,p3,p4;               // исходные углы картинки (начальний фактический полигон)
QPoint      s1,s2,s3,s4;               // углы запомненного полигона
int         prihPress=0;               // номер нажатой "прихватки"
QPoint      xy;                        // координаты нажатия на "прихватку" (для отработки смещения)
QPolygonF   plgn;                      // текущий полигон картинки
QPolygonF   newPlgn;                   // новый полигон
QTransform  trns;                      // матрица преобразования
QPixmap     *tPix;                     // промежуточный pixmap для обработки
int         dxa,dxb,dxc,dxd;
int         dya,dyb,dyc,dyd;           // смещения прихваток
int         ndx,ndy;                   // смещения картинки при деформации в отрицательную зону
double      smooth;                    // коэффициент размытия
bool        toSteps=true;              // сохранять шаги трансформации в массиве шагов

struct polygon {
    QPoint  lTop;                       // верхний левый угол
    QPoint  rTop;                       // верхний правый угол
    QPoint  rBot;                       // нижний правый угол
    QPoint  lBot;                       // нижний левый угол
    bool    smooth;                     // размытие
};

std::vector<polygon> steps;             //массив последовательных преобразований полигона

Qdeformation::Qdeformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Qdeformation)
{
    ui->setupUi(this);
    tPix=new QPixmap(1,1);
    ui->label_6->hide();
    ui->doubleSpinBox->setEnabled(false);
    smooth=ui->doubleSpinBox->value();
    ui->pushButton_7->setEnabled(false);
}

Qdeformation::~Qdeformation()
{
    delete ui;
}

void Qdeformation::set_lTop(QPoint pn)
{
    if(lTop==0){
        lTop=new QavLabel(ui->frame);
        lTop->setImnum(0);
    }
    makeBoll(*lTop,pn.x(),pn.y(),1);
}

void Qdeformation::set_lBot(QPoint pn)
{
    if(lBot==0){
        lBot=new QavLabel(ui->frame);
        lBot->setImnum(0);
    }
    makeBoll(*lBot,pn.x(),pn.y(),4);
}

void Qdeformation::set_rBot(QPoint pn)
{
    if(rBot==0){
        rBot=new QavLabel(ui->frame);
        rBot->setImnum(0);
    }
    makeBoll(*rBot,pn.x(),pn.y(),3);
}

void Qdeformation::set_rTop(QPoint pn)
{
    if(rTop==0){
        rTop=new QavLabel(ui->frame);
        rTop->setImnum(0);
    }
    makeBoll(*rTop,pn.x(),pn.y(),2);
}


void Qdeformation::makeBoll(QavLabel &lb, int x, int y, int index)
{
    if(lb.getImnum()!=0){
        lb.move(x,y);
        return;
    }
    lb.setParent(this);
    QImage im(":/new/prefix1/boll");
    lb.setPixmap(QPixmap::fromImage(im));
    lb.setScaledContents(true);
    lb.setGeometry(x,y,16,16);
    lb.setCursor(Qt::PointingHandCursor);
    lb.setImnum(index);
    connect(&lb,SIGNAL(mouse_press(int,int,int)),this,SLOT(ms_press(int,int,int)));
    connect(&lb,SIGNAL(mouse_up(int,int,int)),this,SLOT(ms_up(int,int,int)));
    connect(&lb,SIGNAL(mouse_move(int,int,int)),this,SLOT(ms_move(int,int,int)));
    lb.show();
}

void Qdeformation::ms_press(int x, int y, int i)
{
    xy.setX(x);
    xy.setY(y);
    prihPress=i;
    switch (i) {
    case 1:
        lTop->setCursor(Qt::SizeAllCursor);
        break;
    case 2:
        rTop->setCursor(Qt::SizeAllCursor);
        break;
    case 3:
        rBot->setCursor(Qt::SizeAllCursor);
        break;
    case 4:
        lBot->setCursor(Qt::SizeAllCursor);
        break;
    default:
        prihPress=0;
        break;
    }
}

QPoint Qdeformation::inFrame(QPoint pn)
{
    if(pn.x()>630)  pn.setX(630);
    if(pn.x()<0)    pn.setX(0);
    if(pn.y()>400 ) pn.setY(400);
    if(pn.y()<0)    pn.setY(0);
    return pn;
}

void Qdeformation::ms_up(int x, int y, int i)
{
    xy.setX(x);
    xy.setY(y);
    prihPress=0;
    switch (i) {
    case 1:
        lTop->setCursor(Qt::PointingHandCursor);
        lt=QPoint(lTop->geometry().left(),lTop->geometry().top());
        break;
    case 2:
        rTop->setCursor(Qt::PointingHandCursor);
        rt=QPoint(rTop->geometry().left(),rTop->geometry().top());
        break;
    case 3:
        rBot->setCursor(Qt::PointingHandCursor);
        rb=QPoint(rBot->geometry().left(),rBot->geometry().top());
        break;
    case 4:
        lBot->setCursor(Qt::PointingHandCursor);
        lb=QPoint(lBot->geometry().left(),lBot->geometry().top());
        break;
    default:
        prihPress=0;
        break;
    }
    distortion();
}

void Qdeformation::distortion() // искажение картинки
{
    getD();
    QPixmap px=getNewPix();
    double k=double(pix.width())/double((p2.x()-p1.x()));
    px=getPixOfSize(px, getNewPolygon()/k);
    px=makePixForLabel(px);
    ui->label->setPixmap(px);
    saveStep();
}

QPixmap Qdeformation::getPixOfSize(QPixmap p, QSize sz)
{
    QPixmap canv_pix(sz);                       // промежуточный pixmap для рисования в нем
    canv_pix.fill(Qt::transparent);             // залить пустотой
    QPainter pntr(&canv_pix);                   // живописец, привязанный к pixmap
    pntr.drawPixmap(QRect(QPoint(0,0),sz),p);   // нарисовать
    pntr.end();                                 // завершить работу живописца
    return canv_pix;
}

QPixmap Qdeformation::makePixForLabel(QPixmap p)
{
    QPixmap resalt(QSize(630,400));
    resalt.fill(Qt::transparent);
    QPainter pntr(&resalt);
    QSize sz(p.size());
    int x=lt.x();
    if(x>rt.x())x=rt.x();
    if(x>rb.x())x=rb.x();
    if(x>lb.x())x=lb.x();
    int y=lt.y();
    if(y>rt.y())y=rt.y();
    if(y>rb.y())y=rb.y();
    if(y>lb.y())y=lb.y();

    pntr.drawPixmap(QRect(QPoint(x,y),sz),p);
    pntr.end();
    return resalt;
}

void Qdeformation::ms_move(int x, int y, int i)
{
    if(prihPress==0)return;
    int dx=xy.x()-x;
    int dy=xy.y()-y;
    QPoint pn;
    switch (i) {
    case 1:
        pn=bollMove(*lTop,dx,dy);
        break;
    case 2:
        pn=bollMove(*rTop,dx,dy);
        break;
    case 3:
        pn=bollMove(*rBot,dx,dy);
        break;
    case 4:
        pn=bollMove(*lBot,dx,dy);
        break;
    default:
        pn=QPoint(0,0);
        break;
    }
   if(pn.x()!=0) xy.setX(x);
   if(pn.y()!=0) xy.setY(y);
}

QPoint Qdeformation::bollMove(QavLabel &lb, int dx, int dy)
{
    QRect   rc=lb.geometry();
    QPoint  pn=inFrame(QPoint(rc.left()-dx,rc.top()-dy));
    QPoint  p2;
    p2.setX(rc.left()-pn.x());
    p2.setY(rc.top()-pn.y());
    rc.moveTo(pn);
    lb.setGeometry(rc);
    return p2;

}

void Qdeformation::setBolls()
{
    set_lTop(lt);
    set_lBot(lb);
    set_rTop(rt);
    set_rBot(rb);
}

void Qdeformation::loadPixmap()
{
    double k=double(pix.width())/double(pix.height());  // пропорции картинки
    QRect  rc;
    rc.setRect(0,0,380,380);                            // начальная геометрия лабела на экране
    if(k>=1){
        rc.setHeight(rc.width()/k);                     // новая высота для сохранения пропорции
    }else{
        rc.setWidth(rc.height()*k);                     // новая ширина для сохранения пропорции
    }
    rc.moveTo((630-rc.width())/2,(400-rc.height())/2);  // поместить в центр области трансформации
    //задать начальные координаты "прихваток"
    p1=lt=QPoint(2+(630-rc.width())/2,2+(400-rc.height())/2);
    p4=lb=QPoint(lt.x(),lt.y()+rc.height());
    p2=rt=QPoint(lt.x()+rc.width(),lt.y());
    p3=rb=QPoint(rt.x(),lb.y());
    setBolls();                                         // установить шарики по углам - прихватки для трансформации
    *tPix=getPixOfSize(pix,QSize(rc.width(),rc.height()));
    *tPix=makePixForLabel(*tPix);
    ui->label->setPixmap(*tPix);                        // показать картинку
    steps.clear();
    saveStep();
    ui->pushButton_8->setEnabled(false);
}

void Qdeformation::saveStep()
{
    if(toSteps){
        polygon pl;
        pl.lBot=lb;
        pl.lTop=lt;
        pl.rBot=rb;
        pl.rTop=rt;
        pl.smooth=ui->checkBox->isChecked();
        ui->pushButton_8->setEnabled(true);
        steps.resize(steps.size()+1);
        steps[steps.size()-1]=pl;
    }
}

void Qdeformation::on_pushButton_clicked()
{
    emit pixFin(getNewPix());
    this->close();
}

QPixmap Qdeformation::getNewPix()
{
    QImage im=pix.toImage();
    QSize sz=getNewPolygon();
    double xx=double(sz.width());
    double yy=double(sz.height());
    double xl,yl,dxx,dyy;
    QPixmap v_pix(QSize(xx,yy));                         // промежуточный pixmap для рисования в нем
    v_pix.fill(Qt::transparent);
    QImage im2=v_pix.toImage();
    QRgb   rgb;
    xl=getNewX(0,0);
    yl=getNewY(0,0);
    ui->label_6->show();
    QApplication::processEvents();
    for(int x=0;x<pix.width();x++){
        for(int y=0;y<pix.height();y++){
            rgb=im.pixel(x,y);
            xx=getNewX(x,y);
            yy=getNewY(x,y);
            if(xx-ndx>=0 && xx-ndx<sz.width() && yy-ndy>=0 && yy-ndy<sz.height())
                    im2.setPixel(xx-ndx,yy-ndy,rgb);
            else
            {
                xl=ndx;
                yl=ndy;
            }
            if(ui->checkBox->isChecked() && (x>0 && y>0)) {  // заполнение просветов - smoothing
                dxx=xx-xl; dyy=yy-yl;
                for(int i=xx-dxx/smooth-1;i<xx+dxx/smooth+1;i++){
                    for(int j=yy-dyy/smooth-1;j<yy+dyy/smooth+1;j++){
                        if(i-ndx>=0 && i-ndx<sz.width() && j-ndy>=0 && j-ndy<sz.height())
                                                im2.setPixel(i-ndx,j-ndy,rgb);
                    }
                }
            }
            xl=xx;yl=yy;
        }      
    }
    ui->label_6->hide();
    return QPixmap::fromImage(im2);
}

void Qdeformation::getD()
{
    double k=double(pix.width())/double((p2.x()-p1.x()));
    dxa=k*(rb.x()-p3.x()); dya=k*(rb.y()-p3.y());
    dxb=k*(rt.x()-p2.x()); dyb=k*(rt.y()-p2.y());
    dxc=k*(lt.x()-p1.x()); dyc=k*(lt.y()-p1.y());
    dxd=k*(lb.x()-p4.x()); dyd=k*(lb.y()-p4.y());
}

double Qdeformation::minValue(double v1, double v2, double v3, double v4)
{
    double r=v1;
    if(v2<r)r=v2;
    if(v3<r)r=v3;
    if(v4<r)r=v4;
    return r;
}

double Qdeformation::maxValue(double v1, double v2, double v3, double v4)
{
    double r=v1;
    if(v2>r)r=v2;
    if(v3>r)r=v3;
    if(v4>r)r=v4;
    return r;
}

QSize Qdeformation::getNewPolygon()
{
    double x1=getNewX(0,0);
    double x2=getNewX(pix.width(),0);
    double x3=getNewX(pix.width(),pix.height());
    double x4=getNewX(0,pix.height());
    double minX=minValue(x1,x2,x3,x4);
    double maxX=maxValue(x1,x2,x3,x4);
    x1=maxX-minX;
    double y1=getNewY(0,0);
    double y2=getNewY(pix.width(),0);
    double y3=getNewY(pix.width(),pix.height());
    double y4=getNewY(0,pix.height());
    double minY=minValue(y1,y2,y3,y4);
    double maxY=maxValue(y1,y2,y3,y4);
    y1=maxY-minY;
    ndx=minX;
    ndy=minY;
    return QSize(x1+1,y1+1);
}

double Qdeformation::getNewX(int x, int y) // пересчет координаты X
{
    double N=pix.height()*pix.width();
    double dbx=x,dby=y,Nx=pix.width(),Ny=pix.height();
    double x1=(dxa*dby)/N*dbx;
    double x2=(dxb*(Ny-dby))/N*dbx;
    double x3=(dxc*(Ny-dby))/N*(Nx-dbx);
    double x4=(dxd*dby)/N*(Nx-dbx);
    return dbx+x1+x2+x3+x4;
}

double Qdeformation::getNewY(int x, int y) // пересчет координаты Y
{
    double N=pix.height()*pix.width();
    double dbx=x,dby=y,Nx=pix.width(),Ny=pix.height();
    double y1=(dya*dbx)/N*dby;
    double y2=(dyb*dbx)/N*(Ny-dby);
    double y3=(dyc*(Nx-dbx))/N*(Ny-dby);
    double y4=(dyd*(Nx-dbx))/N*dby;
    return dby+y1+y2+y3+y4;
}

void Qdeformation::on_checkBox_clicked()
{
    distortion();
}

void Qdeformation::on_pushButton_2_clicked() // отразить по горизонтале
{
    QImage im1=pix.toImage();
    QImage im2=pix.toImage();
    QRgb rgb;
    for(int x=0;x<pix.width();x++){
        for(int y=0;y<pix.height();y++){
            rgb=im1.pixel(x,y);
            im2.setPixel(pix.width()-x-1,y,rgb);
        }
    }
    pix=QPixmap::fromImage(im2);
    distortion();
}

void Qdeformation::on_pushButton_3_clicked() // отразить по вертикале
{
    QImage im1=pix.toImage();
    QImage im2=pix.toImage();
    QRgb rgb;
    for(int x=0;x<pix.width();x++){
        for(int y=0;y<pix.height();y++){
            rgb=im1.pixel(x,y);
            im2.setPixel(x,pix.height()-y-1,rgb);
        }
    }
    pix=QPixmap::fromImage(im2);
    distortion();
}

void Qdeformation::on_doubleSpinBox_valueChanged(double arg1)
{
    smooth=arg1;
    distortion();
}

void Qdeformation::on_checkBox_clicked(bool checked) // разрешить изменение smooth
{
    ui->doubleSpinBox->setEnabled(checked);
}

void Qdeformation::on_pushButton_4_clicked() // горизольнтальное обращение полигона
{
    int p=lt.y();
    lt.setY(rt.y()); rt.setY(p);
    p=630-lt.x();
    lt.setX(630-rt.x());rt.setX(p);
    p=lb.y();
    lb.setY(rb.y()); rb.setY(p);
    p=630-lb.x();
    lb.setX(630-rb.x());rb.setX(p);
    setBolls();
    distortion();
}

void Qdeformation::on_pushButton_5_clicked() // вертикальное обращение полигона
{
    int p=lt.x();
    lt.setX(lb.x()); lb.setX(p);
    p=400-lt.y();
    lt.setY(400-lb.y());lb.setY(p);
    p=rb.x();
    rb.setX(rt.x()); rt.setX(p);
    p=400-rb.y();
    rb.setY(400-rt.y());rt.setY(p);
    setBolls();
    distortion();
}

void Qdeformation::on_pushButton_6_clicked() // save polygon
{
    s1=lt;s2=rt;s3=rb;s4=lb;
    ui->pushButton_7->setEnabled(true);
}

void Qdeformation::on_pushButton_7_clicked() // load saved polygon
{
    lt=s1;rt=s2;rb=s3;lb=s4;
    setBolls();
    distortion();
}

void Qdeformation::on_pushButton_8_clicked() // шаг назад
{
    steps.resize(steps.size()-1);
    lt=steps[steps.size()-1].lTop;
    rt=steps[steps.size()-1].rTop;
    lb=steps[steps.size()-1].lBot;
    rb=steps[steps.size()-1].rBot;
    ui->checkBox->setChecked(steps[steps.size()-1].smooth);
    if(steps.size()==1)ui->pushButton_8->setEnabled(false);
    setBolls();
    toSteps=false;
    distortion();
    toSteps=true;
}

void Qdeformation::on_pushButton_10_clicked()
{
    loadPixmap();
}

void Qdeformation::on_pushButton_9_clicked()
{
    this->close();
}
