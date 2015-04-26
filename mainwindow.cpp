#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagesetup.h"
#include "avLabel.h"
#include "userlayout.h"
#include "about.h"
#include "captioneditor.h"
#include "texteditor.h"
#include "qdeformation.h"
#include <math.h>
#include <qmath.h>
#include "repaint.h"
#include "lighter.h"
#include "tofile.h"
#include "avbutton.h"
#include "refresh.h"

#include <QDesktopWidget>
#include <QPrinter>
#include <QPainter>
#include <QMovie>
#include <QThread>
#include <QPicture>
#include <QFileDialog>
//#include <QPrintDialog>
#include <QTimer>
//#include <QMatrix>
#include <QPointF>
#include <QSettings>
#include <QDrag>
#include <QMimeData>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QBitmap>
#include <QTextStream>
#include <QFont>
#include <QUrl>
#include <QMessageBox>
#include <QProcess>

#define PI 3.14159265

#include <iostream>
using namespace std;


/*
Каждый лист имеет свой размер бумаги, который задается в переменной int sheet[N].size,
где N=0,1,... - номер листа
ориентация листа указывается в переменной bool sheet[N].list_orn: true - портретная
Физические размеры в мм хранятся в переменных
    int     sheet[N].height - высота
    int     sheet[N].width  - ширина
*/


// глобальные переменные

QPrinter *printer;       // принтер, в него рисовать и настраивать
QPainter *pntr;          // устройство рисования (с него печатать)
QRepaintPixmap *repn;    // форма для корректировки цвета
lighter *lght;           // форма для управления освещением
PageSetup *ps;
TextEditor *txed;
about *ab;
userlayout *uslay;
QMenu *rmenu;
QMovie *animGif;
QTimer *timer;
captioneditor *cped;
QSettings setty("vap", "vap");
loadpicture *ldp;
QString str_time;
QavLabel *fon;
QavLabel *rez;          // рамка обрезки
Qdeformation *deform;
toFile *sfile;          // форма ввода параметров для сохранения в файл
refresh *rfsh;          // форма предложения обновления
int  vapIndex;        // индекс версии программы 0 = 3.8.2

//int oldAA=0; // раскомментировать, если задействована процедура управления прозрачностью (где-то на 3550 строке кода)

bool flag2;
int	 img_count=-1; 	    // количество превьюшек
int  cpt_count=-1;      // количество подписей на текущем листе
int	 comp = 0;			// текущая компоновка
int  getx = 0;			// максимальный размер х для картинки  в этой компоновке
int  gety = 0;			// максимальный размер y для картинки  в этой компоновке
double  prt_x= 210;     // x для портретной ориентации
int  gor = 420;			// w листа предпросмотра
int  ver = 594;			// h листа предпросмотра
int  pg_mar = 10;		// поля листа бумаги
int  pol = 10;			// поля между картинок mm
int  buf=-1; 		    // количество загруженных картинок, отсчет с нуля
int  ttx, tty;			// временные значения для отработки перемещения объектов
int  imgpress=-1;		// № нажатой превьюшки
int  bufpress=-1;		// № картинки в общем списке, превью которой нажато
int  imgpress2=-1;		// № нажатой превьюшки
int  bufpress2=-1;		// № картинки в общем списке, превью которой нажато
int  curz=0;            // z порядок для текущей страницы, используется для расчета наложений
int  lists=0, curlist=0;// всего листов и текущий лист; отсчет с 1, т.е. 0 - ничего нет
int  img_on_list;       // кол-во картинок на листе в текущей компоновке
double  ppx, ppy;       // точек на мм по Х и У для текущего предпросмотра
double  prx, pry;       // точек на мм по Х и У для текущего принтера
int  x_prw=150, y_prw=70; // координаты листа предпросмотра
bool rott_press=false;   // флаг нажатия на ручку вращения
bool prop=true;         // флаг пропорциональности при изменении размера
int  w_cou=1, h_cou=1;  // компоновка w_cou x h_cou
int  w_fon, h_fon;      // текущие размеры фона
int  t_comp;            // временно для хранения номера компоновки
bool out_rot=false;     // не вызывать слот поворота true - без поворота
bool fillsize=false;    // режим заполнения по максимальному
int  mem_pol=10;        // сохраненное значение полей между картинок pol
int  h_min_l=525;       // минимальная высота окна для ланшафтной ориентации
int  h_min_p=678;       // минимальная высота окна для портретной ориентации
double paper_ratio;     // пропорции бумаги h/w
QSize wind_sz;          // текущие размеры главного окна
int  gor_old;           // предыдущие размеры окна
int  offset=0;          // смещение кнопок компоновки при прокрутке
bool l, r, t, b;        // флаги захвата рамки обрезки
double list_scl=0;      // реальное соотношение размеров экрана и бумаги
bool pathFile=true;     // запоминать путь к последнему файлу и открывать диалоги там же
bool autoOrn=true;      // автоматический выбор ориентации бумаги
bool exitFlag=false;    // флаг выхода из функции, когда она вызывается, но выполнять ее не надо
bool fun=false;         // выдавать имена вызывемых функций в терминал
bool pdf=false;         // печать в pdf
int  imgFrame=-1;       // номер превьюшки, вокруг которой включена рамка. (-1) - нет такой.
avButton *prnBt1;       // кнопка печати в высоком качестве
avButton *prnBt2;       // кнопка печати в низком качестве
avButton *prnBt3;       // кнопка печати в черно-белом варианте

// поля бумаги
int left_m;
int right_m;
int top_m;
int bottom_m;
// размеры бумаги
int paper_w;
int paper_h;
bool rap;
bool all_rot;
// пользовательская компоновка
int ul_hor;
int ul_ver;
QString p_name;         // имя принтера
int sheet_size;
bool all_sizes;
bool set_orn;
bool print_color=true;  // печать в цвете
bool print_hi=true;     // печать в макс. качестве
QString list_n;
double h_ofsett;        // горизонтальное смещение позиции печати
int pap_sor;            // источник бумаги
int pap_name;
QString caption;
bool show_cap;
QColor font_cl;         // цвет шрифта подписи
QColor back_cl;         // цвет шрифта фона
QFont  font_cpt("Ubuntu"); // шрифт подписи
bool trans;             // прозрачный фон надписи
bool runShow=false;     // флаг разрешения работы процедуры show_pict - чтобы избежать бесконечной рекурсии
double font_scl;
bool testPrint;         // печать тестового креста (для проверки позиционирования)
bool noResizewindow=false; // Запрет на изменение размера окна программы
bool ctrl=false;        // нажата клавиша ctrl
bool flag3=true;        // флаг, запрещающий реакцию на изменение размера бумаги из ленты
QPoint wind_pos;        // координаты главного окна (после перемещения)
int  pgCou=1;           // количество копий, выводимых на печать

struct pict {                // загруженная картинка и все ее свойства
    QString   pict;          // путь к файлу
    int       rot;           // 0 - нормально, угол поворота
    QPixmap   pix0;          // исходный pixmap
    QPixmap   pix;           // текущий pixmap
    double    left;          // координата Х
    double    top;           // координата Y
    double    height;        // высота
    double    width;         // ширина
    bool      show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
    int       list;          // номер страницы этой картинки
    int       load;          // флаг того, картинка уже загружена в память
    int       z;             // z-порядок
    int       prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
    double    compress;      // коэф. компрессии для уже показанной картинки
    // caption
    QString   caption;       // подпись к картинке
    bool      show_caption;  // показывать подпись
    QColor    back_color;    // цвет фона
    bool      trans;         // прозрачный фон
    QColor    font_color;    // цвет шрифта
    QFont     font;          // шрифт
    double    leftCap;       // координата Х
    double    topCap;        // координата Y
    double    heightCap;     // высота
    double    widthCap;      // ширина
    int       cpt;           // номер подписи на экране к этой картинке
    int       cp_num;        // номер avLabel - подписи на экране
    int       cp_z;          // z-порядок для подписи
    QPixmap   cp_pixmap;     // pixmap подписи
    bool      isTextBlock;   // true - это текстовый блок
    int       alig;          // выравнивание в текстовом болоке 0-1-2 -> слева-по центру-справа
    // polygon -
//    bool      transformed;   // картинка была подвержена трансформации
//    QPoint    lTop;
//    QPoint    rTop;
//    QPoint    rBot;
//    QPoint    lBot;
};

struct prew {
    QavLabel     *pct;        //картинка
    int          buf;         //абсолютный номер
};

struct sheets {              // описание листа
    bool    list_orn;        // ориентация листа true -портретная
    int     size;            // размер листа - номер по списку размеров
    int     height;          // высота mm
    int     width;           // ширина mm
    // далее для печати только:
    int     sheet_h;         // высота области предпросмотра на экране
    int     sheet_w;         // ширина области предпросмотра на экране
};

sheets   curSheet;           // описание текущего листа

QavLabel *rott;              // "ручка" вращения
QavLabel *resiz;             // "ручка" изменения размера
QavLabel *delt;              // кнопка удаления картинки
QavLabel *clip;              // кнопка ножницы


std::vector<pict> toprint;      //массив загруженных картинок
std::vector<prew> toshow;       //массив превьющек
std::vector<prew> tocaption;    //массив подписей
std::vector<sheets> sheet;      // массив описаний листов


int c_load=0;                   // флаг загрузки донате
QNetworkAccessManager *manager;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  // конструктор
{
    if(fun)cout << "MainWindow (make window and widgets)" << endl;
    ui->setupUi(this);
    this->setAcceptDrops(true);
    vapIndex=0;
    fon=new QavLabel(ui->sheet);
    connect(fon, SIGNAL(mouse_press(int,int,int)), this, SLOT(sheetPress()));
    connect(fon, SIGNAL(mouse_wheel(int, int)), this, SLOT(mouseWheel(int, int)));
    fon->setImnum(-1);
    list_n.append("A4 210x297 mm");
    fon->show();
    all_sizes=true;
    set_orn = true;
    all_rot=true;
    ul_hor=3;
    ul_ver=3;
    h_ofsett=0;
    pap_sor=0;
    font_scl=1;
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    //printer = new QPrinter(); //QPrinter::HighResolution);
    pntr = new QPainter();
    rap=true;
    animGif = new QMovie(":/new/prefix1/indicator");
    ui->label->setMovie(animGif);
    // Скрыть элемeнты управления на период тормозов при загрузке 
    hide_interf();
    // кнопки по углам превьюшек:
    set_indic_pos();
    set_rott_btn();
    set_delt_btn();
    set_resiz_btn();
    set_clip_btn();
    set_timer();            // таймер, чтобы гасить кнопки
    rest_sind_size();       // восстановить размеры и положение окна программы
    rest_printer_sett();    // восстановить настройки принтера
    // стартовые параметры листа (по умолчанию)
    curSheet.size=pap_name;    
    curSheet.width=paper_w;
    curSheet.height=paper_h;
    curSheet.list_orn=true;
    sheet.push_back(curSheet);
    get_marg();
    rest_view_sett();       // восстановить компоновку
    ui->pushButton_32->hide();
    paper_ratio=double(paper_h)/double(paper_w);
    sheetPress();
    gor_old=gor;
    setty.beginGroup("Settings");
    pathFile=setty.value("path",true).toBool();
    autoOrn=setty.value("autoOrn", true).toBool();
    setty.endGroup();
    load_my_pSizes();
    flag3=false;
    // кнопки печати
    prnBt1=new avButton(ui->tab);
    prnBt1->setGeometry(10,0,44,38);
    prnBt1->setIcon(QIcon(QPixmap::fromImage(QImage(":/new/prefix1/vap"))));
    prnBt1->setIconSize(QSize(32,32));
    prnBt1->setToolTip(tr("Print"));
    connect(prnBt1,SIGNAL(whellUp()),this,SLOT(numCopUp()));
    connect(prnBt1,SIGNAL(whellDown()),this,SLOT(numCopDwn()));
    connect(prnBt1,SIGNAL(clicked()),this,SLOT(print1()));
    prnBt1->show();

    prnBt2=new avButton(ui->tab);
    prnBt2->setGeometry(60,0,44,38);
    prnBt2->setIcon(QIcon(QPixmap::fromImage(QImage(":/new/prefix1/vap_2"))));
    prnBt2->setIconSize(QSize(32,32));
    prnBt2->setToolTip(tr("Draft"));
    connect(prnBt2,SIGNAL(whellUp()),this,SLOT(numCopUp()));
    connect(prnBt2,SIGNAL(whellDown()),this,SLOT(numCopDwn()));
    connect(prnBt2,SIGNAL(clicked()),this,SLOT(print2()));
    prnBt2->show();

    prnBt3=new avButton(ui->tab);
    prnBt3->setGeometry(110,0,44,38);
    prnBt3->setIcon(QIcon(QPixmap::fromImage(QImage(":/new/prefix1/vap_black"))));
    prnBt3->setIconSize(QSize(32,32));
    prnBt3->setToolTip(tr("Black-and-white version"));
    connect(prnBt3,SIGNAL(whellUp()),this,SLOT(numCopUp()));
    connect(prnBt3,SIGNAL(whellDown()),this,SLOT(numCopDwn()));
    connect(prnBt3,SIGNAL(clicked()),this,SLOT(print3()));
    prnBt3->show();

    getVesion();

}

void MainWindow::numCopUp()
{
    if(pgCou<99) pgCou++;
    ui->spinBox->setValue(pgCou);
}

void MainWindow::numCopDwn()
{
    if(pgCou>1) pgCou--;
    ui->spinBox->setValue(pgCou);
}

void MainWindow::hide_interf()
{
    ui->tabWidget_2->setVisible(false);
    ui->pushButton_32->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_16->setGeometry(5,20,451,20);
}

void MainWindow::set_rott_btn() // создать кнопку вращения картинки
{
    rott = new QavLabel(fon);
    rott->setImnum(-1);
    rott->setPixmap(QPixmap::fromImage(QImage(":/new/prefix1/rotation")));
    rott->setScaledContents(true);
    rott->setGeometry(0,0,16,16);
    rott->setCursor(Qt::PointingHandCursor);
    rott->hide();
    rott->setToolTip("Rotate");
    connect(rott, SIGNAL(mouse_press(int, int, int)), this, SLOT(pct_press_rott()));
}

void MainWindow::set_delt_btn() // создать кнопку удаления картинки
{
    delt = new QavLabel(fon);
    delt->setImnum(-1);
    QImage im1(":/new/prefix1/delete");
    delt->setPixmap(QPixmap::fromImage(im1));
    delt->setScaledContents(true);
    delt->setGeometry(0,0,16,16);
    delt->setCursor(Qt::PointingHandCursor);
    delt->hide();
    delt->setToolTip(tr("Delete"));
    connect(delt, SIGNAL(mouse_press(int, int, int)), this, SLOT(pct_press_delete()));
}

void MainWindow::set_resiz_btn() // создать кнопку изм. размера картинки
{
    resiz = new QavLabel(fon);
    resiz->setImnum(-1);
    QImage im2(":/new/prefix1/resize");
    resiz->setPixmap(QPixmap::fromImage(im2));
    resiz->setScaledContents(true);
    resiz->setGeometry(0,0,16,16);
    resiz->setCursor(Qt::PointingHandCursor);
    resiz->hide();
    resiz->setToolTip("Resize");
    connect(resiz, SIGNAL(mouse_press(int, int, int)), this, SLOT(resiz_press(int, int)));
    connect(resiz, SIGNAL(mouse_move(int, int, int)),  this, SLOT(resiz_move(int, int)));
    connect(resiz, SIGNAL(mouse_up(int, int, int)), this, SLOT(resiz_up()));
}

