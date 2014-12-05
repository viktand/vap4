#include "repaint.h"
#include "ui_repaint.h"
#include <QPainter>
#include <QVector>
#include <QProgressDialog>
#include "avLabel.h"

QavLabel *label;
QPixmap *tempPix;
QPixmap *temp2Pix;         // для отмены назначения прозрачности
QImage  *im;
QColor  clr;
QColor  trsColor;
QRgb    rgb;
int     oldV=0;         // старое занчение яркости (до смещения регулятора)
int     oldH=0;
int     oldS=0;
int     oldR=0;
int     oldG=0;
int     oldB=0;
int     oldA=0;
bool    flg=true;
bool    trsFag=true;    // флаг режима выбора прозрачного цвета. True - нет выбоа (наоборот, чтобы код был красивее)

struct pixel{  //пиксель без ограничений по разбросу значений
    int h;     // тон
    int s;     // насыщенность
    int v;     // яркость
    int al;    // альфа
};

std::vector <std::vector<pixel> > tempPicture;  // рабочий массив для преобразования картинки
std::vector <QPoint> trnsPoints;                // массив кликнутых точек, которые должны стать прозрачными

QRepaintPixmap::QRepaintPixmap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRepaintPixmap)
{
    ui->setupUi(this);
    tempPix=new QPixmap(1,1);
    im = new QImage(tempPix->toImage());
    label = new QavLabel(this);
    connect(label,SIGNAL(mouse_up(int,int,int)),this,SLOT(setTransparentColor(int,int)));
    label->show();
}

QRepaintPixmap::~QRepaintPixmap()
{
    delete ui;
}

void QRepaintPixmap::loadPicture()
{
    resetSliders();
    double ratio=double(pix.width())/double(pix.height());
    if(ratio>1)label->setGeometry(QRect(49,25+(400-400/ratio)/2,400,400/ratio));
    else label->setGeometry(QRect(25+(400-400*ratio)/2,49,400*ratio,400));
    loadInVector();
    label->setCursor(Qt::ArrowCursor);
    trnsPoints.resize(0);
}

QPixmap QRepaintPixmap::getPixOfSize(QPixmap p, QSize sz)
{
    QPixmap canv_pix(sz);                       // промежуточный pixmap для рисования в нем
    canv_pix.fill(Qt::transparent);             // залить пустотой
    QPainter pntr(&canv_pix);                   // живописец, привязанный к pixmap
    pntr.drawPixmap(QRect(QPoint(0,0),sz),p);   // нарисовать
    pntr.end();                                 // завершить работу живописца
    return canv_pix;
}

int QRepaintPixmap::getHue(int h)
{
    if(h>359)return 359;
    if(h<0)return 0;
    return h;
}

int QRepaintPixmap::getSaturation(int s)
{
    if(s>255)return 255;
    if(s<0)return 0;
    return s;
}

void QRepaintPixmap::putPixel(int x, int y)
{
    clr.setHsv(getHue(tempPicture[x][y].h),
               getSaturation(tempPicture[x][y].s),
               getSaturation(tempPicture[x][y].v),
               getSaturation(tempPicture[x][y].al));
    im->setPixel(x,y,clr.rgba());
}

void QRepaintPixmap::putPixelRGB(int x, int y)
{
    im->setPixel(x,y,qRgba(getSaturation(tempPicture[x][y].h),
                           getSaturation(tempPicture[x][y].s),
                           getSaturation(tempPicture[x][y].v),
                           getSaturation(tempPicture[x][y].al)));
}

void QRepaintPixmap::on_horizontalSlider_valueChanged(int value) // изменение яркости
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].v+=value-oldV;
            putPixel(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_4->setText(QString::number(value));
    oldV=value;
}

void QRepaintPixmap::on_horizontalSlider_2_valueChanged(int value) // изменение тона
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].h+=value-oldH;
            putPixel(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_5->setText(QString::number(value));
    oldH=value;
}


void QRepaintPixmap::on_horizontalSlider_3_valueChanged(int value) // изменение насыщенности
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].s+=value-oldS;
            putPixel(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_6->setText(QString::number(value));
    oldS=value;
}

void QRepaintPixmap::on_pushButton_2_clicked() // ок - сохранить результат
{
    *im=pix.toImage().convertToFormat(QImage::Format_ARGB32,Qt::AutoColor);
    QProgressDialog* pprd = new QProgressDialog(tr("Installation of new colors"), "&Cancel", 0, pix.width());
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Repaint"));
    pprd->setAutoClose(true);
    pprd->setAutoReset(true);
    pprd->show();
    int h=0,s=0,v=0,a=oldA;
    if((oldH != 0)||(oldS != 0)||(oldV != 0)||(oldA != 0)||(oldR != 0)||(oldG != 0)||(oldB != 0)){
        for(int x=0;x<pix.width();x++){
            for(int y=0;y<pix.height();y++){
                if((oldH != 0)||(oldS != 0)||(oldV != 0)||(oldA != 0)){
                    clr.setRgba(im->pixel(x,y));
                    clr.getHsv(&h,&s,&v,&a);
                    clr.setHsv(getHue(h+oldH),
                               getSaturation(s+oldS),
                               getSaturation(v+oldV),
                               getSaturation(a+oldA));
                    im->setPixel(x,y,clr.rgba());
                }
                if((oldR != 0)||(oldG != 0)||(oldB != 0)){
                    rgb=im->pixel(x,y);
                    h=getSaturation(qRed(rgb)+oldR);
                    s=getSaturation(qGreen(rgb)+oldG);
                    v=getSaturation(qBlue(rgb)+oldB);
                    a=qAlpha(rgb);
                    im->setPixel(x,y,qRgba(h,s,v,a));
                }

            }
            pprd->setValue(x+1);
            qApp->processEvents();
            if (pprd->wasCanceled())
            {
                *im=tempPix->toImage();
                return;
            }
        }
    }

    if(int(trnsPoints.size()>0)){    // если были назначены прозрачные цвета
        pprd->setLabelText(tr("Installation transparency"));
        pprd->setValue(0);
        pprd->show();
        double cf=double(pix.width())/double(temp2Pix->width());
        std::vector <QRgb> points;
        for(int z=0;z<int(trnsPoints.size());z++)
            points.push_back(im->pixel(cf*trnsPoints[z].x(),cf*trnsPoints[z].y())); // собрать массив фактических цветов, которые будут прозрачными
        for(int x=0;x<pix.width();x++){
            for(int y=0;y<pix.height();y++){
                rgb=im->pixel(x,y);
                for(int z=0;z<int(points.size());z++){
                    if(campColor(rgb,points[z],ui->spinBox->value())){
                        im->setPixel(x,y,qRgba(255,255,255,0));
                        break;
                    }
                }
            }
            pprd->setValue(x+1);
            qApp->processEvents();
            if (pprd->wasCanceled())
            {
                *im=tempPix->toImage();
                return;
            }
        }
    }
    pix=QPixmap::fromImage(*im);
    emit endRepaint(pix);
    delete pprd;
    close();
}

void QRepaintPixmap::on_tabWidget_currentChanged(int index) // переключение вкладок
{
    if(flg)return;
    label->setCursor(Qt::ArrowCursor);
    trsFag=true;
    if(index>0){
        for(int x=0;x<tempPix->width();x++)
            for(int y=0;y<tempPix->height();y++){
                rgb=im->pixel(x,y);
                tempPicture[x][y].h=qRed(rgb);
                tempPicture[x][y].s=qGreen(rgb);
                tempPicture[x][y].v=qBlue(rgb);
                tempPicture[x][y].al=qAlpha(rgb);
            }
        if(index==2){
            label->setCursor(Qt::CrossCursor);
            trsFag=false;
        }
    }else{
        int h,s,v,a;
        for(int x=0;x<tempPix->width();x++)
            for(int y=0;y<tempPix->height();y++){
                rgb=im->pixel(x,y);
                clr.setRgba(rgb);
                clr.getHsv(&h,&s,&v,&a);
                tempPicture[x][y].h=h;
                tempPicture[x][y].s=s;
                tempPicture[x][y].v=v;
                tempPicture[x][y].al=a;

            }    
    }
}

void QRepaintPixmap::on_horizontalSlider_4_valueChanged(int value) // red
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].h+=value-oldR;
            putPixelRGB(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_13->setText(QString::number(value));
    oldR=value;
}

void QRepaintPixmap::on_horizontalSlider_5_valueChanged(int value) // green
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].s+=value-oldG;
            putPixelRGB(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_11->setText(QString::number(value));
    oldG=value;
}

void QRepaintPixmap::on_horizontalSlider_6_valueChanged(int value) // blue
{
    if(flg)return;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            tempPicture[x][y].v+=value-oldB;
            putPixelRGB(x,y);
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_12->setText(QString::number(value));
    oldB=value;
}

void QRepaintPixmap::on_pushButton_clicked() // вернуть исходную картинку
{
    resetSliders();
    loadInVector();
    trsFag=true;
    ui->pushButton_3->setEnabled(false);
    label->setCursor(Qt::ArrowCursor);
}

void QRepaintPixmap::loadInVector()  // загрузить картинку в вектор для быстрой обработки
{
    QSize sz;
    sz.setWidth(label->geometry().width());
    sz.setHeight(label->geometry().height());
    *tempPix=getPixOfSize(pix,sz);
    label->setPixmap(*tempPix);
    *im=tempPix->toImage();
    tempPicture.resize(tempPix->width());
    for(int x=0;x<tempPix->width();x++)tempPicture[x].resize(tempPix->height());
    load();
}