void MainWindow::set_clip_btn()
{
    clip = new QavLabel(fon);
    clip->setImnum(-1);
    QImage im3(":/new/prefix1/rez");
    clip->setPixmap(QPixmap::fromImage(im3));
    clip->setScaledContents(true);
    clip->setGeometry(0,0,16,16);
    clip->setCursor(Qt::PointingHandCursor);
    clip->hide();
    clip->setToolTip("Clip");
    connect(clip, SIGNAL(mouse_press(int,int,int)), this, SLOT(show_clip()));
}

void MainWindow::set_timer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(istimer()));
}

void MainWindow::load_my_pSizes()
{
    flag3=true;
    int n1=ui->comboBox->currentIndex();
    ui->comboBox->clear();
    ui->comboBox->addItem("A4: 210 x 297 mm");
    setty.beginGroup("Paper sizes");
    bool go=true;
    QString s;
    int  i=0, index=0, n=0;
    while(go)
    {
        s=setty.value("name"+QString::number(i), "").toString();
        index=setty.value("index"+QString::number(i++), -1).toInt();
        if (index>0){
            ui->comboBox->addItem(s);
            if(pap_name==index)
            {
                n=ui->comboBox->count()-1;
                list_n=s;
            }
        }
        else  go=false;
    }
    setty.endGroup();
    if(pap_name==0){
        s="A4: 210 x 297 mm";
        n=0;
    }
    if(n1!=n)flag3=true;
    ui->comboBox->setCurrentIndex(n);
    flag3=true;
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // нажатие на контрол
    if(e->key()==16777249) ctrl=true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    // отпускание контрола
    if(e->key()==16777249) ctrl=false;
}

void MainWindow::mouseWheel(int i, int index) // пролистывание страниц колесиком мыши или изменение размера картинок
{
    imgpress2=index;
    if(ctrl) // нажат контрол - меняем размер
    {
       img_size_ch(i*4);
    }
    else // листаем листы
    {
        if(i>0)
        {
           on_pushButton_30_clicked();   // назад
        }
        else
        {
            on_pushButton_31_clicked(); // вперед
        }
    }
}

void MainWindow::img_size_ch(int step)
{
    // изменить размер текущей картинки
    if(imgpress2<0) return;
    bufpress2=toshow[imgpress2].buf;
    int x1, y1, cx, cy;
    int x3, y3, x4, y4;
    float k, k1;
    double d1, d2;
    QRect rc;
    rc=toshow[imgpress2].pct->geometry();
    QRect rc2=rc;
    k=toshow[imgpress2].pct->pixmap()->height();
    k1=toshow[imgpress2].pct->pixmap()->width();
    k=k/k1;
    rc.setWidth(rc.width()+step);
    rc.setHeight(rc.width()*k);
    rc.setLeft(rc.left()-step/2);
    rc.setTop(rc.top()-(step/2*k));
    toshow[imgpress2].pct->setGeometry(rc);
    x1=rc.left()-8;
    if(x1<0) x1=0;
    y1=rc.top()-8;
    if(y1<0) y1=0;
    rott->move(x1,y1);
    int rx, ry;
    rx=rc.left()-8+rc.width();
    if (rx>fon->width()) rx=fon->width()-16;
    ry=rc.top()-8;
    if (ry>fon->height()) ry=fon->height()-16;
    delt->move(rx,ry);
    d1=toprint[bufpress2].pix.width();
    d2=rc.width();
    toprint[bufpress2].compress=d1/d2;
    show_pict_size();
    cx=rc.left()-8;
    if(cx<8) cx=8;
    cy=rc.top()+rc.height()-8;
    if(cy>fon->height()-16) cy=fon->height()-16;
    clip->move(cx,cy);
    resiz->move(rx,cy);

    toprint[bufpress2].width=rc.width();
    toprint[bufpress2].height=rc.height();
    toprint[bufpress2].top=rc.top();
    toprint[bufpress2].left=rc.left();

    // begin caption
    x4=y4=0;
    if(rc2.top()<toprint[bufpress2].topCap) y4=rc2.height()-rc.height();
    if(rc2.left()<toprint[bufpress2].leftCap) x4=rc2.width()-rc.width();
    rc=getCaptionRect(bufpress2);
    x3=rc.left()-x4;
    y3=rc.top()-y4;
    int h=rc.height();
    int w=rc.width();
    rc.setLeft(x3);
    rc.setTop(y3);
    rc.setWidth(w);
    rc.setHeight(h);
    setCaptionRect(bufpress2, rc);
}

void MainWindow::setCaptionRect(int index, QRect r)
{
    toprint[index].leftCap=r.left();
    toprint[index].topCap=r.top();
    toprint[index].widthCap=r.width();
    toprint[index].heightCap=r.height();
}

QRect MainWindow::getCaptionRect(int index)
{
    return QRect(toprint[index].leftCap,toprint[index].topCap,toprint[index].widthCap,toprint[index].heightCap);
}

void MainWindow::setInterface()
{
    // Установить вид интерфейса программы в соответствии с настройками
    if(fun)cout << "setInterface" << endl;
    ui->tabWidget_2->setVisible(true);
    ui->tabWidget_2->setGeometry(1,1,651,71);
    ui->pushButton_32->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_16->setVisible(false);
    QRect rc=ui->sheet->geometry();
    rc.setLeft(1);
    ui->sheet->setGeometry(rc);
    ui->tabWidget_2->setCurrentIndex(0);
    make_list();
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    if(fun)cout << "resizeEvent" << endl;
    if(noResizewindow)
    {
        noResizewindow=false;
        return;
    }
    list_scl=double(ui->list->widthMM())/double(curSheet.width);
    gor_old=gor;
    wind_sz=e->size();
    make_list();
    layout_scale();
    show_pict();
    save_wind_size();
}

void MainWindow::moveEvent(QMoveEvent *e)
{
    wind_pos=e->pos();
    save_wind_size();
}

void MainWindow::closeEvent(QCloseEvent *cl)
{
    if(cl->Quit)qApp->closeAllWindows();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event) // сброс картинок мышью на программу
{
    cout << "was drag and drop..."<< endl;
    if (event->mimeData()->hasUrls())
    {
        prm.clear();
        prm.append("drops");
        for(int i=0;i<event->mimeData()->urls().count();i++)
        {
            prm.append(event->mimeData()->urls()[i].toString());
        }
        cou_prm=prm.count();
        if_show();
        return;
    }

    if (event->mimeData()->hasText())
    {
        QString dropFiles;
        dropFiles.append(event->mimeData()->text());
        prm.clear();
        prm.append("drops");
        cou_prm=1;
        QString s="";
        for(int i=0; i<dropFiles.length(); i++)
        {
            if(dropFiles.mid(i,1)!="\n")
            {
                s.append(dropFiles.mid(i,1));
            }
            else
            {
                prm.append(s);
                cout << "file: "<< s.toStdString() << endl;
                s.clear();
                cou_prm++;
            }
        }
        if(s.length()>0){
            prm.append(s);
            cout << "file: "<< s.toStdString() << endl;
            s.clear();
            cou_prm++;
        }
        if_show();
    }

}

void MainWindow::get_marg() // расчитать поля бумаги - область, недоступную для печати
{
    if(fun)cout << "get_marg" << endl;
    setty.beginGroup("Settings");
    if(setty.value("without_m", false).toBool())
    {
        left_m=right_m=top_m=bottom_m=0;
    }
    else
    {
        printer = new QPrinter(QPrinter::HighResolution);
        printer->setPaperSize(QPrinter::A4);
        QRect pap=printer->paperRect();
        QRect pag=printer->pageRect();
        left_m=right_m=double((pap.width()-pag.width())/2)/(double(printer->resolution())/25.4);
        top_m=bottom_m=double((pap.height()-pag.height())/2)/(double(printer->resolution())/25.4);
    }
    setty.endGroup();
}

void MainWindow::end_rotation() // Завершение поворота листа при смене орентации
{
    if(fun)cout << "end_rotation" << endl;
    for(int i=0;i<lists;i++){
        if(i==curlist-1 || all_rot){
            swap(sheet[i].height,sheet[i].width);
            sheet[i].list_orn=!sheet[i].list_orn;
        }
    }
    swap(paper_h, paper_w);
    swap(gor,ver);
    swap(curSheet.height,curSheet.width);
    curSheet.list_orn=!curSheet.list_orn;
    make_list();
    btn_comp_press(comp);
    if (all_rot)
    {
        if(!runShow)recomp();
    }
        else
    {
        if(!runShow)recomp_curlist();
    }
}

void MainWindow::make_list() // создать пустой лист предпросмотра
{
    if(fun)cout << "make_list" << endl;
    redraw();
    int x, y, w, h;
    ui->list->setGeometry(x_prw, y_prw, gor, ver);
    get_pp();
    x=x_prw+left_m*ppx;
    y=y_prw+top_m*ppy;
    w=gor-left_m*ppx-right_m*ppx;
    h=ver-top_m*ppy-bottom_m*ppy;
    ui->sheet->setGeometry(x, y, w, h);
    fon->setGeometry(0, 0, w, h);
    w_fon=w;
    h_fon=h;
    set_indic_pos();
}

void MainWindow::redraw()
{
    // изменение размера окна программы
    // расчет окна предпросмотра
    if(fun)cout << "redraw" << endl;
    double x, y, w, h;                 //будущий рект окна
    y=curSheet.height;
    x=curSheet.width;
    int lft=150;
    lft=10;
    paper_ratio=x/y;
    w=x=wind_sz.width()-(lft+5);       //допустимая область для размещения
    h=y=wind_sz.height()-75;           // --
    if (w/paper_ratio>h) w=h*paper_ratio;
    else h=w/paper_ratio;
    x=(x-w)/2.0;
    y=(y-h)/2.0;
    x_prw=x+lft;
    y_prw=((wind_sz.height()-75)-h)/2+73;
    gor=w;
    ver=h;
}

void MainWindow::layout_scale()
{
    if(fun)cout << "layout_scale" << endl;
    if(gor_old==gor) return;
    double k=double(gor)/double(gor_old);
    for(int i=0;i<=buf;i++){
        if(toprint[i].show){
            toprint[i].left=k*toprint[i].left;
            toprint[i].top=k*toprint[i].top;
            toprint[i].height=k*toprint[i].height;
            toprint[i].width=k*toprint[i].width;
            toprint[i].leftCap=toprint[i].leftCap*k;
            toprint[i].topCap=toprint[i].topCap*k;
            toprint[i].widthCap=toprint[i].widthCap*k;
            toprint[i].heightCap=toprint[i].heightCap*k;
        }
    }
    gor_old=gor;
    img_size_cur_comp();
}

void MainWindow::get_pp() // получить количество точек на мм для текущего предпросмотра
{
    if(fun)cout << "get_pp" << endl;
    if(sheet[curlist-1].list_orn)
    {
        ppx=double(ui->list->width())/double(curSheet.width);
        ppy=double(ui->list->height())/double(curSheet.height);
    }
    else
    {
        ppx=double(ui->list->width())/double(curSheet.height);
        ppy=double(ui->list->height())/double(curSheet.width);
    }
}

void MainWindow::set_wind_size()
{
    if(fun)cout << "set_wind_size" << endl;
    int w,h;
    if (paper_h>paper_w) h=h_min_p;
    else h=h_min_l;
    w=x_prw+ui->list->geometry().width()+10;
    this->resize(w,h);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::if_show()
{
    if(fun)cout << "if_show" << endl;
    ind_start();
    load_param();
    ind_stop();
    setInterface();
}



void MainWindow::load_param() // обработка параметров командной строки
{
    if(fun)cout << "load_param" << endl;
    if (cou_prm<2) return;
    QString fn;
    QFileInfo fi;
    bool fl_no;
    for(int i=1;i<cou_prm; i++)
     {

        flag2=true;
        fl_no=true;
        if (rap) cout << "File to load " << i << " : " << prm[i].toStdString() << endl;
        fn=fn.append(prm[i].trimmed());

        if (fn.contains("file:///")) {
            fn=esc_to_utf(fn);
            fn=fn.right(fn.length()-7);
        }
        if (fn.contains("smb://")) fn=esc_to_utf(fn);

        if(i+1<cou_prm)
        {
            if((prm[i+1].mid(0,1)!="/") && ((prm[i+1].mid(0,8)!="file:///"))
                    && ((prm[i+1].mid(0,8)!="smb://")))
            {
                fl_no=false;
                fn.append(" ");
            }
        }
        if (fl_no)
        {
            fi.setFile(fn);
            //cout << "fn: " << fn.toStdString() << endl;
            if(fi.isFile())
            {
                cout << "I think it's a file..." << endl;
                open_pct(fn);
            }
            else //папка
            {
                cout << "I think it's a folder..." << endl;
                load_folder(fn);
            }
            while (flag2) { QApplication::processEvents(); }
            fn.clear();
        }
     }
    show_pict();
}


void MainWindow::load_folder(QString fn)
{
    if(fun)cout << "load_folder" << endl;
    QDir dir(fn);
    if(!dir.exists()){
        cout << "Oh no. It's something else..."<<endl;
        flag2=false;
        return;
    }
    QStringList nameFilter; // имя фильтра
    nameFilter << "*.png" << "*.xpm" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif" << "*.ico";
    QFileInfoList list = dir.entryInfoList( nameFilter, QDir::Files );// только файлы
    QFileInfo fileinfo; // список
    foreach (fileinfo, list)
    {
        flag2=true;
        open_pct(fileinfo.absoluteFilePath());
        while (flag2) { QApplication::processEvents(); }
    }
}

QString get_name(QString s)
{
    if(fun)cout << "get_name" << endl;
    QString ts="";
    for (int i=s.length()-1; i>0; i--)
    {
        if(s[i]!='/') ts.push_front(s[i]);
                else return ts;
    }
    return ts;
}

void MainWindow::open_pct(QString filename) // добавить картинку в список
{
    if(fun)cout << "open_pct" << endl;
    if (filename.contains("file:///")) filename=esc_to_utf(filename);
    if (filename.contains("smb://")) filename=esc_to_utf(filename);
    if (rap) cout << "File to open: " <<  filename.toStdString() << endl;
        buf++;
        if(lists==0)
        {
            lists=1;
            curlist=1;
        }
        str_time.clear();
        if (filename.contains("smb://")){
            QProcess proc;
            QString s;
            s.append("cp ");
            s.append(filename);
            s.append(" /home/and/1.jpg");
            proc.start(s);
            proc.waitForFinished(-1);
            filename.clear();
            filename.append("/home/and/1.jpg");
        }
        str_time.append(filename);
        toprint.resize(buf+1);
        toprint[buf].pict=filename;
        toprint[buf].caption=get_name(filename);
        toprint[buf].back_color=Qt::white;    // цвет фона
        toprint[buf].font_color=Qt::black;    // цвет шрифта
        toprint[buf].trans=true;
        toprint[buf].isTextBlock=false;
        QFont fn;
        fn.setPointSize(10);
        fn.setFamily("Sans");
        toprint[buf].font=fn;                 // шрифт
        toprint[buf].cp_num=-1;               // нет привязанного avLabel
        toprint[buf].show_caption=false;      // без подписи
        setCaptionRect(buf, QRect(0,0,0,0));  // начальная геометрия подписи
        QThread *thread = new QThread;
        ldp = new loadpicture;
        ldp->moveToThread(thread);

        connect (thread, SIGNAL(started()), SLOT(start_load_picture()));
        connect (ldp, SIGNAL(sendImage(QImage)), SLOT(end_load_picture(QImage)));
        thread->start();
}

// между этими двумя слотами работает класс loadpicture в отдельном потоке

void MainWindow::end_load_picture(QImage image)
{
    if(fun)cout << "end_load_picture" << endl;
    if (image.isNull()) { cout << ":( <- loading" << endl;} // неудачная загрузка
        double d;
        toprint[buf].pix0 = toprint[buf].pix = QPixmap::fromImage(image);
        if(autoOrn && buf==0) setAutoOrn();
        toprint[buf].show=false;
        d=buf+1;
        d=ceil(d/img_on_list);
        toprint[buf].list=d;
        if (comp==10)toprint[buf].list=1;
        lists=0;
        for(int i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
        if (rap) cout << "picture was loaded" << endl;
        img_size_cur_comp();
        flag2=false;
}


void MainWindow::setAutoOrn() // Автоматически установить ориентацию бумаги
{    
    if(fun)cout << "setAutoOrn" << endl;
    if(toprint[0].pix.height()>=toprint[0].pix.width())
    {
        // портретная ориентация
        if(!sheet[0].list_orn) end_rotation();
    }
    else
    {
        // ландшафтная ориентация
        if(sheet[0].list_orn) end_rotation();
    }

}

//  class loadpicture

loadpicture::loadpicture(QObject *parent) :  QObject(parent)
{
    emit start();
}

void loadpicture::start_load(QString filename) // start load
{
    filename=filename.trimmed();
    QImage image(filename);
    emit sendImage(image);
}


// end

void MainWindow::on_checkBox_7_clicked(bool checked) // общая ориентация для всех листов
{
    if(fun)cout << "on_checkBox_7_cliked" << endl;
    if(exitFlag)return;
    all_rot=checked;
    if(ps!=0) ps->set_all();
    if (checked)
    {
        for(int i=0; i<=buf; i++)  if (sheet[toprint[i].list].list_orn != curSheet.list_orn) toprint[i].show=false;
        for(int i=0; i<lists; i++) sheet[i].list_orn=curSheet.list_orn;
    }
}

void MainWindow::set_all_rot(bool b)
{
   if(fun)cout << "set_all_rot" << endl;
   all_rot=b;
}

double MainWindow::get_scaleX()
{
   if(fun)cout << "get_scaleX" << endl;
   double x_size;
   double x_prew;
   QRect rc=printer->paperRect(); // pageRect();
   x_size=rc.width();
   x_prew=ui->sheet->width();
   return x_size/x_prew;
}

double MainWindow::get_scaleY()
{
   if(fun)cout << "get_scaleY" << endl;
   double y_size;
   double y_prew;
   QRect rc=printer->paperRect(); // pageRect();
   y_size=rc.height();
   y_prew=ui->sheet->height(); // fon->height();
   return y_size/y_prew;
}

QPageSize MainWindow::GetPageSize(int i)
{
    switch (sheet[i].size)
    {
        case 5: return QPageSize(QPageSize::A0);
        case 6: return QPageSize(QPageSize::A1);
        case 7: return QPageSize(QPageSize::A2);
        case 8: return QPageSize(QPageSize::A3);
        case 0: return QPageSize(QPageSize::A4);
        case 9: return QPageSize(QPageSize::A5);
        case 10: return QPageSize(QPageSize::A6);
        case 11: return QPageSize(QPageSize::A7);
        case 12: return QPageSize(QPageSize::A8);
        case 13: return QPageSize(QPageSize::A9);
        case 14: return QPageSize(QPageSize::B0);
        case 15: return QPageSize(QPageSize::B1);
        case 17: return QPageSize(QPageSize::B2);
        case 18: return QPageSize(QPageSize::B3);
        case 19: return QPageSize(QPageSize::B4);
        case 1: return QPageSize(QPageSize::B5);
        case 20: return QPageSize(QPageSize::B6);
        case 21: return QPageSize(QPageSize::B7);
        case 22: return QPageSize(QPageSize::B8);
        case 23: return QPageSize(QPageSize::B9);
        case 16: return QPageSize(QPageSize::B10);
        case 24: return QPageSize(QPageSize::C5E);
        case 25: return QPageSize(QPageSize::Comm10E);
        case 26: return QPageSize(QPageSize::DLE);
        case 4: return QPageSize(QPageSize::Executive);
        case 27: return QPageSize(QPageSize::Folio);
        case 28: return QPageSize(QPageSize::Ledger);
        case 3: return QPageSize(QPageSize::Legal);
        case 2: return QPageSize(QPageSize::Letter);
        case 29: return QPageSize(QPageSize::Tabloid);
        default:
            QSizeF szf;
            if(sheet[i].list_orn)szf=QSizeF(sheet[i].width,sheet[i].height);
            else szf=QSizeF(sheet[i].height,sheet[i].width);
            QPageSize ps(szf,QPageSize::Millimeter,
                         "Custom ("+QString::number(sheet[i].width)+" x "+QString::number(sheet[i].height)+")",
                         QPageSize::ExactMatch);
            return ps;
        }
}

void MainWindow::set_printer(int index) // настройка принтера для листа index
{
     if(fun)cout << "set_printer" << endl;
     if(!pdf)printer->setDocName("vap_pictures_"+QString::number(index));
     // установить размер бумаги для этого листа
     setPrinterPage(index);
     // физическое разрешение принтера по x и y (точек на мм)
     prx=printer->physicalDpiX()/25.4;
     pry=printer->physicalDpiY()/25.4;
}

void MainWindow::prePint()
{
    // прогон листов перед печатью
    if(fun)cout << "prePrint" << endl;
    int j=curlist;
    for(int i=1; i<=lists; i++)
    {
        for(int p=0; p<=buf; p++) toprint[p].cp_num=-1;
        curlist=i;
        if (int(sheet.size())<lists) sheet.push_back(sheet.back());
        curSheet=sheet[i-1];
        show_pict();
        sheet[i-1].sheet_w=ui->sheet->width();
        sheet[i-1].sheet_h=ui->sheet->height();
    }
    for(int p=0; p<=buf; p++) toprint[p].cp_num=-1;
    curlist=j;
    curSheet=sheet[j-1];
    show_pict();
}

void MainWindow::setPrinterPage(int i)
{ // Размер бумаги указан в свойствах каждого листа, однако фактически он берется из настроек первого листа.
  // Возможность иметь разные размеры заложена, но не реализована, т.к. маловероятно, что кто-то будет
  // вкладывать в принтер подряд несколько разных листов бумаги.
  // Начиная с этой версии (3.8) будет поддержка только Qt>=5.3, т.к. более старшие
  // не поддерживают классы QPageSize и QPageLayout
    QPageLayout pL;
    pL.setPageSize(GetPageSize(0));
    pL.setMode(QPageLayout::FullPageMode);
    if(sheet[i].list_orn){
        pL.setOrientation(QPageLayout::Portrait);
        printer->setOrientation(QPrinter::Portrait);
    }else{
        pL.setOrientation(QPageLayout::Landscape);
        printer->setOrientation(QPrinter::Landscape);
    }
    printer->setPageLayout(pL);
}

void MainWindow::setPrinter()
{
    // создать принтер и указать его качество печати
    if (print_hi) printer = new QPrinter(QPrinter::HighResolution);
        else printer = new QPrinter();
    // без полей
    printer->setFullPage(true);
    // в файл или на реальный принтер
    if(!pdf){
        printer->setPrinterName(p_name);
        printer->setOutputFileName("");
        printer->setOutputFormat(QPrinter::NativeFormat);
        cout << "printer: " << p_name.toStdString() << endl;
    }else{
        QString user=getenv("HOME");
        if(pathFile)
        {
            setty.beginGroup("Settings");
            user=setty.value("inPath",user).toString();
            setty.endGroup();
        }
        QString fileName = QFileDialog::getSaveFileName
                (this, tr("Print to PDF file..."), user, tr("pdf-file (*.pdf)"));
        if(fileName.isEmpty()) return;
        if(fileName.right(4)!=".pdf")fileName.append(".pdf");
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(fileName);
    }
    // настройка цвета
    if (print_color) printer->setColorMode(QPrinter::Color);
        else printer->setColorMode(QPrinter::GrayScale);
}

void MainWindow::print1() //печать в высоком качестве
{
    print_color=true;
    print_hi=true;
    for(int i=1;i<=pgCou;i++) printAll();
}

void MainWindow::printAll() // собственно печать
{
    if(fun)cout << "on_pushButton_29_cliked (printing)" << endl;
    prePint();          // прогон листов перед печатью
    QRect rc;           // пригодится ниже...
    QBrush br;          // кисть
    QPixmap pxm;        // пиксмэп для печати
    double sclX, sclY;  // коэф. масштабирования между бумагой и предросмотром при пересчете размеров и координат
    double px=0,py=0,ph,pw;
    double dx, dy;      // Смещение области печати от края листа бумаги
    bool f;
    // общая настройка принтера
    setPrinter();   
    for (int i=0; i<lists; i++)
    {
        set_printer(i);             // дополнительно настроить принтер для очередного листа
        if(pdf && i!=0)printer->newPage();
        rc=printer->paperRect();    // размер бумаги
        if(i==0 || !pdf)f=pntr->begin(printer);
        px=0;py=0;
        if(!f)   // начать рисование
        {
            cout << "Attempt to access the printer caused the error." << endl;
            QMessageBox::critical(NULL,QObject::tr("Error"),tr("Printer not found! Check the settings and printer availability."));
            return;
        }
        cout << "pntr->viewport().width()="<< pntr->viewport().width() << endl;
        cout << "pntr->viewport().height()="<< pntr->viewport().height() << endl;
        // Расчет смещения области печати от края листа
        qreal left=0, top=0, right=0, bottom=0, b2=0;
        printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::DevicePixel);
        cout << "margins: " << left << ", " << top << ", " << right << ", " << bottom << endl;
        dx=left; dy=top;
        // масштаб между предпросмотром и бумагой (в пикселах)
        if(pdf)if(sheet[i].list_orn!=sheet[0].list_orn)swap(sheet[i].sheet_w,sheet[i].sheet_h);
        sclX=double(pntr->viewport().width())/sheet[i].sheet_w;
        sclY=double(pntr->viewport().height())/sheet[i].sheet_h;
        int hh=rc.height();   // высота облаcти печати
        int ww=rc.width();    // ширина области печати
        cout << "page left = " << rc.left() << " top = "<< rc.top() << endl;
        cout << "dx = " << dx << "; dy = " << dy << endl;
        cout << "sclX = " << sclX << "; sclY = " << sclY << endl;
        cout << "Page: rect size: height = "<< rc.height() << "; width = " << rc.width() << endl;

        double ppx=px, ppy=py;
        for(int j=0; j<=buf; j++)
        {
            if (toprint[j].list==i+1)
            {
                cout << "coordinates on the screen: x="<< toprint[j].left<<" y="<<toprint[j].top<<endl;
                    px=ppx+double(toprint[j].left)*sclX;
                    py=ppy+double(toprint[j].top)*sclY;
                    ph=double(toprint[j].height)*sclY;
                    pw=double(toprint[j].width) *sclX;
                    cout << "coordinates on the paper: x="<< px<<" y="<<py<<endl;
                    rc=QRect (px,py,pw,ph); // размер на бумаге
                    pxm=toprint[j].pix.scaled(pw, ph, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //картинка в разрешении принтера
                    if(py<dy) // обрезать верх, если он выходит за пределы
                    {
                        ph=ph-dy+py;
                        pxm=pxm.copy(0,dy-py,pw,ph);
                        py=dy;
                        rc.setTop(py);
                        rc.setHeight(ph);
                     }
                    if((hh-dy-b2)<(py+ph)) // картинку надо подрезать снизу, чтобы не вызвать цветного мусора
                    {
                        ph=hh-py-dy-1-b2;
                        pxm=pxm.copy(0,0,pw,ph);
                        rc.setHeight(ph);
                    }
                    if(px<dx) // обрезать картинку слева - торчит
                    {
                        pw=pw-dx+px;
                        pxm=pxm.copy(dx-px,0,pw,ph);
                        px=dx;
                        rc.setLeft(px);
                        rc.setWidth(pw);
                    }
                    if((ww-dx)<(px+pw))  // картинка торчит справа, надо обрезать
                    {
                        pw=ww-px-dx-1;
                        pxm=pxm.copy(0,0,pw,ph);
                        rc.setWidth(pw);
                    }
                    pntr->drawPixmap(rc, pxm, pxm.rect());
                    //caption
                    if(toprint[j].show_caption || ui->checkBox_12->isChecked())
                        if(toprint[j].widthCap!=0)
                        {
                            pntr->setFont(toprint[j].font);
                            rc.setTop(toprint[j].topCap*sclY);
                            rc.setLeft(toprint[j].leftCap*sclX);
                            rc.setWidth(toprint[j].widthCap*sclX);
                            rc.setHeight(toprint[j].heightCap*sclY);
                            br.setColor(toprint[j].font_color);
                            if(!toprint[j].trans)
                            {
                                br.setColor(toprint[j].back_color);
                                pntr->setBrush(br);
                                pntr->setBackgroundMode(Qt::OpaqueMode);
                                pntr->setBackground(br);
                                pntr->setBrush(Qt::SolidPattern);
                                pntr->drawRect(rc);
                            }

                            pntr->setPen(toprint[j].font_color);
                            pntr->drawText(rc, Qt::AlignLeft, toprint[j].caption);
                        }
             }
        }
        if(i==lists-1 || !pdf)pntr->end(); // отправить на печать
    }
   cout << "end printing" << endl;
   printer->~QPrinter(); // уничтожить экземпляр принтера
}

void MainWindow::start_load_picture()
{
    if(fun)cout << "start_load_picture" << endl;
    ldp->start_load(str_time);
}

void MainWindow::pct_press(int x, int y, int i) // нажатие на превьюшку
{
    if(fun)cout << "pct_press" << endl;
    if(rez!=0) rez->hide();
    toshow[i].pct->setCursor(Qt::ClosedHandCursor); // поменять курсор
    toshow[i].pct->raise(); // поднять выше всех
    toprint[toshow[i].buf].z=++curz;
    if(toprint[toshow[i].buf].show_caption || ui->checkBox_12->isChecked())
    {
        tocaption[toprint[toshow[i].buf].cp_num].pct->raise();
        toprint[toshow[i].buf].cp_z=++curz;
    }
    ttx=x;
    tty=y;
    imgpress=i;
    imgpress2=i;
    showPctBord(true);
    bufpress=toshow[i].buf;
    if (!ui->checkBox_9->isChecked()) return; // Иконки быстрого редактирования запрещены
    bufpress2=bufpress;
    int rx, ry, dy, cx;
    rx=toprint[bufpress].left-8;
    if (rx<0) rx=0;
    ry=toprint[bufpress].top-8;
    if (ry<0) ry=0;
    rott->show();
    rott->move(rx,ry);
    cx=rx;
    dy=ry;
    rott->raise();
    rx=toprint[bufpress].left-8+toprint[bufpress].width;
    if (rx>fon->width()) rx=fon->width()-16;
    ry=toprint[bufpress].top-8+toprint[bufpress].height;
    if (ry>fon->height()) ry=fon->height()-16;
    resiz->show();
    resiz->move(rx,ry);
    resiz->raise();
    delt->show();
    delt->move(rx,dy);
    delt->raise();
    clip->move(cx, ry);
    clip->raise();
    clip->show();
    out_rot=true;
    ui->dial_4->setValue(toprint[bufpress].rot);
    out_rot=false;
    show_pict_size();
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
}

void MainWindow::pct_move(int x, int y, int i)
{
        //if(fun)cout << "pct_move" << endl;
        int x1=0, y1=0, dy, cx, x2=0, y2=0;
        x1=x2=ttx;
        y1=y2=tty;
        ttx = x;
        tty = y;
        QRect rc=getCaptionRect(bufpress);
        x2=rc.left()+ttx-x2;
        y2=rc.top()+tty-y2;
        int w=rc.width();
        int h=rc.height();
        rc.setLeft(x2);
        rc.setTop(y2);
        rc.setWidth(w);
        rc.setHeight(h);
        setCaptionRect(bufpress,rc);
        if(toprint[bufpress].show_caption || ui->checkBox_12->isChecked())
        {
            tocaption[toprint[bufpress].cp_num].pct->move(x2,y2);
        }
        x1=toprint[bufpress].left+ttx-x1;
        y1=toprint[bufpress].top+tty-y1;
        toshow[i].pct->move(x1,y1);
        toprint[bufpress].left=x1;
        toprint[bufpress].top=y1;
        dy=y1-8;
        rott->move(cx=x1-8, dy);
        x1=x1+toprint[bufpress].width-8;
        y1=y1+toprint[bufpress].height-8;
        resiz->move(x1,y1);
        delt->move(x1,dy);
        clip->move(cx,y1);
}

void MainWindow::pct_up(int x, int y, int i)
{
    if (i==imgpress)
    {
        imgpress=-1;
        bufpress=-1;
        toshow[i].pct->setCursor(Qt::OpenHandCursor);
        timer->start(3000);
        x=y;
        y=x;
    }
}

void MainWindow::istimer()
{
    timer->stop();
    rott->hide();
    resiz->hide();
    delt->hide();
    clip->hide();
}

void MainWindow::recomp_curlist()
{
    QRect rc;
    for(int i=0; i<=buf; i++)
        if (toprint[i].list==curlist){
            toprint[i].show=false;
            toprint[i].cp_num=-1;
            rc.setRect(0,0,0,0);
            setCaptionRect(i,rc);
        }
    show_pict();
}


void MainWindow::set_setting(int r) // применить настройки
{
    if(fun)cout << "set_setting" << endl;
    if(rap)cout << "set new settings:"<< r << endl;
    if(r==0)return;
    if(r==3){if(!curSheet.list_orn)end_rotation();return;}
    if(r==4){if(curSheet.list_orn)end_rotation();return;}
    load_my_pSizes();
    if(r==1)return;
    save_printer_sett();
    curSheet.size=sheet_size;
    curSheet.height=paper_h;
    curSheet.width=paper_w;
    curSheet.list_orn=(paper_h>paper_w);
    if (all_sizes)for(int i=0; i<int(sheet.size()); i++)sheet[i]=curSheet;
    else sheet[curlist-1]=curSheet;
//    if(curSheet.list_orn)on_pushButton_33_clicked(); // портретная ориентация
//    else      on_pushButton_34_clicked(); // ландшафтная ориентация
    setty.beginGroup("Settings");
        pathFile=setty.value("path", false).toBool();
    setty.endGroup();   
    show_pict();
}

QString MainWindow::get_file()
{
    QString hm="/home";
    if(pathFile)
    {
        setty.beginGroup("Settings");
        hm=setty.value("inPath","/home").toString();
        setty.endGroup();
    }
    return QFileDialog::getOpenFileName(this, tr("Open File"),
            hm, tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.ico)"));
}

void MainWindow::ind_start()
{
    ui->label->show();
    ui->label->raise();
    animGif->start();
}

void MainWindow::ind_stop()
{
    animGif->stop();
    ui->label->hide();
}

int mmToPixV(int mm) // перевод мм в пиксели предпросмотра
{
    return mm*(float(h_fon)/float(paper_h));
}

int mmToPixG(int mm) // перевод мм в пиксели предпросмотра
{
    return mm*(float(w_fon)/float(paper_w));
}

int getmx()
// получить макимальный размер Х для текущей компоновки
{
    return(w_fon-mmToPixG(pol))/w_cou-mmToPixG(pol);

}

int getmy()
// получить макимальный размер Y для текущей компоновки
{
 return (h_fon-mmToPixV(pol))/h_cou-mmToPixV(pol);
}

// размеры для текущей компоновки
void MainWindow::img_size_cur_comp()
{
    getx=getmx();
    gety=getmy();
}

void MainWindow::btn_comp_press(int i)
{
    comp=i;
    img_size_cur_comp ();
}

void MainWindow::set_user_layout()
{
    if(fun)cout << "set_use_layout" << endl;
    img_on_list=ul_hor*ul_ver;
    w_cou=ul_hor;
    h_cou=ul_ver;
    setOrnSheets (set_orn);
    btn_comp_press(11);
    recomp();
    ui->checkset_2->setGeometry(442,1,16,16);
    ui->label_17->setText(tr("Select: ")+QString::number(w_cou) + "x" + QString::number(h_cou));
}

void MainWindow::setOrnSheets(bool orn)
{
    if(curSheet.list_orn != orn) end_rotation();
}

void MainWindow::set_indic_pos()
{
    int x,y;
    x=(ui->sheet->geometry().width()-128)/2;
    y=(ui->sheet->geometry().height()-128)/2;
    ui->label->setGeometry(x,y,128,128);
}


int max_z(int l) // максимальное значение z-порядка для листа l
{
    int c=0;
    for (int i=0; i<=buf; i++)
        if (toprint[i].list==l)
        {
            if (toprint[i].z>c) c=toprint[i].z;
            if (toprint[i].cp_z>c) c=toprint[i].cp_z;
        }
    return c;
}

QSize setsize(QSize sz) // Точная подгонка размера image под место
{
  float x=0, y=0, e=0;
  x=sz.height();
  y=sz.width();
  if (x * y == 0) return sz;
  e=x/y ;
  y=gety;
  x=y/e;
  if(x>getx)
    {
        x=getx;
        y=x*e;
    }
  sz.setHeight(y);
  sz.setWidth(x);
  return sz;
}

void MainWindow::kill_pict()
{
    if (rap) cout << "clear preview page" << endl;
    for(int i=0; i<=img_count; i++) toshow[i].pct->deleteLater();
    for(int i=0; i<=cpt_count; i++)
    {
        tocaption[i].pct->deleteLater();
        toprint[tocaption[i].buf].cp_num=-1;
    }
    img_count=-1;
    cpt_count=-1;
    curz=0;
}

void MainWindow::set_ornt_list()
// повернуть лист (при пролистывании)
{
    if(fun)cout << "set_orn_list" << endl;
    if(curSheet.list_orn==sheet[curlist-1].list_orn)return;
    make_list(); // создать новый лист
    setOrnSheets(sheet[curlist-1].list_orn);
}

void MainWindow::show_pict() // показать картинки текущего листа
{
    if((buf<0)||runShow) return;
    kill_pict(); // очистить лист предпросмотра
    runShow=true;
    make_list(); // создать новый лист
    if (buf>-1) quick_buttons_off();
    QSize sz;
    double d1, d2;
    int x, y, n=1;
    if(comp==1) n=2;
    curz=max_z(curlist);

    for (int i=0; i<=buf; i++)
    {
      if (toprint[i].list==curlist)
      {   // создать превью
          img_count++;
          toprint[i].prew=img_count;
          toshow.push_back(prew());
          toshow[img_count].buf=i;
          toshow[img_count].pct = new QavLabel(fon);
          toshow[img_count].pct->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
          toshow[img_count].pct->setScaledContents(true);
          toshow[img_count].pct->setPixmap(toprint[i].pix);
          toshow[img_count].pct->setCursor(Qt::OpenHandCursor);
          toshow[img_count].pct->setImnum(img_count);
          toshow[img_count].pct->setAttribute(Qt::WA_DeleteOnClose);
          toshow[img_count].pct->show();
          QObject::connect(toshow[img_count].pct, SIGNAL(mouse_press(int, int, int)),
                       this, SLOT(pct_press(int, int, int)));
          QObject::connect(toshow[img_count].pct, SIGNAL(mouse_move(int, int, int)),
                       this, SLOT(pct_move(int, int, int)));
          QObject::connect(toshow[img_count].pct, SIGNAL(mouse_up(int, int, int)),
                       this, SLOT(pct_up(int, int, int)));
          QObject::connect(toshow[img_count].pct, SIGNAL(rclicked(int, int, int)),
                       this, SLOT(show_r_menu(int,int,int)));
          QObject::connect(toshow[img_count].pct, SIGNAL(mouse_wheel(int, int)),
                       this, SLOT(mouseWheel(int, int)));
          if (!toprint[i].show)
            {   // картинка рисуется первый раз
                sz=setsize(toshow[img_count].pct->pixmap()->size());
                if (img_count<(w_cou*h_cou) and comp!=10)
                {
                    if(getx==sz.width()){
                        x=(img_count%w_cou)*(w_fon/w_cou)+mmToPixG(pol);
                    } else {
                        x=(img_count%w_cou)*(w_fon/w_cou)+(getx-sz.width())/2;
                    }
                    if(gety==sz.height()){
                        y=ceil(img_count/w_cou)*(h_fon/(h_cou*n))+mmToPixV(pol);
                    } else {
                        y=ceil(img_count/w_cou)*(h_fon/(h_cou*n))+(gety-sz.height())/2;
                    }
                }
                else //подбор координат для "лишних" картинок
                {
                    x=w_fon;
                    while (x>=sz.width()) x=rand()%(fon->width());
                    y=h_fon;
                    while (y>=sz.height()) y=rand()%(fon->height());
                }
                if (comp==0 && fillsize) // заполнение при необходимости
                {
                    fill_all();
                    y=(h_fon-toprint[i].height)/2;
                    x=(w_fon-toprint[i].width)/2;
                    sz.setWidth(toprint[i].width);
                    sz.setHeight(toprint[i].height);
                }
                toshow[img_count].pct->setGeometry(x, y, sz.width(), sz.height());
                d1=toprint[i].pix.width();
                d2=sz.width();
                toprint[i].compress=d1/d2;
                toprint[i].left=x;
                toprint[i].top=y;
                toprint[i].z=++curz;
                toprint[i].show=true;
                toprint[i].width=sz.width();
                toprint[i].height=sz.height();
              }
        else
          {  // картинка уже была нарисована
              x=toprint[i].left;
              y=toprint[i].top;
              toshow[img_count].pct->setGeometry(x, y,toprint[i].width, toprint[i].height);
              toprint[i].compress=double(toprint[i].pix.width())/double(toprint[i].width);
          }
          show_caption(i);
      }
    }

    set_z();
    QString s, c;
    s.append(tr("Sheet "));
    c = QString::number(curlist);
    s.append(c);
    s.append(tr(" of "));
    c = QString::number(lists);
    s.append(c);
    ui->label_2->setText(s);
    if (rap) cout << "pictures was shown" << endl;
    runShow=false;
}

void MainWindow::set_z()
{
    // определяем глубину
    curz=max_z(curlist);
    //поднимаем-опускаем
    for(int i=0; i<=curz; i++)
        for(int j=0; j<=buf; j++)
        {
            if(toprint[j].list==curlist && toprint[j].z==i)
                toshow[toprint[j].prew].pct->raise(); // поднять выше всех -image
            if(toprint[j].list==curlist && toprint[j].cp_z==i && !toprint[j].isTextBlock &&
                   (toprint[j].show_caption || ui->checkBox_12->isChecked()) )
                        tocaption[toprint[j].cp_num].pct->raise(); // поднять выше всех - caption

        }
}


void MainWindow::recomp()
{
    double d;
    QRect rc;
    for(int i=0; i<=buf; i++)
    {
        toprint[i].show=false;
        d=i+1;
        d=ceil(d/double(img_on_list));
        toprint[i].list=d;
        toprint[i].cp_num=-1;
        rc.setRect(0,0,0,0);
        setCaptionRect(i,rc);
    }
    lists=0;
    for(int i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
    //curlist=1;
    show_pict();
    save_view_sett();
}

void MainWindow::pct_press_rott() // нажатие на ручку вращения
{
    rott->raise(); // поднять выше всех
    int a=toprint[bufpress2].rot+90;
    rotated(a);
    timer->stop();
    timer->start(3000);
}

void MainWindow::rotated(int g)
{
    double r=g*PI/180;
    QPixmap pix(1,1);
    double d;
    QPoint pn;
    pn.setX(toprint[bufpress2].left+toprint[bufpress2].width/2);
    pn.setY(toprint[bufpress2].top+toprint[bufpress2].height/2);
    pix=toprint[bufpress2].pix0;
    QSize sz=pix.size();
    int x=sz.width();
    int y=sz.height();
    int z=sqrt(x*x+y*y);
    QSize sz2;
    sz2.setHeight(z);
    sz2.setWidth(z);
    QPixmap canv_pix(sz2);
    canv_pix.fill(Qt::transparent); // залить пустотой
    QPainter p(&canv_pix);
    // центр холста
    p.translate(z/2,z/2);
    p.rotate(g);
    p.translate(-z/2,-z/2);
    p.drawPixmap((z-x)/2,(z-y)/2, pix);
    p.end();
    int h=x*fabs(sin(r))+y*fabs(cos(r));
    int w=x*fabs(cos(r))+y*fabs(sin(r));
    x=(z-w)/2;
    y=(z-h)/2;
    pix=canv_pix.copy(x, y, w, h);
    toprint[bufpress2].pix=pix;
    d=pix.width();
    x=d/toprint[bufpress2].compress;
    d=pix.height();
    y=d/toprint[bufpress2].compress;
    toprint[bufpress2].width=x;
    toprint[bufpress2].height=y;
    toprint[bufpress2].left=pn.x()-x/2;
    toprint[bufpress2].top=pn.y()-y/2;
    reShow(imgpress2);
    int rx, ry, y2;
    rx=toprint[bufpress2].left-8+toprint[bufpress2].width;
    if (rx>fon->width()) rx=fon->width()-16;
    y2=ry=toprint[bufpress2].top-8+toprint[bufpress2].height;
    if (ry>fon->height()) ry=fon->height()-16;
    resiz->move(rx,ry);
    resiz->raise();
    ry=toprint[bufpress2].top-8;
    if (ry>fon->height()) ry=fon->height()-16;
    delt->move(rx,ry);
    rott->move(toprint[bufpress2].left-8, toprint[bufpress2].top);
    clip->move(toprint[bufpress2].left-8, y2);
    if(g>180)g=g-360;
    if(g<-180)g=360+g;
    toprint[bufpress2].rot=g;
    out_rot=true;
    ui->dial_4->setValue(toprint[bufpress2].rot);
    out_rot=false;
}

void MainWindow::reShow(int index)  // обновить превью для картинки index (№ превью)
{
    int i=toshow[index].buf;
    if (toprint[i].list==curlist)
      {
          toshow[index].pct->setPixmap(toprint[i].pix);
          toshow[index].pct->setGeometry(toprint[i].left,toprint[i].top,toprint[i].width,toprint[i].height);
      }
}

void MainWindow::resiz_press(int x, int y) // нажатие на ручку изменения размера
{
    resiz->setCursor(Qt::ClosedHandCursor); // поменять курсор
    resiz->raise(); // поднять выше всех
    ttx=x;
    tty=y;
    timer->stop();
}

void MainWindow::resiz_move(int x, int y)
{
    {
        int x1, y1, cx, cy;
        int x2, y2, x3, y3, x4, y4;
        float k, k1;
        double d1, d2;
        QRect rc;
        x1=x2=x3=ttx;
        y1=y2=y3=tty;
        ttx = x;
        tty = y;
        rc=resiz->geometry();
        x1=rc.left()+ttx-x1;
        y1=rc.top()+tty-y1;
        resiz->move(x1,y1);
        rc=toshow[imgpress2].pct->geometry();
        QRect rc2=rc;
        cx=rc.left()-8;
        cy=rc.top()+rc.height()-8;
        k=toshow[imgpress2].pct->pixmap()->height();
        k1=toshow[imgpress2].pct->pixmap()->width();
        k=k/k1;
        rc.setWidth(rc.width()+(ttx-x2));
        if (prop) rc.setHeight(rc.width()*k);
        else      rc.setHeight(rc.height()+(tty-y2));
        toshow[imgpress2].pct->setGeometry(rc);
        int rx, ry;
        rc=toshow[imgpress2].pct->geometry();
        rx=rc.left()-8+rc.width();
        if (rx>fon->width()) rx=fon->width()-16;
        ry=toprint[bufpress2].top-8;
        if (ry>fon->height()) ry=fon->height()-16;
        delt->move(rx,ry);
        d1=toprint[bufpress2].pix.width();
        d2=rc.width();
        toprint[bufpress2].compress=d1/d2;
        show_pict_size();
        clip->move(cx,cy);
        // begin caption
        rc=toshow[imgpress2].pct->geometry();
        x4=y4=0;
        if(rc2.top()<toprint[bufpress2].topCap) y4=rc2.height()-rc.height();
        if(rc2.left()<toprint[bufpress2].leftCap) x4=rc2.width()-rc.width();
        rc=getCaptionRect(bufpress2);
        x3=rc.left()-x4;
        y3=rc.top()-y4;
        int h=rc.height();
        int w=rc.width();
        rc.setLeft(x3);
        rc.setTop(y3);
        rc.setWidth(w);
        rc.setHeight(h);
        setCaptionRect(bufpress2,rc);
        if(toprint[bufpress2].show_caption || ui->checkBox_12->isChecked())
        {
            tocaption[toprint[bufpress2].cp_num].pct->move(x3,y3);
            tocaption[toprint[bufpress2].cp_num].pct->raise();
        }
        // end caption
    }
}

void MainWindow::resiz_up()
{
    setNewSizeOfPct();
    timer->start(3000);
    resiz->setCursor(Qt::PointingHandCursor);
    int rx, ry;
    rx=toprint[bufpress2].left-8+toprint[bufpress2].width;
    if (rx>fon->width()) rx=fon->width()-16;
    ry=toprint[bufpress2].top-8+toprint[bufpress2].height;
    if (ry>fon->height()) ry=fon->height()-16;
    resiz->move(rx,ry);
    rx=clip->geometry().left();
    clip->move(rx,ry);
}

void MainWindow::pct_press_delete()
{
    if (buf==0)
    {
        toprint.clear();
        buf=-1;
        kill_pict();
        quick_buttons_off();
        return;
    }
    toprint.erase(toprint.begin()+bufpress2);
    buf--;
    quick_buttons_off();
    show_pict();
}

void MainWindow::fill_all()
{
    // Расчитать значения сторон картинки для полного заполнения листа
    // в первой компоновке   (для текущего листа)
     double e;
     int i;
     QRect rc=toshow[0].pct->geometry();
     double d1=rc.width();
     double d2=rc.height();
     e=d1/d2; // соотношение сторон картинки
     if ((paper_w<paper_h))
     {
        if (rc.width() >= rc.height())
        {
           i=h_fon*e;
           if (i>w_fon)
           {
               toprint[toshow[0].buf].width=i;
               toprint[toshow[0].buf].height=h_fon;
               return;
           }
           else
           {
               toprint[toshow[0].buf].width=w_fon;
               toprint[toshow[0].buf].height=w_fon/e;
               return;
           }
        }
        else
        {
           i=w_fon/e;
           if (i>h_fon)
           {
               toprint[toshow[0].buf].width=w_fon;
               toprint[toshow[0].buf].height=i;
               return;
            }
           else
           {
               toprint[toshow[0].buf].width=h_fon*e;
               toprint[toshow[0].buf].height=h_fon;
               return;
            }
         }
     }
     else
     {
         if (rc.width()>=rc.height())
         {
             i=w_fon/e;
             if(i>h_fon)
             {
                toprint[toshow[0].buf].width=w_fon;
                toprint[toshow[0].buf].height=i;
                return;
             }
             else
             {
                toprint[toshow[0].buf].width=h_fon*e;
                toprint[toshow[0].buf].height=h_fon;
                return;
             }
          }
         else
         {
             i=h_fon*e;
             if(i>w_fon)
             {
                 toprint[toshow[0].buf].width=i;
                 toprint[toshow[0].buf].height=h_fon;
                 return;
             }
             else
             {
                 toprint[toshow[0].buf].width=w_fon;
                 toprint[toshow[0].buf].height=w_fon/e;
                 return;
             }

         }
    }
}

void MainWindow::save_wind_size()
{
     setty.beginGroup("MainWindow");
        setty.setValue("size", size());
        setty.setValue("pos", pos());
     setty.endGroup();
}

void MainWindow::rest_sind_size()
{
   setty.beginGroup("MainWindow");
        resize(setty.value("size", QSize(400, 400)).toSize());
        move(setty.value("pos", QPoint(200, 200)).toPoint());
   setty.endGroup();
}

void MainWindow::save_printer_sett()
{
    setty.beginGroup("Printer");
        setty.setValue("printer_name", p_name);
        setty.setValue("left_m", left_m);
        setty.setValue("right_m", right_m);
        setty.setValue("top_m", top_m);
        setty.setValue("bottom_m", bottom_m);
        setty.setValue("paper_name", pap_name);
        setty.setValue("color", print_color);
        setty.setValue("offset", h_ofsett);
        setty.setValue("sourse", pap_sor);
        setty.setValue("width", paper_w);
        setty.setValue("height", paper_h);
        setty.setValue("caption", list_n);
        setty.setValue("font_scl", font_scl);
    setty.endGroup();
    setty.beginGroup(p_name);
        setty.setValue("left_m", left_m);
        setty.setValue("right_m", right_m);
        setty.setValue("top_m", top_m);
        setty.setValue("bottom_m", bottom_m);
        setty.setValue("paper_name", pap_name);
        setty.setValue("color", print_color);
        setty.setValue("offset", h_ofsett);
        setty.setValue("sourse", pap_sor);
        setty.setValue("font_scl", font_scl);
    setty.endGroup();
}

QString MainWindow::get_run(QString s)
{
    QProcess proc;
    proc.start(s);
    proc.waitForFinished(-1);
    QByteArray btar;
    btar = proc.readAll();
    QString ver;
    ver.append(btar);
    return ver;
}

void MainWindow::rest_printer_sett()
{
    // принтер по умолчанию
    QString l;
    l.append(get_run("lpstat -d"));
    p_name=(l.mid(28, l.length()-29));

    setty.beginGroup("Printer");
        p_name=(setty.value("printer_name", p_name).toString());
        print_color=setty.value("color", true).toBool();
        pap_name=setty.value("paper_name", 0).toInt();
        pap_sor=setty.value("sourse", 0).toInt();
        paper_h=setty.value("height", 297).toInt();
        paper_w=setty.value("width", 210).toInt();
        list_n=setty.value("caption", "A4 210x297 mm").toString();
        font_scl=setty.value("font_scl", 1).toDouble();
    setty.endGroup();
}

void MainWindow::save_view_sett()
{
    setty.beginGroup("Settings");
        setty.setValue("layout", comp);
    setty.endGroup();
}

void MainWindow::rest_view_sett()
{
    setty.beginGroup("Settings");
        set_layout(setty.value("layout", 0).toInt());
    setty.endGroup();
}


int HexToInt(char ch)
{
    int  i;
    i=int(ch);
    if (i>64) i=i-55;
    else i=i-48;
    return i;
}
QString MainWindow::esc_to_utf(QString st)
// преобразование строки из escape последовательности в UTF8
// только для кириллицы !!!
// долбаный марлин... и не только он...
{
    int h;
    QString res;
    QByteArray ba=st.toLatin1();
    QString alp;
    alp.append("АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя");
    int i=0;
    while (i<st.length())
    {
        if (st.mid(i, 4)=="%D0%" || st.mid(i, 4)=="%D1%" || st.mid(i,3)=="%20")
           {
            if(st.mid(i,3)=="%20")
            {
                res.append(" ");
                i=i+2;
            }
            else
            {
                h=1024 + HexToInt(ba[i+2]) * 64 + 16 * (HexToInt(ba[i+4])-8) + HexToInt(ba[i+5]);
                if (h==1025)res.append("Ё");
                if (h==1105)res.append("ё");
                h=h-1040;
                if (h>-1 && h<64)  res.append(alp.mid(h,1));
                cout << res.toStdString() << endl;
                i=i+5;
            }
           }
        else res.append(st.mid(i,1));
        i++;
    }
    return res;
}

void MainWindow::sheetPress() // нажатие на чистое место листа
{

    showPctBord(false);
    if(imgpress2>-1)
    {
        resetCursor();
        quick_buttons_off();
        if (rez!=0)rez->hide();
    }
    ui->pushButton_32->hide();
    ui->checkBox_10->setVisible(true);
    ui->checkBox_12->setVisible(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
}

void MainWindow::show_pict_size()
{
    QSize sz=toshow[imgpress2].pct->size();
    QString l;
    double d1;
    l.append("Width: ");
    d1=sz.width();
    l.append(QString::number(d1/ppx));
    l.clear();
    l.append("height: ");
    d1=sz.height();
    l.append(QString::number(d1/ppy));
}

void MainWindow::set_layout(int c)
{
    switch(c)
    {
    case 0:
        on_l1_2_clicked(); break;
    case 1:
        on_l2_2_clicked(); break;
    case 2:
        on_l3_2_clicked(); break;
    case 3:
        on_l4_2_clicked(); break;
    case 4:
        on_l5_2_clicked(); break;
    case 5:
        on_l6_2_clicked(); break;
    case 6:
        on_l7_2_clicked(); break;
    case 7:
        on_l8_2_clicked(); break;
    case 8:
        on_l9_2_clicked(); break;
    case 9:
        on_l10_2_clicked(); break;
    default:
        on_l1_2_clicked();
    }
}


QPixmap MainWindow::cut_pix(QPixmap pix, QRect rc)
{
    QPixmap p=pix.copy(rc);
    return p;
}

void MainWindow::show_clip() // включить рамку обрезки
{
    if (rez==0)
    {
        rez=new QavLabel(fon);
        rez->setAutoFillBackground(false);
        setStyleSheet("QavLabel#rez { border-color: red; } ");
        connect(rez, SIGNAL(mouse_move(int,int,int)), this, SLOT(rez_move(int, int)));
        connect(rez, SIGNAL(mouse_press(int,int,int)), this, SLOT(rez_press(int, int)));
        connect(rez, SIGNAL(mouse_up(int,int,int)), this, SLOT(rez_up()));
        cout << "trimming frame is created"<< endl;
    }
    rez->setGeometry(toshow[imgpress2].pct->geometry());
    paint_frame();
    rez->raise();
    rez->show();
    quick_buttons_off();
    ui->pushButton_32->show();
    ui->tabWidget_2->setCurrentIndex(3);
    ui->checkBox_10->setVisible(false);
    ui->checkBox_12->setVisible(false);
    showPctBord(false);
}

void MainWindow::paint_frame()
{
    QPixmap pm(QSize(rez->width(), rez->height()));
    pm.fill(Qt::transparent);
    QPainter painter(&pm);
    //цвет линии and style
    QPen pen;
    pen.setBrush(QColor(255,128,0));
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    //рамка
    painter.drawLine(0,0, rez->width(), 0);
    painter.drawLine(0,0, 0, rez->height());
    painter.drawLine(rez->width()-1,0, rez->width()-1, rez->height());
    painter.drawLine(rez->width()-1, rez->height()-1, 0, rez->height()-1);
    //Отображаем результат отрисовки
    rez->setPixmap(pm);
}

void MainWindow::quick_buttons_off()
{
    rott->hide();
    resiz->hide();
    delt->hide();
    clip->hide();
}

void MainWindow::rez_move(int x, int y)
{
    QRect rc=rez->geometry();
    int dx=x-ttx;
    int dy=y-tty;
    if (t) rc.setTop(rc.top()+dy);
    if (l) rc.setLeft(rc.left()+dx);
    if (b) rc.setHeight(rc.height()+dy);
    if (r) rc.setWidth(rc.width()+dx);
    if (!(l | t | r | b))
    {
        ttx=rc.left()+dx;
        tty=rc.top()+dy;
        rez->move(ttx, tty);
    }
    else  rez->setGeometry(rc);
    ttx=x; tty=y;
    paint_frame();
}

void MainWindow::rez_press(int x, int y)
{
    ttx=x; tty=y;
    l=t=r=b=false;
    if(imgpress2<0) return;
    int xx=rez->x()+ui->sheet->x()+this->x();
    int yy=rez->y()+ui->sheet->y()+this->geometry().top();
    int dx1=x-xx;
    int dx2=abs(dx1-rez->geometry().width());
    int dy1=y-yy;
    int dy2=abs(dy1-rez->geometry().height());
    l=(dx1<7);
    r=(dx2<7);
    t=(dy1<7);
    b=(dy2<7);
    if ((l || r) && !(t || b))rez->setCursor(Qt::SizeHorCursor);
    if (!(l || r) && (t || b))rez->setCursor(Qt::SizeVerCursor);
    if ((l && b) || (t && r))rez->setCursor(Qt::SizeBDiagCursor);
    if ((l && t) || (r && b))rez->setCursor(Qt::SizeFDiagCursor);
    if (!(l | t | r| b)) rez->setCursor(Qt::SizeAllCursor);
}

void MainWindow::rez_up()
{
    rez->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_pushButton_32_clicked() // обрезка
{
    QRect rc=rez->geometry();
    QRect rc1=toshow[imgpress2].pct->geometry();
    rc1.setTop(rc.top()-rc1.top());
    rc1.setLeft(rc.left()-rc1.left());
    rc1.setTop(double(rc1.top()) * toprint[bufpress2].compress);
    rc1.setLeft(double(rc1.left()) * toprint[bufpress2].compress);
    rc1.setWidth(double(rc.width()) * toprint[bufpress2].compress);
    rc1.setHeight(double(rc.height()) * toprint[bufpress2].compress);
    QPixmap p = toprint[bufpress2].pix;
    p=p.copy(rc1);
    toprint[bufpress2].pix=p;
    toprint[bufpress2].pix0=p;
    toprint[bufpress2].left=rc.left();
    toprint[bufpress2].top=rc.top();
    toprint[bufpress2].width=rc.width();
    toprint[bufpress2].height=rc.height();
    toshow[imgpress2].pct->setPixmap(p);
    toshow[imgpress2].pct->setGeometry(rc);
    cout << "clipped successfully!" << endl;
    ui->pushButton_32->hide();
    rez->hide();
    ui->checkBox_10->setVisible(true);
    ui->checkBox_12->setVisible(true);
}

void MainWindow::on_pushButton_36_clicked()
{
    if(ab==0)
    {
        ab=new about();
        QFont font("Ubuntu");
        font.setPointSize(10);
        ab->setFont(font);
    }
    ab->show();
}

void MainWindow::show_r_menu(int x, int y, int i)
{    
    if (rmenu==0)
    {
        rmenu = new QMenu;
    }
    QRect rc;
    rc.setTop(y);
    rc.setLeft(x);
    rc.setWidth(400);
    rc.setHeight(50);
    rmenu->setGeometry(rc);
    imgpress2=i;
    bufpress2=toshow[i].buf;
    showPctBord(true);
    quick_buttons_off();
    if(toprint[bufpress2].isTextBlock) set_rmenu(2);
    else set_rmenu(1);
}

void MainWindow::rot()
{
    int a=toprint[bufpress2].rot+90;
    rotated(a);
}

void MainWindow::set_rmenu(int index) // отобразить контекстное меню № index
{
    rmenu->clear();
    switch(index)
    {
        case 1:
            make_menu_1();
            break;
        case 2:
            make_menu_2();
            break;
    }
    rmenu->show();
    imgpress=-1;
    bufpress=-1;
}

void MainWindow::make_menu_1()
// правый клик по картинке
{
    QAction *ac1 = rmenu->addAction(tr("&Rotated clockwise by 90 degrees"), this, SLOT(rot()));
    ac1->setIcon(QPixmap(":/new/prefix1/rotation"));
    ac1->setIconVisibleInMenu(true);
    QAction *ac2 = rmenu->addAction(tr("&Delete this image"), this, SLOT(pct_press_delete()));
    ac2->setIcon(QPixmap(":/new/prefix1/delete"));
    ac2->setIconVisibleInMenu(true);
    QAction *ac3 = rmenu->addAction(tr("&Cut out a fragment image"), this, SLOT(show_clip()));
    ac3->setIcon(QPixmap(":/new/prefix1/rez"));
    ac3->setIconVisibleInMenu(true);
    QAction *ac4 = rmenu->addAction(tr("Caption &editor"), this, SLOT(show_cap_editor()));
    ac4->setIcon(QPixmap(":/new/prefix1/pencil"));
    ac4->setIconVisibleInMenu(true);
    QAction *ac5 = rmenu->addAction(tr("&Simply turn on/off caption"), this, SLOT(turn_caption()));
    ac5->setIcon(QPixmap(":/new/prefix1/pencil"));
    ac5->setIconVisibleInMenu(true);
    QAction *ac6 = rmenu->addAction(tr("Move to the &previous sheet"), this, SLOT(move_prev()));
    ac6->setIcon(QPixmap(":/new/prefix1/down"));
    QAction *ac7 = rmenu->addAction(tr("Move to the &next sheet"), this, SLOT(move_next()));
    ac7->setIcon(QPixmap(":/new/prefix1/up"));
    QAction *ac8 = rmenu->addAction(tr("Transform picture"), this, SLOT(on_pushButton_2_clicked()));
    ac8->setIcon(QPixmap(":/new/prefix1/transform"));
}

void MainWindow::make_menu_2()
// правый клик по text_block
{
    QAction *ac1 = rmenu->addAction(tr("&Rotated clockwise by 90 degrees"), this, SLOT(rot()));
    ac1->setIcon(QPixmap(":/new/prefix1/rotation"));
    ac1->setIconVisibleInMenu(true);
    QAction *ac2 = rmenu->addAction(tr("&Delete this image"), this, SLOT(pct_press_delete()));
    ac2->setIcon(QPixmap(":/new/prefix1/delete"));
    ac2->setIconVisibleInMenu(true);
    QAction *ac5 = rmenu->addAction(tr("&Edit this text block"), this, SLOT(edit_textBlock()));
    ac5->setIcon(QPixmap(":/new/prefix1/pencil"));
    ac5->setIconVisibleInMenu(true);
    QAction *ac6 = rmenu->addAction(tr("Move to the &previous sheet"), this, SLOT(move_prev()));
    ac6->setIcon(QPixmap(":/new/prefix1/down"));
    QAction *ac7 = rmenu->addAction(tr("Move to the &next sheet"), this, SLOT(move_next()));
    ac7->setIcon(QPixmap(":/new/prefix1/up"));
    QAction *ac8 = rmenu->addAction(tr("Transform picture"), this, SLOT(on_pushButton_2_clicked()));
    ac8->setIcon(QPixmap(":/new/prefix1/transform"));
}


void MainWindow::edit_textBlock()
// редактировать текстовый блок
{

    open_textblockEd();
    txed->loadtext(toprint[bufpress2].caption,
                   toprint[bufpress2].back_color,
                   toprint[bufpress2].font_color,
                   toprint[bufpress2].font,
                   toprint[bufpress2].trans,
                   toprint[bufpress2].alig);
}

void MainWindow::move_prev()
// переместить картинку на предыдущий лист
{

    toprint[bufpress2].list--;
    if(toprint[bufpress2].list==0)
    {
        for(int i=0; i<=buf; i++) toprint[i].list++;
        lists++;
        curlist++;
    }
    sheetPress();
    show_pict();
}

void MainWindow::move_next()
// переместить картинку на следующий лист
{
    toprint[bufpress2].list++;
    if(toprint[bufpress2].list>lists)lists++;
    sheetPress();
    show_pict();
}

void MainWindow::resetCursor()
{
    imgFrame=-1;
    imgpress=-1;
    bufpress=-1;
    imgpress2=-1;
    bufpress2=-1;
}

void MainWindow::turn_caption() //переключить состояние видимости подписи
{
    toprint[bufpress2].show_caption=!toprint[bufpress2].show_caption;
    show_caption(bufpress2);
}

void MainWindow::show_cap_editor()
{
    if (cped==0)
    {
        cped=new captioneditor;
        QFont font;
        font.setPointSize(fn_size);
        cped->setFont(font);
        connect(cped, SIGNAL(end_edit(QString,QColor,QColor,QFont,bool,bool)),
                this, SLOT(set_caption(QString,QColor,QColor,QFont,bool,bool)));
    }
    caption=toprint[bufpress2].caption;
    show_cap=toprint[bufpress2].show_caption;
    font_cl=toprint[bufpress2].font_color;
    back_cl=toprint[bufpress2].back_color;
    font_cpt=toprint[bufpress2].font;
    trans=toprint[bufpress2].trans;
    cped->fullFileName=toprint[bufpress2].pict;
    cped->load_caption();
    cped->show();
}

void MainWindow::set_caption(QString text, QColor f_cl, QColor b_cl, QFont fn, bool shw, bool tr)
{ // for bufpress2 only
    toprint[bufpress2].caption=text;       // подпись к картинке
    toprint[bufpress2].show_caption=shw;   // показывать подпись
    toprint[bufpress2].back_color=b_cl;    // цвет фона
    toprint[bufpress2].font_color=f_cl;    // цвет шрифта
    toprint[bufpress2].font=fn;            // шрифт
    toprint[bufpress2].trans=tr;           // порзрачный фон
    cp_setGeometry(bufpress2);             // геометрия подписи
    cp_setPixmap(bufpress2);               // картинка подписи
    show_caption(bufpress2);               // show caption
}

void MainWindow::cp_setGeometry(int index) // начальная геометрия подписи
{
    QFontMetrics m(toprint[index].font);
    int w=m.width(toprint[index].caption);
    int h=m.height();
    int t=toprint[index].top+toprint[index].height;
    int l=toprint[index].left+(toprint[index].width-w)/2;
    setCaptionRect(index,QRect(l,t,w,h));       // геометрия подписи
}


void MainWindow::cp_setPixmap(int index)
// создать картинку надписи - нужно для предпросмотра, чтобы масштабировать
{
    cp_setGeometry(index);
    QRect rc=getCaptionRect(index);
    int h=rc.height();
    int w=rc.width();
    rc.setTop(0);
    rc.setLeft(0);
    rc.setWidth(w);
    rc.setHeight(h);
    QPixmap pm(w, h);
    if(toprint[index].trans) pm.fill(Qt::transparent);
    else pm.fill(toprint[index].back_color);
    QPainter pn;
    pn.begin(&pm); // начать рисование
    pn.setFont(toprint[index].font);
    pn.setPen(toprint[index].font_color);
    pn.drawText(rc, Qt::AlignLeft, toprint[index].caption);
    pn.end(); // закончить рисование
    toprint[index].cp_pixmap=pm;
}

void MainWindow::show_caption(int index) // показать подпись к картинке index
{
    if(toprint[toshow[index].buf].isTextBlock)return;
    int count;
    QRect rc;
    if (toprint[index].list!=curlist)return;
        if(toprint[index].show_caption || ui->checkBox_12->isChecked())
        {   // создать подпись
          if(toprint[index].widthCap==0)cp_setGeometry(index);    // создать геометрию
          if(toprint[index].cp_num==-1)
          { // создать avLabel для этой подписи, если надо
            cpt_count++;
            tocaption.push_back(prew());
            count=toprint[index].cp_num=cpt_count;
            tocaption[cpt_count].buf=index;
            tocaption[cpt_count].pct = new QavLabel(fon);
            tocaption[cpt_count].pct->setImnum(count);
            tocaption[cpt_count].pct->setBackgroundRole(QPalette::Base);
            tocaption[cpt_count].pct->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            tocaption[cpt_count].pct->setCursor(Qt::OpenHandCursor);
            // сигналы
            QObject::connect(tocaption[count].pct, SIGNAL(mouse_press(int, int, int)),
                         this, SLOT(cp_press(int, int, int)));
            QObject::connect(tocaption[count].pct, SIGNAL(mouse_move(int, int, int)),
                         this, SLOT(cp_move(int, int, int)));
            QObject::connect(tocaption[count].pct, SIGNAL(mouse_up(int, int, int)),
                         this, SLOT(cp_up(int, int, int)));
            cp_setPixmap(index);
          }
          else count=toprint[index].cp_num;
          tocaption[count].pct->setScaledContents(true);
          rc=getCaptionRect(index);
          list_scl=double(ui->list->widthMM())/double(curSheet.width);
          rc.setWidth(double(rc.width())*list_scl*font_scl);
          rc.setHeight(double(rc.height())*list_scl*font_scl);
          tocaption[count].pct->setGeometry(rc);
          tocaption[count].pct->setPixmap(toprint[index].cp_pixmap);
          tocaption[count].pct->show();
          tocaption[count].pct->raise();
          toprint[index].cp_z=++curz;
          //cout << "caption size:  " << rc.width()<< ", "<< rc.height()<< endl;
      }
    else{ // убрать подпись
        if(toprint[index].cp_num>-1){
            tocaption[toprint[index].cp_num].pct->deleteLater();
            toprint[index].cp_num=-1;
            cpt_count--;
            }
        }
}
void MainWindow::cp_press(int x, int y, int i) // нажатие на caption
{
    if(rez!=0) rez->hide();
    tocaption[i].pct->setCursor(Qt::ClosedHandCursor); // поменять курсор
    tocaption[i].pct->raise(); // поднять выше всех
    toprint[toshow[i].buf].z=++curz;
    ttx=x;
    tty=y;
    imgpress=i;
    imgpress2=i;
    bufpress=tocaption[i].buf;
    bufpress2=bufpress;
}

void MainWindow::cp_move(int x, int y, int i)
{
        int x1=0, y1=0;
        x1=ttx;
        y1=tty;
        ttx = x;
        tty = y;
        QRect rc=getCaptionRect(bufpress);
        x1=rc.left()+ttx-x1;
        y1=rc.top()+tty-y1;
        int h=rc.height();
        int w=rc.width();
        tocaption[i].pct->move(x1,y1);
        rc.setLeft(x1);
        rc.setTop(y1);
        rc.setHeight(h);
        rc.setWidth(w);
        setCaptionRect(bufpress,rc);
}

void MainWindow::cp_up(int x, int y, int i)
{
    if (i==imgpress)
    {
        imgpress=-1;
        bufpress=-1;
        tocaption[i].pct->setCursor(Qt::OpenHandCursor);
        x=y;
        y=x;
    }
}

void MainWindow::on_checkBox_12_clicked(bool checked) // включить подписи
{
    if(buf<0 || exitFlag)return;
    for(int i=0; i<=buf; i++) toprint[i].show_caption=checked;
    show_pict();

}

// Текстовые блоки
//**************************************************************

void MainWindow::open_textblockEd()
{
    if (txed==0)
    {
        txed=new TextEditor;
        QFont font;
        font.setPointSize(fn_size);
        txed->setFont(font);
        connect(txed, SIGNAL(settext(QString,QColor,QColor,QFont,bool,int)),
                this, SLOT(setTextBlock(QString,QColor,QColor,QFont,bool,int)));
    }
    txed->show();
}

void MainWindow::on_pushButton_37_clicked()
// вставить текст
{
    bufpress2=-1;
    open_textblockEd();
}

void MainWindow::setTextBlock(QString text, QColor BackColor, QColor LitColor,
                              QFont TextFont, bool trans, int alig)
{
    int lin=0;
    QString st="";
    QString strk="";
    QFontMetrics m(TextFont);
    for(int i=0; i<text.length(); i++)
    {
        st=st.append(text[i]);
        if (text[i]=='\n')
        {
            lin++;
            if(m.width(st.trimmed())>m.width(strk.trimmed()))
            {
                strk.clear();
                strk.append(st);
                st.clear();
            }
        }
    }
    if(st!="")
    {
        lin++;
        if(m.width(st.trimmed())>m.width(strk.trimmed()))
        {
            strk.clear();
            strk.append(st);
            st.clear();
        }
    }
    int h=m.height()*lin;               // высота картинки
    int w=m.width(strk.trimmed());      // ширина картинки
    QRect rc;
    rc.setTop(0);
    rc.setLeft(0);
    rc.setWidth(w);
    rc.setHeight(h);
    if (rap) cout << "h= "<< h << " w= "<< w << endl;
    QPixmap px(w,h);
    if (trans) px.fill(Qt::transparent);
    else       px.fill(BackColor);
    pntr->begin(&px);
    pntr->setPen(LitColor);
    pntr->setFont(TextFont);
    switch (alig)
    {
        case 0:
            pntr->drawText(rc, Qt::AlignLeft, text);
            break;
        case 1:
            pntr->drawText(rc, Qt::AlignCenter, text);
            break;
        case 2:
            pntr->drawText(rc, Qt::AlignRight, text);
            break;
    }
    pntr->end();
    addTextPictute(px,text,BackColor,LitColor,TextFont,trans,alig);
    show_pict();
}


void MainWindow::addTextPictute(QPixmap pixmap, QString text,
                     QColor BackColor, QColor LitColor, QFont TextFont, bool trans, int alig)
// добавить (обновить) картинку-textblock в список
{
    int i=bufpress2;
    if (bufpress2==-1) // new text block
    {
        buf++;
        if(lists==0)
        {
            lists=1;
            curlist=1;
        }
        toprint.push_back(pict());
        i=buf;
        toprint[i].show=false;
    }
    else               // edit text block
    {
        toprint[i].show=true;
    }
        toprint[i].pict="";
        toprint[i].caption=text;
        toprint[i].show_caption=false;
        toprint[i].back_color=BackColor;    // цвет фона
        toprint[i].font_color=LitColor;     // цвет шрифта
        toprint[i].trans=trans;             // прозрачный фон
        toprint[i].isTextBlock=true;
        toprint[i].font=TextFont;           // шрифт
        toprint[i].cp_num=-1;               // нет привязанного avLabel
        QRect rc;
        rc.setRect(0,0,0,0);
        setCaptionRect(i,rc);                 // начальная геометрия подписи
        toprint[i].pix0=toprint[buf].pix = pixmap;
        toprint[i].list=curlist;
        toprint[i].alig=alig;
        if (toprint[i].rot!=0)
        {
            rotated(toprint[i].rot);
        }
        if (comp==10)toprint[i].list=1;
        lists=0;
        for(i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
        if (rap) cout << "textblock was set" << endl;
        img_size_cur_comp();
        show_pict();
}

// сеансы
// **********************************************************************************

void MainWindow::on_pushButton_27_clicked()
// сохранить сеанс
{
    QString user=getenv("HOME");
    if(pathFile)
    {
        setty.beginGroup("Settings");
        user=setty.value("inPath",user).toString();
        setty.endGroup();
    }
    QString fileName = QFileDialog::getSaveFileName
            (this, tr("Save in file..."), user, tr("vap_sessions (*.vap)"));
    if(fileName.isEmpty()) return;
    setty.beginGroup("Settings");
    setty.setValue("inPath", GetPathFrom(fileName));
    setty.endGroup();
    if (!(isvap(fileName))) fileName.append(".vap");
    if (saveSassion(fileName))
         {if (rap) cout << "Save session in file: "<< fileName.toStdString() << endl;}
    else {if (rap) cout << "Save session failed "<< endl;}
}

bool MainWindow::saveSassion(QString fileName)
// процесс сохранения сеанса
{
    bool r=false;
    if (!fileName.isEmpty())
    {
        ind_start();
        QFile file(fileName);
        r=file.open(QIODevice::WriteOnly ); //| QIODevice::Text);
        //QTextStream out(&file);
        QDataStream out(&file);
        out<<flag2;     out<<comp;      out<<getx;
        out<<gety;      out<<prt_x;
        out<<gor;       out<<ver;       out<<pg_mar;
        out<<pol;       out<<buf;       out<<ttx;
        out<<tty;       out<<curz;      out<<lists;
        out<<curlist;   out<<img_on_list; out<<ppx;
        out<<ppy;       out<<prx;       out<<pry;
        out<<x_prw;     out<<y_prw;     out<<rott_press;
        out<<prop;      out<<w_cou;     out<<h_cou;
        out<<w_fon;     out<<h_fon;     out<<t_comp;
        out<<out_rot;   out <<  fillsize; out<< mem_pol;
        out <<  h_min_l;        // минимальная высота окна для ланшафтной ориентации
        out <<  h_min_p;        // минимальная высота окна для портретной ориентации
        out <<  paper_ratio;    // пропорции бумаги h/w
        out <<  wind_sz;        // текущие размеры главного окна
        out <<  gor_old;        // предыдущие размеры окна
        out <<  offset;         // смещение кнопок компоновки при прокрутке
        out <<  list_scl;       // реальное соотношение размеров экрана и бумаги
        out <<  pathFile;       // запоминать путь к последнему файлу и открывать диалоги там же
        out <<  autoOrn;        // автоматический выбор ориентации бумаги
        out <<  exitFlag;       // флаг выхода из функции, когда она вызывается, но выполнять ее не надо
        out <<  fun;            // выдавать имена вызывемых функций в терминал
        QApplication::processEvents();
        out << left_m;
        out << right_m;
        out << top_m;
        out << bottom_m;
        out << paper_w;
        out << paper_h;
        out << rap;
        out << all_rot;
        out << ul_hor;
        out << ul_ver;
        out << p_name;
        out << sheet_size;
        out << all_sizes;
        out << set_orn;
        out << print_color;
        out << list_n;
        out << h_ofsett;        // горизонтальное смещение позиции печати
        out << pap_sor;         // источник бумаги
        out << pap_name;
        out << caption;
        out << show_cap;
        out << font_cl;         // цвет шрифта подписи
        out << back_cl;         // цвет шрифта фона
        out <<  font_cpt;       // шрифт подписи
        out << trans;           // прозрачный фон надписи
        out << runShow;         // флаг разрешения работы процедуры show_pict - чтобы избежать бесконечной рекурсии
        out << font_scl;
        out << noResizewindow;  // Запрет на изменение размера окна программы

        for(int i=0; i<=buf; i++)
            {
            out<<toprint[i].pict;          // путь к файлу
            out<<toprint[i].rot;           // 0 - нормально, угол поворота
            out<<toprint[i].pix0;          // исходный pixmap
            QApplication::processEvents();
            out<<toprint[i].pix;           // текущий pixmap
            QApplication::processEvents();
            out<<toprint[i].left;          // координата Х
            out<<toprint[i].top;           // координата Y
            out<<toprint[i].height;        // высота
            out<<toprint[i].width;         // ширина
            out<<toprint[i].show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
            out<<toprint[i].list;          // номер страницы этой картинки
            out<<toprint[i].load;          // флаг того, картинка уже загружена в память
            out<<toprint[i].z;             // z-порядок
            out<<toprint[i].prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
            out<<toprint[i].compress;      // кэф. компрессии для уже показанной картинки
            out<<toprint[i].caption;       // подпись к картинке
            out<<toprint[i].show_caption;  // показывать подпись
            out<<toprint[i].back_color;    // цвет фона
            out<<toprint[i].trans;         // прозрачный фон
            out<<toprint[i].font_color;    // цвет шрифта
            out<<toprint[i].font;          // шрифт
            out<<toprint[i].leftCap;       // геометрия подписи
            out<<toprint[i].topCap;
            out<<toprint[i].widthCap;
            out<<toprint[i].heightCap;
            out<<toprint[i].cpt;           // номер подписи на экране к этой картинке
            out<<toprint[i].cp_num;        // номер avLabel - подписи на экране
            out<<toprint[i].cp_z;          // z-орядок для подписи
            out<<toprint[i].cp_pixmap;     // pixmap подписи
            out<<toprint[i].isTextBlock;   // true - это текстовый блок
            out<<toprint[i].alig;
            QApplication::processEvents();
            }
        if (int(sheet.size())<lists)
            for(int i=sheet.size(); i<lists; i++)
                sheet.push_back(curSheet);
        for (int i=0; i<=lists-1; i++)
        {
            out << sheet[i].list_orn;
            out << sheet[i].height;
            out << sheet[i].size;
            out << sheet[i].width;
        }
        file.close();
        ind_stop();
    }
    return r;
}

bool MainWindow::isvap(QString s)
{
    s=s.mid(s.length()-4, 4).toUpper();
    return (s==".VAP");
}


QString MainWindow::GetPathFrom(QString pFile)
{
    QString st;
    for(int i=pFile.length()-1;
        pFile[i]!='/'; --i)
    {
        st=pFile.mid(0,i-1);
    }
    if(st.isEmpty())
    {
        setty.beginGroup("Settings");
        st=setty.value("inPath", getenv("HOME")).toString();
        setty.endGroup();
    }
    return st;
}

void MainWindow::on_pushButton_28_clicked()
// открыть сохраненный сеанс
{
    QString user=getenv("HOME");
    if(pathFile)
    {
        setty.beginGroup("Settings");
        user=setty.value("inPath",user).toString();
        setty.endGroup();
    }
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open file..."), user, tr("vap_sessions (*.vap)"));
    if(fileName.isEmpty()) return;
    setty.beginGroup("Settings");
    setty.setValue("inPath", GetPathFrom(fileName));
    setty.endGroup();
    if (!(isvap(fileName))) fileName.append(".vap");
    if (openSassion(fileName))
          {if (rap) cout << "Open session from file: "<< fileName.toStdString() << endl;}
    else  {if (rap) cout << "Open session failed "<< endl;}
}

bool MainWindow::openSassion(QString fileName)
// процесс восстановления сеанса
{
    bool r=false;
    if(fileName.trimmed()==".vap") return r;
    if (!fileName.isEmpty())
    {
        toprint.clear();
        ind_start();
        QFile file(fileName);
        r=file.open(QIODevice::ReadOnly);
        QDataStream in(&file);

        in >>  flag2;
        in >>	comp;			// текущая компоновка
        set_layout(comp);
        in >>  getx;			// максимальный размер х для картинки  в этой компоновке
        in >>  gety;			// максимальный размер y для картинки  в этой компоновке
        in >>  prt_x;          // x для портретной ориентации
        in >>  gor;			// w листа предпросмотра
        in >>  ver;			// h листа предпросмотра
        in >>  pg_mar;		    // поля листа бумаги
        in >>  pol;			// поля между картинок mm
        in >> buf; 		    // количество загруженных картинок, отсчет с нуля
        toprint.resize(buf+1);
        in >>  ttx;
        in >>  tty;			// временные значения для отработки перемещения объектов
        in >>  curz;           // z порядок для текущей страницы, используется для расчета наложений
        in >>  lists;
        in >>  curlist;        // всего листов и текущий лист; отсчет с 1, т.е. 0 - ничего нет
        in >>  img_on_list;    // кол-во картинок на листе в текущей компоновке
        in >>  ppx;
        in >>  ppy;            // точек на мм по Х и У для текущего предпросмотра
        in >>  prx;
        in >>  pry;            // точек на мм по Х и У для текущего принтера
        in >>  x_prw;
        in >>  y_prw;          // координаты листа предпросмотра
        in >>  rott_press;     // флаг нажатия на ручку вращения
        in >>  prop;           // флаг пропорциональности при изменении размера
        in >>  w_cou;
        in >>  h_cou;          // компоновка w_cou x h_cou
        in >>  w_fon;
        in >>  h_fon;          // текущие размеры фона
        in >>  t_comp;         // временно для хранения номера компоновки
        in >>  out_rot;        // не вызывать слот поворота true - без поворота
        in >>  fillsize;       // режим заполнения по максимальному
        in >>  mem_pol;        // сохраненное значение полей между картинок pol
        in >>  h_min_l;        // минимальная высота окна для ланшафтной ориентации
        in >>  h_min_p;        // минимальная высота окна для портретной ориентации
        in >>  paper_ratio;    // пропорции бумаги h/w
        in >>  wind_sz;        // текущие размеры главного окна
        resize(wind_sz);
        in >>  gor_old;        // предыдущие размеры окна
        in >>  offset;         // смещение кнопок компоновки при прокрутке
        in >>  list_scl;       // реальное соотношение размеров экрана и бумаги
        in >>  pathFile;       // запоминать путь к последнему файлу и открывать диалоги там же
        in >>  autoOrn;        // автоматический выбор ориентации бумаги
        in >>  exitFlag;       // флаг выхода из функции, когда она вызывается, но выполнять ее не надо
        in >>  fun;            // выдавать имена вызывемых функций в терминал
        QApplication::processEvents();
        in >> left_m;
        in >> right_m;
        in >> top_m;
        in >> bottom_m;
        in >> paper_w;
        in >> paper_h;
        in >> rap;
        in >> all_rot;
        in >> ul_hor;
        in >> ul_ver;
        in >> p_name;
        in >> sheet_size;
        in >> all_sizes;
        in >> set_orn;
        in >> print_color;
        in >> list_n;
        in >> h_ofsett;        // горизонтальное смещение позиции печати
        in >> pap_sor;         // источник бумаги
        in >> pap_name;
        in >> caption;
        in >> show_cap;
        in >> font_cl;         // цвет шрифта подписи
        in >> back_cl;         // цвет шрифта фона
        in >> font_cpt;       // шрифт подписи
        in >> trans;           // прозрачный фон надписи
        in >> runShow;         // флаг разрешения работы процедуры show_pict - чтобы избежать бесконечной рекурсии
        in >> font_scl;
        in >> noResizewindow;  // Запрет на изменение размера окна программы
        for(int i=0; i<=buf; i++)
            {
            in>>toprint[i].pict;          // путь к файлу
            in>>toprint[i].rot;           // 0 - нормально, угол поворота
            in>>toprint[i].pix0;          // исходный pixmap
            QApplication::processEvents();
            in>>toprint[i].pix;           // текущий pixmap
            QApplication::processEvents();
            in>>toprint[i].left;          // координата Х
            in>>toprint[i].top;           // координата Y
            in>>toprint[i].height;        // высота
            in>>toprint[i].width;         // ширина
            in>>toprint[i].show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
            in>>toprint[i].list;          // номер страницы этой картинки
            in>>toprint[i].load;          // флаг того, картинка уже загружена в память
            in>>toprint[i].z;             // z-порядок
            in>>toprint[i].prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
            in>>toprint[i].compress;      // кэф. компрессии для уже показанной картинки
            in>>toprint[i].caption;       // подпись к картинке
            in>>toprint[i].show_caption;  // показывать подпись
            in>>toprint[i].back_color;    // цвет фона
            in>>toprint[i].trans;         // прозрачный фон
            in>>toprint[i].font_color;    // цвет шрифта
            in>>toprint[i].font;          // шрифт
            in>>toprint[i].leftCap;       // геометрия подписи
            in>>toprint[i].topCap;
            in>>toprint[i].widthCap;
            in>>toprint[i].heightCap;
            in>>toprint[i].cpt;           // номер подписи на экране к этой картинке
            in>>toprint[i].cp_num;        // номер avLabel - подписи на экране
            in>>toprint[i].cp_z;          // z-орядок для подписи
            in>>toprint[i].cp_pixmap;     // pixmap подписи
            in>>toprint[i].isTextBlock;   // true - это текстовый блок
            in>>toprint[i].alig;
            QApplication::processEvents();
            toprint[i].cp_num=-1;
        }
        sheet.clear();
        sheet.resize(lists);
        for(int i=0;i<lists;i++)
        {
            in>>sheet[i].list_orn;
            in>>sheet[i].height;
            in>>sheet[i].size;
            in>>sheet[i].width;
        }
        file.close();
        curSheet=sheet[0];
        ind_stop();
        img_count=-1;
        show_pict();
    }
    return r;
}


// лента

void MainWindow::on_l1_2_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(0);
    recomp();
    ui->checkset_2->setGeometry(2,1,16,16);
    ui->label_17->setText(tr("Select: 1 in center"));
}

void MainWindow::on_l2_2_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(1);
    recomp();
    ui->checkset_2->setGeometry(42,1,16,16);
    ui->label_17->setText(tr("Select: 1 in top"));
}

void MainWindow::on_l3_2_clicked()
{
    w_cou=1;
    h_cou=2;
    btn_comp_press(2);
    img_on_list=2;
    recomp();
    ui->checkset_2->setGeometry(82,1,16,16);
    ui->label_17->setText(tr("Select: 1x2"));
}

void MainWindow::on_l4_2_clicked()
{
    img_on_list=3;
    w_cou=1;
    h_cou=3;
    btn_comp_press(3);
    recomp();
    ui->checkset_2->setGeometry(122,1,16,16);
    ui->label_17->setText(tr("Select: 1x3"));
}

void MainWindow::on_l5_2_clicked()
{
    img_on_list=4;
    w_cou=2;
    h_cou=2;
    btn_comp_press(4);
    recomp();
    ui->checkset_2->setGeometry(162,1,16,16);
    ui->label_17->setText(tr("Select: 2x2"));
}

void MainWindow::on_l6_2_clicked()
{
    img_on_list=6;
    w_cou=2;
    h_cou=3;
    btn_comp_press(5);
    recomp();
    ui->checkset_2->setGeometry(202,1,16,16);
    ui->label_17->setText(tr("Select: 2x3"));
}

void MainWindow::on_l7_2_clicked()
{
    img_on_list=8;
    w_cou=2;
    h_cou=4;
    btn_comp_press(6);
    recomp();
    ui->checkset_2->setGeometry(242,1,16,16);
    ui->label_17->setText(tr("Select: 2x4"));
}

void MainWindow::on_l8_2_clicked()
{
    img_on_list=9;
    w_cou=3;
    h_cou=3;
    btn_comp_press(7);
    recomp();
    ui->checkset_2->setGeometry(282,1,16,16);
    ui->label_17->setText(tr("Select: 3x3"));
}

void MainWindow::on_l9_2_clicked()
{
    img_on_list=15;
    w_cou=3;
    h_cou=5;
    btn_comp_press(8);
    recomp();
    ui->checkset_2->setGeometry(322,1,16,16);
    ui->label_17->setText(tr("Select: 3x5"));
}

void MainWindow::on_l10_2_clicked()
{
    img_on_list=20;
    w_cou=4;
    h_cou=5;
    btn_comp_press(9);
    recomp();
    ui->checkset_2->setGeometry(362,1,16,16);
    ui->label_17->setText(tr("Select: 4x5"));
}

void MainWindow::on_pushButton_38_clicked()
{
    if(fun)cout << "on_pushButton_cliked" << endl;
    for (int i=0; i<=buf; i++) toprint[i].list=1;
    btn_comp_press(10);
    img_on_list=buf+1;
    w_cou=h_cou=2;
    recomp();
    ui->checkset_2->setGeometry(402,1,16,16);
    ui->label_17->setText(tr("Select: All in oun"));
}

void MainWindow::on_pushButton_39_clicked() // my layout
{
    if(fun)cout << "on_pushButton_39_cliked" << endl;
    if(uslay == 0)
    {
        uslay=new userlayout();
        connect(uslay, SIGNAL(is_ok()), this, SLOT(set_user_layout()));
        uslay->make_preview();
    }
    uslay->show();
}

void MainWindow::on_pushButton_25_clicked()//открыть 1 файл
{
    if(fun)cout << "on_pushButton_25_cliked" << endl;
    QString fileName = get_file();
    if(fileName.isEmpty()) return;
    setty.beginGroup("Settings");
    setty.setValue("inPath", GetPathFrom(fileName));
    setty.endGroup();
    if (!fileName.isEmpty())
    {
        ind_start();
        flag2=true;
        open_pct(fileName);
        while (flag2) { QApplication::processEvents(); }
        show_pict();
        ind_stop();
    }
}

void MainWindow::on_pushButton_26_clicked() // открыть папку
{
    if(fun)cout << "on_pushButton_26_cliked" << endl;
    ind_start();
    QString hm="/home";
    if(pathFile)
    {
        setty.beginGroup("Settings");
        hm=setty.value("inPath","/home").toString();
        setty.endGroup();
    }
    QString dirf = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                     hm, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dirf.isEmpty())
    {
        ind_stop();
        return;
    }
    setty.beginGroup("Settings");
    setty.setValue("inPath", dirf);
    setty.endGroup();
    load_folder(dirf);
    show_pict();
    ind_stop();
}

void MainWindow::on_pushButton_30_clicked() // назад
{
    if(fun)cout << "on_pushButton_30_cliked" << endl;
    if(curlist>1)
    {
        for(int i=0; i<=buf; i++) toprint[i].cp_num=-1; // сброс связи каринок с превьюшками на экране
        curlist--;
        curSheet=sheet[curlist-1];
        show_pict();
    }
}

void MainWindow::on_pushButton_31_clicked() // вперед
{
    if(fun)cout << "on_pushButton_31_cliked" << endl;
    if (curlist<lists)
    {
        for(int i=0; i<=buf; i++) toprint[i].cp_num=-1; // сброс связи каринок с превьюшками на экране
        curlist++;
        if (int(sheet.size())<lists)
        {   // если лист открывается впервые, то добавить его в вектор и дать ему свойства предыдущего листа
            sheet.resize(curlist);
            sheet[curlist-1]=curSheet;
        }else{
            curSheet=sheet[curlist-1];
        }
        show_pict();
     }
}

void MainWindow::on_pushButton_35_clicked() // настройка программы
{
    if(fun)cout << "on_pushButton_35_cliked" << endl;
    if(ps == 0)
    {
        ps=new PageSetup();
        QFont font;
        font.setPointSize(fn_size);
        //ps->setFont(font);
        connect(ps, SIGNAL(end_set(int)), this, SLOT(set_setting(int)));
        connect(ps, SIGNAL(set_all_em(bool)), this, SLOT(set_all_rot(bool)));
    }
    ps->load_data();        // загрузить данные в окно
    ps->show();             // показать окно
    ps->set_path(pathFile); // установить состояние флага сохранения пути
    ps->reset_result();     // сбросить код возврата
}

void MainWindow::on_pushButton_3_clicked() // поменять ориентацию листа
{
     if(fun)cout << "on_pushButton_3_cliked -> set orn." << endl;
     end_rotation();
}

void MainWindow::slfunc1()
{
    img_size_cur_comp ();
    for(int i=0;i<=buf;i++)toprint[i].show=false;
    show_pict();
}


void MainWindow::on_checkBox_8_clicked(bool checked) // заполнить
{
    if(exitFlag)return;
    fillsize=checked;
    for(int i=0; i<=buf; i++) toprint[i].show=false;
    if (checked){
        if (comp==0){
            fill_all();
            toprint[toshow[0].buf].top=(h_fon-toprint[toshow[0].buf].height)/2;
            toprint[toshow[0].buf].left=(w_fon-toprint[toshow[0].buf].width)/2;
            reShow(0);
        }else{
            mem_pol=pol;
            pol=0;
            slfunc1();
        }
    }else{
        if(comp==0)
        {
            toprint[toshow[0].buf].show=false;
            recomp();
        }else{
            pol=mem_pol;
            slfunc1();
        }
    }
}


void MainWindow::on_checkBox_10_clicked(bool checked) // сохранять пропорции
{
    if(exitFlag)return;
    prop=checked;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
// быстрый выбор бумаги из ленты
{
    if(flag3) return;
    QString s="";
    sheets p;
    if(ui->comboBox->currentIndex()==0)
    {
        p.size=0;
        p.height=297;
        p.width=210;
        p.list_orn=true;
        s.append(&arg1);
    }
    else
    {
        int i=0;
        bool go=true;
        setty.beginGroup("Paper sizes");
        while(go)
        {
            s=setty.value("name"+QString::number(i), "").toString();
            if (&arg1==s){
                p.size=setty.value("index"+QString::number(i), 0).toInt();
                p.height=setty.value("H"+QString::number(i), 297).toDouble();
                p.width=setty.value("W"+QString::number(i), 210).toDouble();
                p.list_orn=true;
                go=false;
            }
            i++;
         }
        setty.endGroup();
    }
    list_n=s;
    sheet[0]=p;
    for(int i=0; i<lists; i++) sheet[i]=p;
    curSheet=p;
    make_list();
    recomp();
}

void MainWindow::on_dial_3_valueChanged(int value) // зазоры между миниатюрами
{
    if (pol==value || exitFlag) return;
    pol=value;
    slfunc1();
}

void MainWindow::on_dial_4_valueChanged(int value) // Плавное вращение миниатюры (и картинки)
{
    if(exitFlag)return;
    if (imgpress2 == -1 || out_rot) return;
    rotated(value);
}

void MainWindow::on_pushButton_clicked() // печать в PDF-файл
{
    pdf=true;
    print_color=true;
    print_hi=true;
    printAll();
    pdf=false;
}

void MainWindow::setNewSizeOfPct()
{
    QRect rc=toshow[imgpress2].pct->geometry();
    toprint[bufpress2].width=rc.width();
    toprint[bufpress2].height=rc.height();
    toprint[bufpress2].left=rc.left();
    toprint[bufpress2].top=rc.top();
    if(toprint[bufpress2].show_caption)
    {
        rc=tocaption[toprint[bufpress2].cp_num].pct->geometry();
        setCaptionRect(bufpress2,rc);
    }
}

void MainWindow::resizeTo(float c)
{
    if(imgpress2<0)return;
    quick_buttons_off();
    QRect rc;
    rc=toshow[imgpress2].pct->geometry();
    rc.setWidth(float(rc.width())*c);
    rc.setHeight(float(rc.height())*c);
    toshow[imgpress2].pct->setGeometry(rc);
    if(toprint[bufpress2].show_caption)
    {
        rc=tocaption[toprint[bufpress2].cp_num].pct->geometry();
        int x,y,h,w;
        x=float(rc.left())*c;
        y=float(rc.top())*c;
        h=rc.height();
        w=rc.width();
        rc.setRect(x,y,w,h);
        tocaption[toprint[bufpress2].cp_num].pct->setGeometry(rc);
    }
    setNewSizeOfPct();
}

void MainWindow::moveTo(int x, int y)
{
    if(imgpress2<0)return;
    quick_buttons_off();
    QRect rc=toshow[imgpress2].pct->geometry();
    int x1=rc.left()-x;
    int y1=rc.top()-y;
    int h=rc.height();
    int w=rc.width();
    rc.setRect(x1,y1,w,h);
    toshow[imgpress2].pct->setGeometry(rc);
    if(toprint[bufpress2].show_caption)
    {
        rc=tocaption[toprint[bufpress2].cp_num].pct->geometry();
        x1=rc.left()-x;
        y1=rc.top()-y;
        h=rc.height();
        w=rc.width();
        rc.setRect(x1,y1,w,h);
        tocaption[toprint[bufpress2].cp_num].pct->setGeometry(rc);
    }
    setNewSizeOfPct();
}

void MainWindow::showPctBord(bool b)
{
    if(b){
        if(imgFrame>-1)toshow[imgFrame].pct->setFrameStyle(QFrame::NoFrame);
        QPalette p=toshow[imgpress2].pct->palette();
        QColor cl;
        cl.setRgb(255,100,0);
        p.setColor( QPalette::WindowText,cl);
        toshow[imgpress2].pct->setPalette(p);
        toshow[imgpress2].pct->setFrameStyle(QFrame::Box | QFrame::Plain);
        toshow[imgpress2].pct->setLineWidth(1);
        imgFrame=imgpress2;
    }else{
        if(imgFrame>-1)toshow[imgFrame].pct->setFrameStyle(QFrame::NoFrame);
        imgFrame=-1;
    }
}

void MainWindow::on_pushButton_17_clicked() // увеличить выбранную картинку на 1%
    {resizeTo(1.01);}

void MainWindow::on_pushButton_18_clicked() // уменьшить выбранную картинку на 1%
    {resizeTo(0.99);}

void MainWindow::on_pushButton_19_clicked() // сдвинуть картинку влево
    {moveTo(1,0);}

void MainWindow::on_pushButton_20_clicked() // сдвинуть картинку вправо
    {moveTo(-1,0);}

void MainWindow::on_pushButton_21_clicked() // сдвинуть картинку вверх
    {moveTo(0,1);}

void MainWindow::on_pushButton_22_clicked() // сдвинуть картинку вниз
    {moveTo(0,-1);}

void MainWindow::on_pushButton_2_clicked() // загрузить активную картинку в окошко трансформатора
{
    if(deform==0){
        deform=new Qdeformation(this);
        connect(deform,SIGNAL(pixFin(QPixmap)),this,SLOT(setNewPix(QPixmap)));
    }
    deform->show();
    deform->setPixmap(toprint[bufpress2].pix);
}

void MainWindow::setNewPix(QPixmap p)
{
    toprint[bufpress2].pix=toprint[bufpress2].pix0=p;
    show_pict();
}

void MainWindow::print3() // печать ч/б
{
    print_color=false;
    print_hi=true;
    for(int i=1;i<=pgCou;i++) printAll();
}

void MainWindow::print2() // печать в низком качестве
{
    print_color=true;
    print_hi=false;
    for(int i=1;i<=pgCou;i++) printAll();
}

void MainWindow::on_pushButton_5_clicked() // открыть окно коррекции цвета
{
    if(repn==0){
        repn = new QRepaintPixmap(this);
        connect(repn,SIGNAL(endRepaint(QPixmap)),this,SLOT(setNewPix(QPixmap)));
    }
    repn->show();
    repn->setPixmap(toprint[bufpress2].pix);
}

// "опрозрачивание" текущей картинки к этой процедуре нужен регулятор, например слайдер
//void MainWindow::setTransp(int value)
//{
//    int a;
//    QRgb rgb;
//    QColor cl;
//    QImage im=toprint[bufpress2].pix.toImage().convertToFormat(QImage::Format_ARGB32,Qt::AutoColor);
//    for(int x=0;x<im.width();x++)
//        for(int y=0;y<im.height();y++){
//            rgb=im.pixel(x,y);
//            a=qAlpha(rgb);
//            a+=value-oldAA;
//            if(a>255)a=255;
//            if(a<0)a=0;
//            cl.setRgba(rgb);
//            cl.setAlpha(a);
//            im.setPixel(x,y,cl.rgba());
//        }
//    toprint[bufpress2].pix=toprint[bufpress2].pix0=QPixmap::fromImage(im);
//    show_pict();
//    oldAA=value;
//}

void MainWindow::on_pushButton_6_clicked() //обработка светом
{
    if(lght==0){
        lght = new lighter(this);
        connect(lght,SIGNAL(endLighter(QPixmap)),this,SLOT(setNewPix(QPixmap)));
    }
    lght->show();
    lght->setPixmap(toprint[bufpress2].pix);
}

void MainWindow::on_pushButton_7_clicked() // сохранить текущий лист как файл
{
    if(sfile==0){
        sfile=new toFile(this);
        connect(sfile,SIGNAL(end(QString,QString,int,QSize,bool)),this,SLOT(saveToFile(QString,QString,int,QSize,bool)));
    }
    sfile->setSheetSize(QSize(ui->sheet->width(),ui->sheet->height()));
    setty.beginGroup("Settings");
        QString hm=setty.value("inPath","/home").toString();
    setty.endGroup();
    sfile->setPath(hm);
    sfile->show();
}

void MainWindow::saveToFile(QString fName, QString form, int quality, QSize sz, bool t)
{
    if(fun)cout << "on_pushButton_7_cliked (to jpg)" << endl;
    QRect rc;           // пригодится ниже...
    QPixmap jpg(sz);    // итоговая картинка
    QPixmap pxm;        // пиксмэп для рисования
    QBrush br;          // кисть
    double sclX, sclY;  // коэф. масштабирования между бумагой и предросмотром при пересчете размеров и координат
    double px=0,py=0,ph,pw;
    if(t)jpg.fill(Qt::transparent);
    else jpg.fill(Qt::white);
    QPainter pnt;
    int i=curlist;
    bool f=pnt.begin(&jpg);
    if(!f){
        cout << "enable to image file :(" << endl;
        return;
    }
        rc=jpg.rect();    // размер pixmap
        px=0;py=0;
        // Расчет смещения области печати от края листа
        // масштаб между предпросмотром и pixmap (в пикселах)
        if(sheet[i].list_orn!=sheet[0].list_orn)swap(sheet[i].sheet_w,sheet[i].sheet_h);
        sclX=double(jpg.width())/double(ui->sheet->width());
        sclY=double(jpg.height())/double(ui->sheet->height());
        for(int j=0; j<=buf; j++)
        {
            if (toprint[j].list==curlist)
            {
                cout << "coordinates on the screen: x="<< toprint[j].left<<" y="<<toprint[j].top<<endl;
                    px=double(toprint[j].left)*sclX;
                    py=double(toprint[j].top)*sclY;
                    ph=double(toprint[j].height)*sclY;
                    pw=double(toprint[j].width) *sclX;
                    rc=QRect (px,py,pw,ph); // размер на выходе
                    pxm=toprint[j].pix.scaled(pw, ph, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //картинка в разрешении принтера
                    pnt.drawPixmap(rc, pxm, pxm.rect());
                    //caption
                    if(toprint[j].show_caption || ui->checkBox_12->isChecked())
                        if(toprint[j].widthCap!=0)
                        {
                            pnt.setFont(toprint[j].font);
                            rc.setTop(toprint[j].topCap*sclY);
                            rc.setLeft(toprint[j].leftCap*sclX);
                            rc.setWidth(toprint[j].widthCap*sclX);
                            rc.setHeight(toprint[j].heightCap*sclY);
                            br.setColor(toprint[j].font_color);
                            if(!toprint[j].trans)
                            {
                                br.setColor(toprint[j].back_color);
                                pnt.setBrush(br);
                                pnt.setBackgroundMode(Qt::OpaqueMode);
                                pnt.setBackground(br);
                                pnt.setBrush(Qt::SolidPattern);
                                pnt.drawRect(rc);
                            }
                            pnt.setPen(toprint[j].font_color);
                            pnt.drawText(rc, Qt::AlignLeft, toprint[j].caption);
                        }
             }
        }
   pnt.end(); // end
   if(jpg.save(fName,form.toStdString().c_str(),quality))
       cout << "End saving to image file. It seems that successfully." << endl;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    // Изменение количества копий выводимых на печать
    pgCou=arg1;
}

// **********************************
// обновления
void MainWindow::getVesion()
{
    QDate dt=QDate::currentDate();
    setty.beginGroup("Version");
    dt=setty.value("dateCheck",QDate(1999,1,1)).toDate();
    if(dt.daysTo(QDate::currentDate())>10 && (setty.value("checking", true).toBool())){
        doDownload("http://qvap.ru/version.html");
        setty.setValue("dateCheck",QDate::currentDate());
    }
    setty.endGroup();
}

void MainWindow::doDownload(QString urlLink)
{
    if(manager==0){
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    }
    manager->get(QNetworkRequest(QUrl(urlLink)));
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if(!(reply->error()))
        {
            QString ver=QString::fromStdString(reply->readAll().toStdString());
            int i=ver.indexOf(":");
            //if(i<0)return;
            int index=ver.left(i).toInt();
            ver=ver.right(ver.length()-i-1);
            cout << "The latest version of the program: " << ver.toStdString() << " (index=" << index << ")" << endl;
            if(vapIndex<index){
                cout << "Probably it is necessary to update the program" << endl;
                setty.beginGroup("Version");
                int lIndex=setty.value("lIndex",0).toInt();
                if(!setty.value("dontask", false).toBool() || lIndex<index){
                    if (rfsh==0){
                        rfsh=new refresh(this);
                    }
                    if (lIndex<index) {
                        rfsh->clearCheck();
                        setty.setValue("lIndex",index);
                    }
                    rfsh->show();

                }
                setty.endGroup();
            }
        }
    reply->deleteLater();
}