void QRepaintPixmap::resetSliders()
{
    flg=true; // блокировать реакцию на изменение слайдеров
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
    ui->horizontalSlider_4->setValue(0);
    ui->horizontalSlider_5->setValue(0);
    ui->horizontalSlider_6->setValue(0);
    ui->horizontalSlider_7->setValue(0);
    ui->label_4->setText("0");
    ui->label_5->setText("0");
    ui->label_6->setText("0");
    ui->label_11->setText("0");
    ui->label_12->setText("0");
    ui->label_13->setText("0");
    ui->label_14->setText("0");
    oldH=oldS=oldV=oldR=oldG=oldB=oldA=0;
    ui->tabWidget->setCurrentIndex(0);
    flg=false;
}

void QRepaintPixmap::on_horizontalSlider_7_valueChanged(int value) // transporancy
{   // управление прозрачностью
    // пройти в цикле всю картинку и поправить прозрачность
    *im=tempPix->toImage().convertToFormat(QImage::Format_ARGB32,Qt::AutoColor);
    int h=0,s=0,v=0,a=oldA;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
                clr.setRgba(im->pixel(x,y));
                clr.getHsv(&h,&s,&v,&a);
                clr.setHsv(getHue(h+oldH),
                           getSaturation(s+oldS),
                           getSaturation(v+oldV),
                           getSaturation(a+oldA));
                im->setPixel(x,y,clr.rgba());
        }
    label->setPixmap(QPixmap::fromImage(*im));
    ui->label_14->setText(QString::number(value));
    oldA=value;
}



void QRepaintPixmap::on_pushButton_3_clicked() //шаг назад в назначении прозрачных цветов
{
    ui->pushButton_3->setEnabled(false);
    *tempPix=*temp2Pix;
    *im=tempPix->toImage();
    label->setPixmap(*tempPix);
    trnsPoints.resize(trnsPoints.size()-1);
}

void QRepaintPixmap::setTransparentColor(int X, int Y) // установть прозрачный цвет по щелчку по картинке
{
    if(trsFag)return;
    if(temp2Pix==0)temp2Pix=new QPixmap();
    *temp2Pix=*tempPix;
    ui->label_box->setText("");
    int dx=0,dy=0; // смещение глобальных координат (из сигнала от лабела)
    dx=label->x()+QRepaintPixmap::geometry().left();
    dy=label->y()+QRepaintPixmap::geometry().top();
    trsColor=im->pixel(X-dx,Y-dy);
    ui->label_box->setAutoFillBackground(true);
    QPalette pal =ui->label_box->palette();
    pal.setColor(QPalette::Window, trsColor);
    ui->label_box->setPalette(pal);      
    bool f=true;
    for(int x=0;x<im->width();x++)
        for(int y=0;y<im->height();y++){
            rgb=im->pixel(x,y);
            if(campColor(rgb,trsColor.rgba(),ui->spinBox->value())){
                im->setPixel(x,y,qRgba(255,255,255,0));
                if(f){
                    trnsPoints.push_back(QPoint(X-dx,Y-dy));
                    f=false;
                }
            }
        }
    *tempPix=QPixmap::fromImage(*im);
    label->setPixmap(*tempPix);
    load();
    ui->pushButton_3->setEnabled(true);
}

void QRepaintPixmap::load()
{
    int h=0,s=0,v=0,a=0;
    for(int x=0;x<tempPix->width();x++)
        for(int y=0;y<tempPix->height();y++){
            rgb=im->pixel(x,y);
            clr.setRgba(rgb);
            clr.getHsv(&h,&s,&v,&a);
            tempPicture[x][y].h=h;
            tempPicture[x][y].s=s;
            tempPicture[x][y].v=v;
            tempPicture[x][y].al=a;
        }
}

bool QRepaintPixmap::campColor(QColor c1, QColor c2, int p)
{
    if(p>100 || p<0)return false;
    p=double(p)*2.55; // перевод процентов в размах значений уровня цвета
    return (diffInts(qRed(c1.rgb()),qRed(c2.rgb()))<=p &&
            diffInts(qGreen(c1.rgb()),qGreen(c2.rgb()))<=p &&
            diffInts(qBlue(c1.rgb()),qBlue(c2.rgb()))<=p);

}

int QRepaintPixmap::diffInts(int i1, int i2)
{
    int rt=i1-i2;
    if(rt<0)rt*=(-1);
    return rt;
}

void QRepaintPixmap::on_dial_valueChanged(int value)
{
    ui->spinBox->setValue(value);
}
