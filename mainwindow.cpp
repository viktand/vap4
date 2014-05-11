#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagesetup.h"
#include "avLabel.h"
#include "userlayout.h"
#include "about.h"
#include "captioneditor.h"
#include "texteditor.h"
#include <math.h>
#include <qmath.h>

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

#define PI 3.14159265

#include <iostream>
using namespace std;

/*
Список размеров листов.
Каждый лист имеет свой размер бумаги, который задается в переменной int sheet[N].size,
где N=0,1,... - номер листа
Возможны следующие размеры и значения указанной переменной:

Constant        Value	Description
    A0          5       841 x 1189 mm
    A1          6       594 x 841 mm
    A2          7       420 x 594 mm
    A3          8       297 x 420 mm
    A4          0       210 x 297 mm, 8.26 x 11.69 inches
    A5          9       148 x 210 mm
    A6          10      105 x 148 mm
    A7          11      74 x 105 mm
    A8          12      52 x 74 mm
    A9          13      37 x 52 mm
    B0          14      1000 x 1414 mm
    B1          15      707 x 1000 mm
    B2          17      500 x 707 mm
    B3          18      353 x 500 mm
    B4          19      250 x 353 mm
    B5          1       176 x 250 mm, 6.93 x 9.84 inches
    B6          20      125 x 176 mm
    B7          21      88 x 125 mm
    B8          22      62 x 88 mm
    B9          23      33 x 62 mm
    B10         16      31 x 44 mm
    C5E         24      163 x 229 mm
    Comm10E     25      105 x 241 mm, U.S. Common 10 Envelope
    DLE         26      110 x 220 mm
    Executive	4       7.5 x 10 inches, 190.5 x 254 mm
    Folio       27      210 x 330 mm
    Ledger      28      431.8 x 279.4 mm
    Legal       3       8.5 x 14 inches, 215.9 x 355.6 mm
    Letter      2       8.5 x 11 inches, 215.9 x 279.4 mm
    Tabloid     29      279.4 x 431.8 mm
    Custom      30      sheet[N].width x sheet[N].heigth mm

ориентация листа указывается в переменной bool sheet[N].list_orn: true - портретная
Физические размеры в мм хранятся в переменных
    int     sheet[N].heigth - высота
    int     sheet[N].width  - ширина
*/


// глобальные переменные
QPrinter *printer;       // принтер, в него рисовать и настраивать
QPainter *pntr;          // устройство рисования (с него печатать)
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
QRect virt_paper;       // виртуальный лист бумаги, на котором рисуется картинка
                        // т.е. лист накладывается на А4 и печатается.
bool flag2;
int  count=0;
int	 img_count=-1; 	    // количество превьюшек
int  cpt_count=-1;      // количество подписей на текущем листе
int	 comp = 0;			// текущая компоновка
int  getx = 0;			// максимальный размер х для картинки  в этой компоновке
int  gety = 0;			// максимальный размер y для картинки  в этой компоновке
double  x_pg = 210;		// текущий размер листа x
double  y_pg = 297;		// текущий размер листа y
double  prt_x= 210;     // x для портретной ориентации
double  scl_pg = 2;		// масштаб листа предпросмотра
int  gor = 420;			// w листа предпросмотра
int  ver = 594;			// h листа предпросмотра
int  pg_mar = 10;		// поля листа бумаги
int  pol = 10;			// поля между картинок
int  crd[20][2];		// список стандартных координат для текущей компоновки
bool orn=true;          // текущая ориентация листа предпросмотра (не листа с картинками!) true - портретная
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
double list_scl;        // реальное соотношение размеров экрана и бумаги
bool pathFile=true;     // запоминать путь к последнему файлу и открывать диалоги там же
bool autoOrn=true;      // автоматический выбор ориентации бумаги
bool exitFlag=false;    // флаг выхода из функции, когда она вызывается, но выполнять ее не надо
bool fun=false;         // выдавать имена вызывемых функций в терминал

// поля бумаги
int left_m;
int right_m;
int top_m;
int bottom_m;
// ориентация
int ornl;
// размеры бумаги
int paper_w;
int paper_h;
bool rap;
bool all_rot;
// пользовательская компоновка
int ul_hor;
int ul_ver;
QString p_name; // имя принтера
int sheet_size;
bool all_sizes;
bool set_orn;
bool print_color;
QString list_n;
int prn_size_x;
int prn_size_y;
double h_ofsett;    // горизонтальное смещение позиции печати
int pap_sor;        // источник бумаги
int pap_name;
bool printer_a3;    // true - A3 (big size paper)
QString caption;
bool show_cap;
QColor font_cl;     // цвет шрифта подписи
QColor back_cl;     // цвет шрифта фона
QFont  font_cpt("Ubuntu"); // шрифт подписи
bool trans;         // прозрачный фон надписи
bool runShow=false; // флаг разрешения работы процедуры show_pict - чтобы избежать бесконечной рекурсии
double font_scl;
bool testPrint;     // печать тестового креста (для проверки позиционирования)
bool lenta=true;    // Использовать ленточный интерфейс
bool noResizewindow=false; // Запрет на изменение размера окна программы
bool rib;           // лента
bool ctrl=false;    // нажата клавиша ctrl
bool flag3=true;    // флаг, запрещающий реакцию на изменение размера бумаги из ленты

struct pict {                // загруженная картинка и все ее свойства
    QString   pict;          // путь к файлу
    int       rot;           // 0 - нормально, угол поворота
    QPixmap   pix0;          // исходный pixmap
    QPixmap   pix;           // текущий pixmap
    int       left;          // координата Х
    int       top;           // координата Y
    int       heigth;        // высота
    int       width;         // ширина
    int       show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
    int       list;          // номер страницы этой картинки
    int       load;          // флаг того, картинка уже загружена в память
    int       z;             // z-порядок
    int       xl;            // Х подписи
    int       yl;            // Y подписи
    char      fnt_Name[31];  // имя фонта подписи
    int       fnt_Size;      // размер фонта подписи
    int       dxl;           // относительное смещение положения подписи от картинки (по Х)
    int       dyl;           // относительное смещение положения подписи от картинки (по Y)
    int       prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
    double    compress;      // кэф. компрессии для уже показанной картинки
    // caption
    QString   caption;       // подпись к картинке
    bool      show_caption;  // показывать подпись
    //bool      frame;         // с рамкой
    QColor    back_color;    // цвет фона
    bool      trans;         // прозрачный фон
    QColor    font_color;    // цвет шрифта
    QFont     font;          // шрифт
    QRect     rect;          // геометрия подписи
    int       cpt;           // номер подписи на экране к этой картинке
    int       cp_num;        // номер avLabel - подписи на экране
    int       cp_z;          // z-орядок для подписи
    QPixmap   cp_pixmap;     // pixmap подписи
    bool      isTextBlock;   // true - это текстовый блок
    int       alig;          // выравнивание в текстовом болоке 0-1-2 -> слева-по центру-справа
};

struct prew {
    QavLabel     *pct;        //картинка
    int          buf;         //абсолютный номер
};

struct sheets {              // описание листа
    bool    list_orn;        // ориентация листа true -портретная
    int     size;            // размер листа - номер по списку размеров
    int     heigth;          // высота
    int     width;           // ширина
};

sheets   curSheet;           // описание текущего листа

QavLabel *rott;              // "ручка" вращения
QavLabel *resiz;             // "ручка" изменения размера
QavLabel *delt;              // кнопка удаления картинки
QavLabel *clip;              // кнопка ножницы


std::vector<pict> toprint;      //массив загруженных картинок
std::vector<prew> toshow;       //массив превьющек
std::vector<prew> tocaption;    //массив подписей
std::vector<prew> totext;       //массив надписей (блоков текста)
//std::vector<bool> list_orn;     // массив ориентаций листов true - портретная ориентация
std::vector<sheets> sheet;      // массив описаний листов


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  // конструктор
{
    if(fun)cout << "MainWindow (make window and widgets)" << endl;
    ui->setupUi(this);
    this->setAcceptDrops(true);
    fon=new QavLabel(ui->sheet);
    connect(fon, SIGNAL(mouse_press(int,int,int)), this, SLOT(show_paper_size()));
    connect(fon, SIGNAL(mouse_wheel(int, int)), this, SLOT(mouseWheel(int, int)));
    fon->setImnum(-1);
    list_n.append("A4 210x297 mm");
    fon->show();
    all_sizes=true;
    print_color=true;
    set_orn = true;
    all_rot=true;
    ul_hor=3;
    ul_ver=3;
    h_ofsett=0;
    pap_sor=0;
    testPrint=false;
    font_scl=1;
    ui->pushButton_13->hide();
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    printer = new QPrinter(QPrinter::HighResolution);
    pntr = new QPainter();
    if (printer->orientation() == QPrinter::Portrait) ornl=1; else ornl=0;
    printer->setPageSize(QPrinter::A4);
    rap=true;
    animGif = new QMovie(":/new/prefix1/indicator");
    ui->label->setMovie(animGif);
    // Скрыть элемeнты управления на период тормозов при загрузке fhhdffffffddddddfffffffff
    hide_interf();
    // кнопки по углам превьюшек:
    set_indic_pos();
    set_rott_btn();
    set_delt_btn();
    set_resiz_btn();
    set_clip_btn();
    set_timer();            // таймер, чтобы гасить кнопки
    set_btn_pos(0);         // распихать кнопки управления программой по своим местам
    rest_sind_size();       // восстановить размеры и положение окна программы
    rest_printer_sett();    // восстановить настройки принтера
    // стартовые параметры листа (по умолчанию)
    curSheet.size=pap_name;
    curSheet.list_orn=true;
    curSheet.width=paper_w;
    curSheet.heigth=paper_h;
    sheet.push_back(curSheet);
    get_marg();
    rest_view_sett();       // восстановить компоновку
    ui->pushButton_6->hide();
    ui->pushButton_32->hide();
    double d1, d2;
    d1=y_pg=paper_h;
    d2=x_pg=paper_w;
    paper_ratio=d1/d2;
    prn_size_y=297;
    prn_size_x=210;
    show_paper_size();
    gor_old=gor;
    setty.beginGroup("Settings");
    pathFile=setty.value("path",true).toBool();
    autoOrn=setty.value("autoOrn", true).toBool();
    lenta=setty.value("ribbon", true).toBool();
    setty.endGroup();
    //setInterface();
    load_my_pSizes();
    flag3=false;
}

void MainWindow::hide_interf()
{
    ui->tabWidget_2->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->scrollArea->setVisible(false);
    ui->scrollArea_2->setVisible(false);
    ui->pushButton_32->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_16->setGeometry(5,20,451,20);
}

void MainWindow::set_rott_btn() // создать кнопку вращения картинки
{
    rott = new QavLabel(fon);
    rott->setImnum(-1);
    QImage im(":/new/prefix1/rotation");
    rott->setPixmap(QPixmap::fromImage(im));
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
    ui->comboBox->setCurrentIndex(n);
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
           on_pushButton_3_clicked();
        }
        else
        {
            on_pushButton_4_clicked();
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
    toprint[bufpress2].heigth=rc.height();
    toprint[bufpress2].top=rc.top();
    toprint[bufpress2].left=rc.left();

    // begin caption
    x4=y4=0;
    if(rc2.top()<toprint[bufpress2].rect.top()) y4=rc2.height()-rc.height();
    if(rc2.left()<toprint[bufpress2].rect.left()) x4=rc2.width()-rc.width();
    rc=toprint[bufpress2].rect;
    x3=rc.left()-x4;
    y3=rc.top()-y4;
    int h=rc.height();
    int w=rc.width();
    rc.setLeft(x3);
    rc.setTop(y3);
    rc.setWidth(w);
    rc.setHeight(h);
    toprint[bufpress2].rect=rc;
    if(toprint[bufpress2].show_caption || ui->checkBox_6->isChecked())
    {
        tocaption[toprint[bufpress2].cp_num].pct->move(x3,y3);
        tocaption[toprint[bufpress2].cp_num].pct->raise();
    }
}


void MainWindow::setInterface()
{
    // Установить вид интерфейса программы в соответствии с настройками
    if(fun)cout << "setInterface" << endl;
    ui->tabWidget_2->setVisible(lenta);
    ui->tabWidget_2->setGeometry(1,1,611,69);
    ui->tabWidget->setVisible(!lenta);
    ui->scrollArea->setVisible(!lenta);
    ui->scrollArea_2->setVisible(!lenta);
    ui->pushButton_32->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_16->setVisible(false);
    if(lenta)
    {
        ui->label_2->setGeometry(300,5,211,16);
        ui->label_3->setGeometry(400,5,211,16);
        QRect rc=ui->sheet->geometry();
        rc.setLeft(1);
        ui->sheet->setGeometry(rc);
        ui->tabWidget_2->setCurrentIndex(0);
    }
    else
    {
        ui->label_2->setGeometry(170,50,211,16);
        ui->label_3->setGeometry(270,50,211,16);
        QRect rc=ui->sheet->geometry();
        rc.setLeft(150);
        ui->sheet->setGeometry(rc);
        ui->tabWidget->setCurrentIndex(0);
    }
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
    gor_old=gor;
    wind_sz=e->size();
    make_list();
    show_pict();
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

void MainWindow::get_marg()
{
    if(fun)cout << "get_marg" << endl;
    setty.beginGroup("Settings");
    if(setty.value("without_m", false).toBool())
    {
        left_m=right_m=top_m=bottom_m=0;
        ui->checkBox_13->setChecked(true);
    }
    else
    {
        top_m=setty.value("top_m", -5).toInt();
        if(top_m==-5)
        {
        QRect pap=printer->paperRect();
        QRect pag=printer->pageRect();
        left_m=right_m=double((pap.width()-pag.width())/2)/(double(printer->resolution())/25.4);
        top_m=bottom_m=double((pap.height()-pag.height())/2)/(double(printer->resolution())/25.4);
        }
        else
        {
            left_m=setty.value("left_m", 5).toInt();
            right_m=setty.value("right_m", 5).toInt();
            bottom_m=setty.value("bottom_m", 5).toInt();
        }
    }
    setty.endGroup();
}

void MainWindow::reScl()
{
    if(fun)cout << "reScl" << endl;
    double scl=double(ui->list->widthMM())/double(paper_w)/list_scl;
    int l,h,w,t;
    for (int i=0; i<=buf; i++)
    {
        l=double(toprint[i].rect.left())*scl;
        t=double(toprint[i].rect.top())*scl;
        w=toprint[i].rect.width()*scl;
        h=toprint[i].rect.height()*scl;
        toprint[i].rect.setLeft(l);
        toprint[i].rect.setTop(t);
        toprint[i].rect.setWidth(w);
        toprint[i].rect.setHeight(h);
    }
    list_scl=double(ui->list->widthMM())/double(paper_w);
}

void MainWindow::setIconOrns(bool b)
{
    if(b)
    {
        ui->pushButton_10->setIcon(ui->pushButton_13->icon());
        ui->pushButton_11->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
        ui->pushButton_33->setIcon(ui->pushButton_13->icon());
        ui->pushButton_34->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    }
    else
    {
        ui->pushButton_11->setIcon(ui->pushButton_13->icon());
        ui->pushButton_10->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
        ui->pushButton_34->setIcon(ui->pushButton_13->icon());
        ui->pushButton_33->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    }
}


void MainWindow::on_pushButton_11_clicked() // ландшафтная ориентация
{
    if(fun)cout << "on_pushButton10_cliked" << endl;
    if(!sheet[curlist-1].list_orn) return;  // ориентация уже ландшафтная
    orn=sheet[curlist-1].list_orn=curSheet.list_orn=false;
    swap(paper_h, paper_w);
    swap(x_pg, y_pg);
    setIconOrns(false);
    end_rotation();
}

void MainWindow::on_pushButton_10_clicked() // портретная ориентация
{
    if(fun)cout << "on_pushButton10_cliked" << endl;
    if(sheet[curlist-1].list_orn) return;  // ориентация уже портретная
    orn=sheet[curlist-1].list_orn=curSheet.list_orn=true;
    swap(paper_h, paper_w);
    swap(x_pg, y_pg);
    setIconOrns(true);
    end_rotation();
}

void MainWindow::end_rotation() // эта функция требует переосмысления...
{
    if(fun)cout << "end_rotation" << endl;
    curSheet=sheet[curlist-1];
    swap(gor,ver);
    make_list();
    if (ui->checkBox->checkState()) for (int i=0; i<lists; i++) sheet[i].list_orn=curSheet.list_orn;
    btn_comp_press(comp);
    if (all_rot)
    {
        //for(int i=0; i<lists; i++) sheet[i].list_orn=curSheet.list_orn;
        if(!runShow)recomp();
    }
        else
    {
        //sheet[curlist-1].list_orn=(paper_w<paper_h);
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
    reScl();
    orn=curSheet.list_orn;
}

void MainWindow::redraw()
{
    // изменение размера окна программы
    // расчет окна предпросмотра
    if(fun)cout << "redraw" << endl;
    double x, y, w, h;                 //будущий рект окна
    y=curSheet.heigth;
    x=curSheet.width;
    if(!curSheet.list_orn) swap(x,y);
    int lft=150;
    if(lenta) lft=10;
    paper_ratio=x/y;
    w=x=wind_sz.width()-(lft+5);       //допустимая область для размещения
    h=y=wind_sz.height()-75;           // --
    if (w/paper_ratio>h) w=h*paper_ratio;
    else h=w/paper_ratio;
    x=(x-w)/2.0;
    y=(y-h)/2.0;
    x_prw=x+lft;
    y_prw=((wind_sz.height()-75)-h)/2+70;
    gor=w;
    ver=h;
    scl_pg=w/297.0;
    layout_scale();
    ui->label_3->setText(list_n);
    setIconOrns(h>=w);
}

void MainWindow::layout_scale()
{
    if(fun)cout << "layout_scale" << endl;
    if(gor_old==gor) return;
    double k=double(gor)/double(gor_old);
    for(int i=0; i<=buf; i++)
    {
        if(toprint[i].show==1)
        {
            toprint[i].left=k*double(toprint[i].left);
            toprint[i].top=k*double(toprint[i].top);
            toprint[i].heigth=k*double(toprint[i].heigth);
            toprint[i].width=k*double(toprint[i].width);
        }
    }
    gor_old=gor;
}

void MainWindow::get_pp() // получить количество точек на мм для текущего предпросмотра
{
    if(fun)cout << "get_pp" << endl;
    if(sheet[curlist-1].list_orn)
    {
        ppx=double(ui->list->width())/double(sheet[curlist-1].width);
        ppy=double(ui->list->height())/double(sheet[curlist-1].heigth);
    }
    else
    {
        ppx=double(ui->list->width())/double(sheet[curlist-1].heigth);
        ppy=double(ui->list->height())/double(sheet[curlist-1].width);
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
        if (fn.contains("file:///")) fn=esc_to_utf(fn);
        if(i+1<cou_prm)
        {
            if((prm[i+1].mid(0,1)!="/") && ((prm[i+1].mid(0,8)!="file:///")))
            {
                fl_no=false;
                fn.append(" ");
            }
        }
        if (fl_no)
        {
            fi.setFile(fn);
            cout << "fn: " << fn.toStdString() << endl;
            if(fi.isFile())
            {
                open_pct(fn);
            }
            else //папка
            {
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
    if (rap) cout << "File to open: " <<  filename.toStdString() << endl;
        buf++;
        if(lists==0)
        {
            lists=1;
            curlist=1;
        }
        str_time.clear();
        str_time.append(filename);
        toprint.push_back(pict());
        toprint[buf].pict=filename;
        toprint[buf].caption=get_name(filename);
        toprint[buf].show_caption=ui->checkBox_6->isChecked();
        toprint[buf].back_color=Qt::white;    // цвет фона
        toprint[buf].font_color=Qt::black;    // цвет шрифта
        toprint[buf].trans=true;
        toprint[buf].isTextBlock=false;
        QFont fn;
        fn.setPointSize(10);
        fn.setFamily("Sans");
        toprint[buf].font=fn;          // шрифт
        toprint[buf].cp_num=-1;        // нет привязанного avLabel
        QRect rc;
        rc.setRect(0,0,0,0);
        toprint[buf].rect=rc;          // начальная геометрия подписи
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
        toprint[buf].show=0;
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


void MainWindow::setAutoOrn()
{
    // Автоматически установить ориентацию бумаги
    if(fun)cout << "setAutoOrn" << endl;
    if(toprint[0].pix.height()>=toprint[0].pix.width())
    {
        // портретная ориентация
        on_pushButton_10_clicked();
    }
    else
    {
        // ландшафтная ориентация
        on_pushButton_11_clicked();
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

void MainWindow::on_pushButton_2_clicked() //открыть 1 файл
{
    if(fun)cout << "on_pushButton2_cliked" << endl;
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

void MainWindow::on_pushButton_12_clicked() // открыть папку
{
   if(fun)cout << "on_pushButton12_cliked" << endl;
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

void MainWindow::on_pushButton_clicked() // all in oun
{
   if(fun)cout << "on_pushButton_cliked" << endl;
   for (int i=0; i<=buf; i++) toprint[i].list=1;
   btn_comp_press(10);
   img_on_list=buf+1;
   w_cou=h_cou=2;
   recomp();
   ui->label_10->setText(tr("All in oun"));
   ui->checkset_2->setGeometry(402,1,16,16);
   ui->label_17->setText(tr("Select: All in oun"));
}

void MainWindow::on_pushButton_8_clicked() // my layout
{
    if(fun)cout << "on_pushButton8_cliked" << endl;
    if(uslay == 0)
    {
        uslay=new userlayout();
        connect(uslay, SIGNAL(is_ok()), this, SLOT(set_user_layout()));
        uslay->make_preview();
    }
    uslay->show();
 }


void MainWindow::on_pushButton_4_clicked() // вперед
{
    if(fun)cout << "on_pushButton4_cliked" << endl;
    if (curlist<lists)
    {
        for(int i=0; i<=buf; i++) toprint[i].cp_num=-1;
        curlist++;
        if (int(sheet.size())<lists) sheet.push_back(sheet.back());
        curSheet=sheet[curlist-1];
        show_pict();
     }
}

void MainWindow::on_pushButton_3_clicked() // назад
{
    if(fun)cout << "on_pushButton3_cliked" << endl;
    if(curlist>1)
    {
        for(int i=0; i<=buf; i++) toprint[i].cp_num=-1;
        curlist--;
        curSheet=sheet[curlist-1];
        show_pict();
    }
}

void MainWindow::on_pushButton_7_clicked() // настройка программы
{
    if(fun)cout << "on_pushButton7_cliked" << endl;
    if(ps == 0)
    {
        ps=new PageSetup();
        QFont font;
        font.setPointSize(fn_size);
        ps->setFont(font);
        connect(ps, SIGNAL(end_set(int)), this, SLOT(set_setting(int)));
        connect(ps, SIGNAL(set_all_em(bool)), this, SLOT(set_all_rot(bool)));
    }
    ps->load_data();        // загрузить данные в окно
    ps->show();             // показать окно
    ps->set_path(pathFile); // установить состояние флага сохранения пути
    ps->reset_result();     // сбросить код возврата
}

void MainWindow::on_checkBox_clicked(bool checked) // общая ориентация для всех листов
{
    if(fun)cout << "on_checkBox_cliked" << endl;
    if(exitFlag)return;
    all_rot=checked;
    if(ps!=0) ps->set_all();
    if (checked)
    {
        for(int i=0; i<=buf; i++)  if (sheet[toprint[i].list].list_orn != curSheet.list_orn) toprint[i].show=0;
        for(int i=0; i<lists; i++) sheet[i].list_orn=curSheet.list_orn;
    }
    exitFlag=true;
    ui->checkBox_7->setChecked(checked);
    ui->checkBox->setChecked(checked);
    exitFlag=false;
}

void MainWindow::set_all_rot(bool b)
{
   if(fun)cout << "set_all_rot" << endl;
   all_rot=b;
   ui->checkBox->setChecked(b);
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

void MainWindow::set_printer_pap_size(int i)
// настройка размера бумаги в принтере для листа № i
{
    switch (sheet[i].size)
    {
        case 5: printer->setPaperSize(QPrinter::A0); break;
        case 6: printer->setPaperSize(QPrinter::A1); break;
        case 7: printer->setPaperSize(QPrinter::A2); break;
        case 8: printer->setPaperSize(QPrinter::A3); break;
        case 0: printer->setPaperSize(QPrinter::A4); break;
        case 9: printer->setPaperSize(QPrinter::A5); break;
        case 10: printer->setPaperSize(QPrinter::A6); break;
        case 11: printer->setPaperSize(QPrinter::A7); break;
        case 12: printer->setPaperSize(QPrinter::A8); break;
        case 13: printer->setPaperSize(QPrinter::A9); break;
        case 14: printer->setPaperSize(QPrinter::B0); break;
        case 15: printer->setPaperSize(QPrinter::B1); break;
        case 17: printer->setPaperSize(QPrinter::B2); break;
        case 18: printer->setPaperSize(QPrinter::B3); break;
        case 19: printer->setPaperSize(QPrinter::B4); break;
        case 1: printer->setPaperSize(QPrinter::B5); break;
        case 20: printer->setPaperSize(QPrinter::B6); break;
        case 21: printer->setPaperSize(QPrinter::B7); break;
        case 22: printer->setPaperSize(QPrinter::B8); break;
        case 23: printer->setPaperSize(QPrinter::B9); break;
        case 16: printer->setPaperSize(QPrinter::B10); break;
        case 24: printer->setPaperSize(QPrinter::C5E); break;
        case 25: printer->setPaperSize(QPrinter::Comm10E); break;
        case 26: printer->setPaperSize(QPrinter::DLE); break;
        case 4: printer->setPaperSize(QPrinter::Executive); break;
        case 27: printer->setPaperSize(QPrinter::Folio); break;
        case 28: printer->setPaperSize(QPrinter::Ledger); break;
        case 3: printer->setPaperSize(QPrinter::Legal); break;
        case 2: printer->setPaperSize(QPrinter::Letter); break;
        case 29: printer->setPaperSize(QPrinter::Tabloid); break;
        default:
                printer->setPaperSize(QPrinter::A4);
                qreal l, b;
                if (sheet[i].list_orn){
                    l=(210-sheet[i].width)/2;
                    b=297-sheet[i].heigth;
                    printer->setPageMargins(l,0,l,b,QPrinter::Millimeter);
                }else{
                    l=297-sheet[i].heigth;
                    b=(210-sheet[i].width)/2;
                    printer->setPageMargins(l,b,0,b,QPrinter::Millimeter);
                }

    }
 }

void MainWindow::set_printer(int index) // настройка принтера
{
     if(fun)cout << "set_printer" << endl;
     printer->setFullPage(true);
     printer->setPrinterName(p_name);
     printer->setDocName("vap_pictures_"+QString::number(index));
     if (print_color) printer->setColorMode(QPrinter::Color);
         else printer->setColorMode(QPrinter::GrayScale);
     printer->setOutputFormat(QPrinter::NativeFormat);
     if(sheet[index].list_orn) printer->setOrientation(QPrinter::Portrait);
     else printer->setOrientation(QPrinter::Landscape);
     set_printer_pap_size(index); // установить размер бумаги для этого листа
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
        show_pict();
    }
    for(int p=0; p<=buf; p++) toprint[p].cp_num=-1;
    curlist=j;
    show_pict();
}

void MainWindow::on_pushButton_5_clicked() //печать
{
    if(fun)cout << "on_pushButton5_cliked (printing)" << endl;
    prePint();     // прогон листов перед печатью
    if(testPrint)
    {
        tstPrin();
        return;
    }
    QRect rc;     // пригодится ниже...
    QBrush br;         // кисть
    QPixmap pxm;       // пиксмэп для печати
    double sclX, sclY; // коэф. масштабирования между бумагой и предросмотром при пересчете размеров и координат
    double px=0,py=0,ph,pw,x,y;
    double dx, dy;     // Смещение области печати от края листа бумаги
    for (int i=0; i<lists; i++)
    {
        set_printer(i);             // настроить принтер
        rc=printer->paperRect();    // размер бумаги
        if(!pntr->begin(printer))   // начать рисование
        {
            cout << "Attempt to access the printer caused the error." << endl;
            QMessageBox::critical(NULL,QObject::tr("Error"),tr("Printer not found! Check the settings and printer availability."));
            return;
        }

        // Расчет смещения области печати от края листа
        qreal left, top, right, bottom, b2=0;
        printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::DevicePixel);
        cout << "maggins: " << left << ", " << top << ", " << right << ", " << bottom << endl;
        dx=left; dy=top;
        // масштаб между предпросмотром и бумагой (в пикселах)
        if(sheet[i].size<30){
        sclX=double(pntr->viewport().width())/double(ui->sheet->width());
        sclY=double(pntr->viewport().height())/double(ui->sheet->height());
        } else {
            if(sheet[i].list_orn){
                x=pntr->viewport().width()/(210.0/double(sheet[i].width));
                y=pntr->viewport().height()/(297.0/double(sheet[i].heigth));
                b2=bottom;
            }else{
                x=pntr->viewport().width()/(297.0/double(sheet[i].heigth));
                y=pntr->viewport().height()/(210.0/double(sheet[i].width));

            }
            sclX=x/double(ui->sheet->width());
            sclY=y/double(ui->sheet->height());
            px=dx;
            py=dy;
         }
        int hh=rc.height();   // высота облаcти печати
        int ww=rc.width();    // ширина области печати
        cout << "page left = " << rc.left() << " top = "<< rc.top() << endl;
        cout << "dx = " << dx << "; dy = " << dy << endl;
        cout << "sclX = " << sclX << "; sclY = " << sclY << endl;
        cout << "Page: rect size: heigth = "<< rc.height() << "; width = " << rc.width() << endl;

        double ppx=px, ppy=py;
        for(int j=0; j<=buf; j++)
        {
            if (toprint[j].list==i+1)
            {
                    px=ppx+double(toprint[j].left)*sclX;
                    py=ppy+double(toprint[j].top)*sclY;
                    ph=double(toprint[j].heigth)*sclY;
                    pw=double(toprint[j].width) *sclX;
                    cout << "px="<< px<<" py="<<py<<endl;
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
                    if(toprint[j].show_caption || ui->checkBox_6->isChecked())
                        if(toprint[j].rect.width()!=0)
                        {
                            pntr->setFont(toprint[j].font);
                            rc.setTop(toprint[j].rect.top()*sclY);
                            rc.setLeft(toprint[j].rect.left()*sclX);
                            rc.setWidth(toprint[j].rect.width()*sclX);
                            rc.setHeight(toprint[j].rect.height()*sclY);
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
            pntr->end(); // отправить на печать
    }
   cout << "end printing" << endl;
}

void MainWindow::tstPrin() // test print
{
    if(fun)cout << "tstPrint" << endl;
    pntr->begin(printer); // начать рисование
    double sclX=get_scaleX();
    int w=ui->sheet->rect().width()*sclX;
    double sclY=get_scaleY();
    int h=ui->sheet->rect().height()*sclY;
    QRect rc(0,0,w,h);
    pntr->drawRect(rc);
    pntr->drawLine(0,0,w,h);
    pntr->drawLine(0,h,w,0);
    pntr->end();
    cout << "Test printing is end" << endl;
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
    if(toprint[toshow[i].buf].show_caption || ui->checkBox_6->isChecked())
    {
        tocaption[toprint[toshow[i].buf].cp_num].pct->raise();
        toprint[toshow[i].buf].cp_z=++curz;
    }
    ttx=x;
    tty=y;
    imgpress=i;
    imgpress2=i;
    bufpress=toshow[i].buf;
    if (!ui->checkBox_2->isChecked()) return;
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
    ry=toprint[bufpress].top-8+toprint[bufpress].heigth;
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
    ui->label_7->setText(QString::number(ui->dial_2->value()));
    show_pict_size();
}

void MainWindow::pct_move(int x, int y, int i)
{
        //if(fun)cout << "pct_move" << endl;
        int x1=0, y1=0, dy, cx, x2=0, y2=0;
        x1=x2=ttx;
        y1=y2=tty;
        ttx = x;
        tty = y;
        QRect rc=toprint[bufpress].rect;
        x2=rc.left()+ttx-x2;
        y2=rc.top()+tty-y2;
        int w=rc.width();
        int h=rc.height();
        rc.setLeft(x2);
        rc.setTop(y2);
        rc.setWidth(w);
        rc.setHeight(h);
        toprint[bufpress].rect=rc;
        if(toprint[bufpress].show_caption || ui->checkBox_6->isChecked())
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
        y1=y1+toprint[bufpress].heigth-8;
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
            toprint[i].show=0;
            toprint[i].cp_num=-1;
            rc.setRect(0,0,0,0);
            toprint[i].rect=rc;
        }
    show_pict();
}


void MainWindow::set_setting(int r) // применить настройки
{
    if(fun)cout << "set_setting" << endl;
    if(r==0)return;
    load_my_pSizes();
    if(r==1)return;
    QString ss=list_n;
    save_printer_sett();
    curSheet.size=sheet_size;
    curSheet.heigth=paper_h;
    curSheet.width=paper_w;
    if (all_sizes)
    {
        for(int i=0; i<lists; i++)
        {
            sheet[i].size=sheet_size;
            sheet[i].heigth=paper_h;
            sheet[i].width=paper_w;
        }
    }
    if(x_pg!=paper_w)
    {
        x_pg=paper_w;
        y_pg=paper_h;
        if(paper_h>=paper_w)
        {
            on_pushButton_10_clicked(); // портретная ориентация
        }
        else
        {
            on_pushButton_11_clicked();
        }
    }
    setty.beginGroup("Settings");
    pathFile=setty.value("path", false).toBool();
    bool rib=setty.value("ribbon", true).toBool();
    ui->checkBox_13->setChecked(setty.value("without_m", false).toBool());
    setty.endGroup();
    if (rib!=lenta)
    {
        lenta=rib;
        setInterface();
        show_pict();
    }
    make_list();
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


int getmx()
// получить макимальный размер Х для текущей компоновки
{
  int i=0;
  i=w_fon/w_cou-pol*(w_cou-1);
  return i;
}

int getmy()
// получить макимальный размер Y для текущей компоновки
{
 int i=0, n=1;
 if(comp==1) n=2;
 i=h_fon/h_cou/n-pol*(h_cou*n-1);
 return i;
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
    // ставим метку на кнопку
     ui->checkset->move(10, comp*80+5-offset);
}

void MainWindow::set_user_layout()
{
    if(fun)cout << "set_use_layout" << endl;
    img_on_list=ul_hor*ul_ver;
    w_cou=ul_hor;
    h_cou=ul_ver;
    if (set_orn) on_pushButton_10_clicked();
    else on_pushButton_11_clicked();
    btn_comp_press(11);
    recomp();
    ui->label_10->setText(QString::number(w_cou) + " x " + QString::number(h_cou));
    ui->checkset_2->setGeometry(442,1,16,16);
    ui->label_17->setText(tr("Select: ")+QString::number(w_cou) + "x" + QString::number(h_cou));
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

void MainWindow::on_dial_valueChanged(int value) // поменять зазоры между миниатюрами
{
    if (pol==value || exitFlag) return;
    pol=value;
    img_size_cur_comp ();
    ui->label_5->setText(QString::number(value));
    for(int i=0; i<=buf; i++) toprint[i].show=0;
    show_pict();
    exitFlag=true;
    ui->dial_3->setValue(value);
    exitFlag=false;
}

void MainWindow::set_ornt_list()
// повернуть лист (при пролистывании)
{
    if(fun)cout << "set_orn_list" << endl;
    if (orn!=curSheet.list_orn) make_list(); // если старая оринтация не соотв новой, то создать новый лист
    if(curSheet.list_orn==sheet[curlist-1].list_orn)return;
    if(sheet[curlist-1].list_orn) on_pushButton_10_clicked();
       else on_pushButton_11_clicked();
}

void MainWindow::show_pict() // показать картинки текущего листа
{
    if((buf<0)||runShow) return;
    kill_pict(); // очистить лист предпросмотра
    runShow=true;
    if (orn!=curSheet.list_orn) make_list(); // если старая оринтация не соотв новой, то создать новый лист
    //set_ornt_list();
    if (buf>-1) quick_buttons_off();
    QSize sz;
    double d1, d2;
    int x, y, n=1;
    if(comp==1) n=2;
    curz=max_z(curlist);
    for (int i=0; i<buf+1; i++)
    {
      if (toprint[i].list==curlist)
      {   // создать превью
          img_count++;
          toprint[i].prew=img_count;
          toshow.push_back(prew());
          toshow[img_count].buf=i;
          toshow[img_count].pct = new QavLabel(fon);
          toshow[img_count].pct->setBackgroundRole(QPalette::Base);
          toshow[img_count].pct->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
          toshow[img_count].pct->setScaledContents(true);
          toshow[img_count].pct->setPixmap(toprint[i].pix);
          toshow[img_count].pct->setCursor(Qt::OpenHandCursor);
          toshow[img_count].pct->setImnum(img_count);
          toshow[img_count].pct->setAttribute(Qt::WA_DeleteOnClose);
          toshow[img_count].pct->show();
          // сигналы
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
          if (toprint[i].show==0)
            {   // картинка рисуется первый раз
                sz=setsize(toshow[img_count].pct->pixmap()->size());
                if (img_count<(w_cou*h_cou) and comp!=10)
                {
                    x=(img_count%w_cou)*(w_fon/w_cou)+(getx-sz.width())/2;
                    y=ceil(img_count/w_cou)*(h_fon/(h_cou*n))+(gety-sz.height())/2;
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
                    y=(h_fon-toprint[i].heigth)/2;
                    x=(w_fon-toprint[i].width)/2;
                    sz.setWidth(toprint[i].width);
                    sz.setHeight(toprint[i].heigth);
                }
                toshow[img_count].pct->setGeometry(x, y, sz.width(), sz.height());
                d1=toprint[i].pix.width();
                d2=sz.width();
                toprint[i].compress=d1/d2;
                toprint[i].left=x;
                toprint[i].top=y;
                toprint[i].z=++curz;
                toprint[i].show=1;
                toprint[i].width=sz.width();
                toprint[i].heigth=sz.height();
              }
        else
          {  // картинка уже была нарисована
              x=toprint[i].left;
              y=toprint[i].top;
              toshow[img_count].pct->setGeometry(x, y,toprint[i].width, toprint[i].heigth);
              toprint[i].compress=double(toprint[i].pix.width())/double(toprint[i].width);
          }
          show_caption(i,(toprint[i].show_caption));
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
            if(toprint[j].list==curlist && toprint[j].cp_z==i &&
                   (toprint[j].show_caption || ui->checkBox_6->isChecked()) )
                        tocaption[toprint[j].cp_num].pct->raise(); // поднять выше всех - caption

        }
}


void MainWindow::recomp()
{
    double d;
    QRect rc;
    for(int i=0; i<=buf; i++)
    {
        toprint[i].show=0;
        d=i+1;
        d=ceil(d/double(img_on_list));
        toprint[i].list=d;
        toprint[i].cp_num=-1;
        rc.setRect(0,0,0,0);
        toprint[i].rect=rc;
    }
    lists=0;
    for(int i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
    curlist=1;
    show_pict();
    save_view_sett();
}

void MainWindow::on_l1_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(0);
    recomp();
    ui->label_10->setText(tr("1 in center"));
    ui->checkset_2->setGeometry(2,1,16,16);
    ui->label_17->setText(tr("Select: 1 in center"));
}


void MainWindow::on_l2_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(1);
    recomp();
    ui->label_10->setText(tr("1 in top"));
    ui->checkset_2->setGeometry(42,1,16,16);
    ui->label_17->setText(tr("Select: 1 in top"));
}

void MainWindow::on_l3_clicked()
{
    w_cou=1;
    h_cou=2;
    btn_comp_press(2);
    img_on_list=2;
    recomp();
    ui->label_10->setText("1 x 2");
    ui->checkset_2->setGeometry(82,1,16,16);
    ui->label_17->setText(tr("Select: 1x2"));
}

void MainWindow::on_l4_clicked()
{
    img_on_list=3;
    w_cou=1;
    h_cou=3;
    btn_comp_press(3);
    recomp();
    ui->label_10->setText("1 x 3");
    ui->checkset_2->setGeometry(122,1,16,16);
    ui->label_17->setText(tr("Select: 1x3"));
}

void MainWindow::on_l5_clicked()
{
    img_on_list=4;
    w_cou=2;
    h_cou=2;
    btn_comp_press(4);
    recomp();
    ui->label_10->setText("2 x 2");
    ui->checkset_2->setGeometry(162,1,16,16);
    ui->label_17->setText(tr("Select: 2x2"));
}

void MainWindow::on_l6_clicked()
{
    img_on_list=6;
    w_cou=2;
    h_cou=3;
    btn_comp_press(5);
    recomp();
    ui->label_10->setText("2 x 3");
    ui->checkset_2->setGeometry(202,1,16,16);
    ui->label_17->setText(tr("Select: 2x3"));
}

void MainWindow::on_l7_clicked()
{
    img_on_list=8;
    w_cou=2;
    h_cou=4;
    btn_comp_press(6);
    recomp();
    ui->label_10->setText("2 x 4");
    ui->checkset_2->setGeometry(242,1,16,16);
    ui->label_17->setText(tr("Select: 2x4"));
}

void MainWindow::on_l8_clicked()
{
    img_on_list=9;
    w_cou=3;
    h_cou=3;
    btn_comp_press(7);
    recomp();
    ui->label_10->setText("3 x 3");
    ui->checkset_2->setGeometry(282,1,16,16);
    ui->label_17->setText(tr("Select: 3x3"));
}

void MainWindow::on_l9_clicked()
{
    img_on_list=15;
    w_cou=3;
    h_cou=5;
    btn_comp_press(8);
    recomp();
    ui->label_10->setText("3 x 5");
    ui->checkset_2->setGeometry(322,1,16,16);
    ui->label_17->setText(tr("Select: 3x5"));
}

void MainWindow::on_l10_clicked()
{
    img_on_list=20;
    w_cou=4;
    h_cou=5;
    btn_comp_press(9);
    recomp();
    ui->label_10->setText("4 x 5");
    ui->checkset_2->setGeometry(362,1,16,16);
    ui->label_17->setText(tr("Select: 4x5"));
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
    pn.setY(toprint[bufpress2].top+toprint[bufpress2].heigth/2);
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
    toprint[bufpress2].heigth=y;
    toprint[bufpress2].left=pn.x()-x/2;
    toprint[bufpress2].top=pn.y()-y/2;
    reShow(imgpress2);
    int rx, ry, y2;
    rx=toprint[bufpress2].left-8+toprint[bufpress2].width;
    if (rx>fon->width()) rx=fon->width()-16;
    y2=ry=toprint[bufpress2].top-8+toprint[bufpress2].heigth;
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
          toshow[index].pct->setGeometry(toprint[i].left,toprint[i].top,toprint[i].width,toprint[i].heigth);
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
        if(rc2.top()<toprint[bufpress2].rect.top()) y4=rc2.height()-rc.height();
        if(rc2.left()<toprint[bufpress2].rect.left()) x4=rc2.width()-rc.width();
        rc=toprint[bufpress2].rect;
        x3=rc.left()-x4;
        y3=rc.top()-y4;
        int h=rc.height();
        int w=rc.width();
        rc.setLeft(x3);
        rc.setTop(y3);
        rc.setWidth(w);
        rc.setHeight(h);
        toprint[bufpress2].rect=rc;
        if(toprint[bufpress2].show_caption || ui->checkBox_6->isChecked())
        {
            tocaption[toprint[bufpress2].cp_num].pct->move(x3,y3);
            tocaption[toprint[bufpress2].cp_num].pct->raise();
        }
        // end caption
    }
}

void MainWindow::resiz_up()
{
    QSize sz;
    timer->start(3000);
    resiz->setCursor(Qt::PointingHandCursor);
    sz=toshow[imgpress2].pct->size();
    toprint[bufpress2].width=sz.width();
    toprint[bufpress2].heigth=sz.height();
    int rx, ry;
    rx=toprint[bufpress2].left-8+toprint[bufpress2].width;
    if (rx>fon->width()) rx=fon->width()-16;
    ry=toprint[bufpress2].top-8+toprint[bufpress2].heigth;
    if (ry>fon->height()) ry=fon->height()-16;
    resiz->move(rx,ry);
    rx=clip->geometry().left();
    clip->move(rx,ry);
}

void MainWindow::on_checkBox_3_clicked(bool checked) // сохранять пропорции
{
    if(exitFlag)return;
    prop=checked;
    exitFlag=true;
    ui->checkBox_3->setChecked(checked);
    ui->checkBox_10->setChecked(checked);
    exitFlag=false;
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


void MainWindow::on_dial_2_valueChanged(int value) // вращение картинки
{
    if(exitFlag)return;
    ui->label_7->setText(QString::number(ui->dial_2->value()));
    if (imgpress2 == -1 || out_rot) return;
    rotated(value);
    exitFlag=true;
    ui->dial_4->setValue(value);
    exitFlag=false;
}



void MainWindow::on_checkBox_4_clicked(bool checked) // заполнить
{
    if(exitFlag)return;
    fillsize=checked;
    for(int i=0; i<=buf; i++) toprint[i].show=0;
    if (checked)
    {
    if (comp==0)
    {
        fill_all();
        toprint[toshow[0].buf].top=(h_fon-toprint[toshow[0].buf].heigth)/2;
        toprint[toshow[0].buf].left=(w_fon-toprint[toshow[0].buf].width)/2;
        reShow(0);
    }
    else
    {
        mem_pol=pol;
        ui->dial->setValue(0);
    }
    }
    else
    {
        ui->dial->setValue(mem_pol);
    }
    exitFlag=true;
    ui->checkBox_4->setChecked(checked);
    ui->checkBox_8->setChecked(checked);
    exitFlag=false;
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
               toprint[toshow[0].buf].heigth=h_fon;
               return;
           }
           else
           {
               toprint[toshow[0].buf].width=w_fon;
               toprint[toshow[0].buf].heigth=w_fon/e;
               return;
           }
        }
        else
        {
           i=w_fon/e;
           if (i>h_fon)
           {
               toprint[toshow[0].buf].width=w_fon;
               toprint[toshow[0].buf].heigth=i;
               return;
            }
           else
           {
               toprint[toshow[0].buf].width=h_fon*e;
               toprint[toshow[0].buf].heigth=h_fon;
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
                toprint[toshow[0].buf].heigth=i;
                return;
             }
             else
             {
                toprint[toshow[0].buf].width=h_fon*e;
                toprint[toshow[0].buf].heigth=h_fon;
                return;
             }
          }
         else
         {
             i=h_fon*e;
             if(i>w_fon)
             {
                 toprint[toshow[0].buf].width=i;
                 toprint[toshow[0].buf].heigth=h_fon;
                 return;
             }
             else
             {
                 toprint[toshow[0].buf].width=w_fon;
                 toprint[toshow[0].buf].heigth=w_fon/e;
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
        setty.setValue("A3", printer_a3);
        setty.setValue("width", paper_w);
        setty.setValue("heigth", paper_h);
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
        setty.setValue("A3", printer_a3);
        setty.setValue("font_scl", font_scl);
    setty.endGroup();
}

void MainWindow::rest_printer_sett()
{
    setty.beginGroup("Printer");
        p_name.clear();
        p_name.append(setty.value("printer_name", "").toString());
        left_m=setty.value("left_m", 10).toInt() ;
        right_m=setty.value("right_m", 10).toInt();
        top_m=setty.value("top_m", 10).toInt();
        bottom_m=setty.value("bottom_m", 10).toInt();
        print_color=setty.value("color", true).toBool();
        pap_name=setty.value("paper_name", 0).toInt();
        pap_sor=setty.value("sourse", 0).toInt();
        printer_a3=setty.value("A3", false).toBool();
        paper_h=setty.value("heigth", 297).toInt();
        paper_w=setty.value("width", 210).toInt();
        list_n=setty.value("caption", "A4 210x297 mm").toString();
        font_scl=setty.value("font_scl", 1).toDouble();
    setty.endGroup();
}

void MainWindow::save_view_sett()
{
    setty.beginGroup("Settings");
        setty.setValue("view_margins", ui->checkBox_5->isChecked());
        setty.setValue("layout", comp);
    setty.endGroup();
}

void MainWindow::rest_view_sett()
{
    setty.beginGroup("Settings");
        on_checkBox_5_clicked(setty.value("view_margins", false).toBool());
        set_layout(setty.value("layout", 0).toInt());
    setty.endGroup();
}

void MainWindow::set_btn_pos(int d)
{
    ui->l1->move(10, 10-d);
    ui->l2->move(10, 90-d);
    ui->l3->move(10, 170-d);
    ui->l4->move(10, 250-d);
    ui->l5->move(10, 330-d);
    ui->l6->move(10, 410-d);
    ui->l7->move(10, 490-d);
    ui->l8->move(10, 570-d);
    ui->l9->move(10, 650-d);
    ui->l10->move(10, 730-d);
    ui->pushButton->move(10, 810-d);
    ui->pushButton_8->move(10, 890-d);
    ui->checkset->move(10, comp*80+5-d);
    offset=d;
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    set_btn_pos(value);
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
    int i=7;
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

void MainWindow::show_paper_size()
{
    ui->label_11->setText(tr("Your paper:"));
    QString l;
    l.append("Width: ");
    l.append(QString::number(paper_w));
    ui->label_12->setText(l);
    l.clear();
    l.append("Heigth: ");
    l.append(QString::number(paper_h));
    ui->label_13->setText(l);
    if(imgpress2>-1)
    {
        imgpress2=-1;
        bufpress2=-1;
        quick_buttons_off();
        if (rez!=0)rez->hide();
    }
    ui->pushButton_6->hide();
    ui->pushButton_32->hide();
    ui->checkBox_10->setVisible(true);
    ui->checkBox_12->setVisible(true);
}

void MainWindow::show_pict_size()
{
    QSize sz=toshow[imgpress2].pct->size();
    ui->label_11->setText(tr("Curent picture:"));
    QString l;
    double d1;
    l.append("Width: ");
    d1=sz.width();
    l.append(QString::number(d1/ppx));
    ui->label_12->setText(l);
    l.clear();
    l.append("Heigth: ");
    d1=sz.height();
    l.append(QString::number(d1/ppy));
    ui->label_13->setText(l);
}

void MainWindow::on_checkBox_5_clicked(bool checked) // показывать поля
{
    if(exitFlag)return;
    if (checked) ui->sheet->setFrameStyle(6);
    else  ui->sheet->setFrameStyle(0);
    save_view_sett();
    ui->checkBox_5->setChecked(checked);
    exitFlag=true;
    ui->checkBox_5->setChecked(checked);
    ui->checkBox_11->setChecked(checked);
    exitFlag=false;
}

void MainWindow::set_layout(int c)
{
    switch(c)
    {
    case 0:
        on_l1_clicked(); break;
    case 1:
        on_l2_clicked(); break;
    case 2:
        on_l3_clicked(); break;
    case 3:
        on_l4_clicked(); break;
    case 4:
        on_l5_clicked(); break;
    case 5:
        on_l6_clicked(); break;
    case 6:
        on_l7_clicked(); break;
    case 7:
        on_l8_clicked(); break;
    case 8:
        on_l9_clicked(); break;
    case 9:
        on_l10_clicked(); break;
    default:
        on_l1_clicked();
    }
    while (5>ui->checkset->geometry().top())
    {
        ui->verticalScrollBar->setValue(ui->verticalScrollBar->value()-1);
    }
    while (260<ui->checkset->geometry().top())
    {
        ui->verticalScrollBar->setValue(ui->verticalScrollBar->value()+1);
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
    ui->pushButton_6->show();
    ui->pushButton_32->show();
    ui->tabWidget_2->setCurrentIndex(2);
    ui->checkBox_10->setVisible(false);
    ui->checkBox_12->setVisible(false);
}

void MainWindow::paint_frame()
{
    QPixmap pm(QSize(rez->width(), rez->height()));
    pm.fill(Qt::transparent);
    QPainter painter(&pm);
    //цвет линии
    painter.setPen(QColor(255,128,0));
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

void MainWindow::on_pushButton_6_clicked() // обрезка
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
    toprint[bufpress2].heigth=rc.height();
    toshow[imgpress2].pct->setPixmap(p);
    toshow[imgpress2].pct->setGeometry(rc);
    cout << "clipped successfully!" << endl;
    ui->pushButton_6->hide();
    ui->pushButton_32->hide();
    rez->hide();
    ui->checkBox_10->setVisible(true);
    ui->checkBox_12->setVisible(true);
}

void MainWindow::on_pushButton_9_clicked()
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
    show_pict();
}

void MainWindow::move_next()
// переместить картинку на следующий лист
{
    toprint[bufpress2].list++;
    if(toprint[bufpress2].list>lists)lists++;
    show_pict();
}

void MainWindow::turn_caption() //переключить состояние видимости подписи
{
    toprint[bufpress2].show_caption=!toprint[bufpress2].show_caption;
    show_caption(bufpress2, toprint[bufpress2].show_caption);
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
    cp_setPixmap(bufpress2); // картинка подписи
    show_caption(bufpress2, shw);          // show caption
}

void MainWindow::cp_setGeometry(int index)
{
    QFontMetrics m(toprint[index].font);
    int w=m.width(toprint[index].caption);
    int h=m.height();
    int t;
    int l;
    if(toprint[index].rect.width()==0)
    {
        t=toprint[index].top+toprint[index].heigth;
        l=toprint[index].left+(toprint[index].width-w)/2;
    }
    else
    {
        t=toprint[index].rect.top();
        l=toprint[index].rect.left();
    }
    QRect rc;
    rc.setRect(l,t,w,h);
    toprint[index].rect=rc;       // геометрия подписи
}


void MainWindow::cp_setPixmap(int index)
// создать картинку надписи - нужно для предпросмотра, чтобы масштабировать
{
    QRect rc=toprint[index].rect;
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

void MainWindow::show_caption(int index, bool sh) // показать подпись к картинке index
{
    int count;
    QRect rc;
    if (toprint[index].list==curlist){
        if(sh)
        {   // создать подпись
          cp_setGeometry(index);    // геометрия
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
          tocaption[count].pct->setPixmap(toprint[index].cp_pixmap);
          tocaption[count].pct->setScaledContents(true);
          rc=toprint[index].rect;
          rc.setWidth(double(rc.width())*list_scl*font_scl);
          rc.setHeight(double(rc.height())*list_scl*font_scl);
          tocaption[count].pct->setGeometry(rc);
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
        QRect rc=toprint[bufpress].rect;
        x1=rc.left()+ttx-x1;
        y1=rc.top()+tty-y1;
        int h=rc.height();
        int w=rc.width();
        tocaption[i].pct->move(x1,y1);
        rc.setLeft(x1);
        rc.setTop(y1);
        rc.setHeight(h);
        rc.setWidth(w);
        toprint[bufpress].rect=rc;
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

void MainWindow::on_checkBox_6_clicked(bool checked) // включить подписи
{
    if(buf<0 || exitFlag)return;
    exitFlag=true;
    ui->checkBox_6->setChecked(checked);
    ui->checkBox_12->setChecked(checked);
    exitFlag=false;
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

void MainWindow::on_pushButton_14_clicked()
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
        toprint[i].show=0;
    }
    else               // edit text block
    {
        toprint[i].show=1;
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
        toprint[i].rect=rc;                 // начальная геометрия подписи
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

void MainWindow::on_pushButton_15_clicked()
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
        out << comp;
        out << buf;
        out << all_rot;
        out << gor;
        for(int i=0; i<=buf; i++)
            {
            out<<toprint[i].pict;          // путь к файлу
            out<<toprint[i].rot;           // 0 - нормально, угол поворота
            out<<toprint[i].pix0;          // исходный pixmap
            QApplication::processEvents();
            out<<toprint[i].pix;           // текущий pixmap
            out<<toprint[i].left;          // координата Х
            out<<toprint[i].top;           // координата Y
            out<<toprint[i].heigth;        // высота
            out<<toprint[i].width;         // ширина
            out<<toprint[i].show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
            out<<toprint[i].list;          // номер страницы этой картинки
            out<<toprint[i].load;          // флаг того, картинка уже загружена в память
            out<<toprint[i].z;             // z-порядок
            out<<toprint[i].xl;            // Х подписи
            out<<toprint[i].yl;            // Y подписи
            out<<toprint[i].dxl;           // относительное смещение положения подписи от картинки (по Х)
            out<<toprint[i].dyl;           // относительное смещение положения подписи от картинки (по Y)
            out<<toprint[i].prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
            out<<toprint[i].compress;      // кэф. компрессии для уже показанной картинки
            out<<toprint[i].caption;       // подпись к картинке
            out<<toprint[i].show_caption;  // показывать подпись
            out<<toprint[i].back_color;    // цвет фона
            out<<toprint[i].trans;         // прозрачный фон
            out<<toprint[i].font_color;    // цвет шрифта
            out<<toprint[i].font;          // шрифт
            out<<toprint[i].rect;          // геометрия подписи
            out<<toprint[i].cpt;           // номер подписи на экране к этой картинке
            out<<toprint[i].cp_num;        // номер avLabel - подписи на экране
            out<<toprint[i].cp_z;          // z-орядок для подписи
            out<<toprint[i].cp_pixmap;     // pixmap подписи
            out<<toprint[i].isTextBlock;   // true - это текстовый блок
            out<<toprint[i].alig;
            QApplication::processEvents();
            }
        out << lists;
        if (int(sheet.size())<lists)
            for(int i=sheet.size(); i<lists; i++)
                sheet.push_back(curSheet);
        for (int i=0; i<=lists-1; i++)
        {
            out << sheet[i].list_orn;
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

void MainWindow::on_pushButton_16_clicked()
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
        buf=0;
        toprint.clear();
        ind_start();
        toprint.clear();
        buf=-1;
        QFile file(fileName);
        r=file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> comp;
        set_layout(comp);
        in >> buf;
        in >> all_rot;
        ui->checkBox->setChecked(all_rot);
        in >> gor_old;
        double d=1;
        if (gor!=gor_old) d=double(gor)/double(gor_old);
        for(int i=0; i<=buf; i++)
            {
            toprint.push_back(pict());
            in>>toprint[i].pict;          // путь к файлу
            in>>toprint[i].rot;           // 0 - нормально, угол поворота
            in>>toprint[i].pix0;          // исходный pixmap
            QApplication::processEvents();
            in>>toprint[i].pix;           // текущий pixmap
            in>>toprint[i].left;          // координата Х
            in>>toprint[i].top;           // координата Y
            in>>toprint[i].heigth;        // высота
            in>>toprint[i].width;         // ширина
            in>>toprint[i].show;          // флаг того, что картинка уже была расчитана для данной компоновки, ее надо просто показать
            in>>toprint[i].list;          // номер страницы этой картинки
            in>>toprint[i].load;          // флаг того, картинка уже загружена в память
            in>>toprint[i].z;             // z-порядок
            in>>toprint[i].xl;            // Х подписи
            in>>toprint[i].yl;            // Y подписи
            in>>toprint[i].dxl;           // относительное смещение положения подписи от картинки (по Х)
            in>>toprint[i].dyl;           // относительное смещение положения подписи от картинки (по Y)
            in>>toprint[i].prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
            in>>toprint[i].compress;      // кэф. компрессии для уже показанной картинки
            in>>toprint[i].caption;       // подпись к картинке
            in>>toprint[i].show_caption;  // показывать подпись
            in>>toprint[i].back_color;    // цвет фона
            in>>toprint[i].trans;         // прозрачный фон
            in>>toprint[i].font_color;    // цвет шрифта
            in>>toprint[i].font;          // шрифт
            in>>toprint[i].rect;          // геометрия подписи
            in>>toprint[i].cpt;           // номер подписи на экране к этой картинке
            in>>toprint[i].cp_num;        // номер avLabel - подписи на экране
            in>>toprint[i].cp_z;          // z-орядок для подписи
            in>>toprint[i].cp_pixmap;     // pixmap подписи
            in>>toprint[i].isTextBlock;   // true - это текстовый блок
            in>>toprint[i].alig;
            QApplication::processEvents();
            toprint[i].cp_num=-1;
            if (d!=1)
            {   // уточнение координат, если новое окно отличается от старого
                toprint[i].left=double(toprint[i].left)*d;
                toprint[i].top=double(toprint[i].top)*d;
                toprint[i].heigth=double(toprint[i].heigth)*d;
                toprint[i].width=double(toprint[i].width)*d;
            }
        }
        in>>lists;
        sheet.clear();
        bool b;
        for(int i=0;i<lists;i++)
        {
            in>>b;
            sheet.push_back(curSheet);
            sheet[i].list_orn=b;
        }
        file.close();
        ind_stop();
        curlist=1;
        show_pict();
    }
    return r;
}


// лента

void MainWindow::on_l1_2_clicked()
{
    on_l1_clicked();
}

void MainWindow::on_l2_2_clicked()
{
    on_l2_clicked();
}

void MainWindow::on_l3_2_clicked()
{
    on_l3_clicked();
}

void MainWindow::on_l4_2_clicked()
{
    on_l4_clicked();
}

void MainWindow::on_l5_2_clicked()
{
    on_l5_clicked();
}

void MainWindow::on_l6_2_clicked()
{
    on_l6_clicked();
}

void MainWindow::on_l7_2_clicked()
{
    on_l7_clicked();
}

void MainWindow::on_l8_2_clicked()
{
    on_l8_clicked();
}

void MainWindow::on_l9_2_clicked()
{
    on_l9_clicked();
}

void MainWindow::on_l10_2_clicked()
{
    on_l10_clicked();
}

void MainWindow::on_pushButton_38_clicked()
{
    on_pushButton_clicked();
}

void MainWindow::on_pushButton_39_clicked()
{
    on_pushButton_8_clicked();
}

void MainWindow::on_pushButton_25_clicked()
{
    on_pushButton_2_clicked(); //открыть 1 файл
}

void MainWindow::on_pushButton_26_clicked()
{
    on_pushButton_12_clicked(); // открыть папку
}

void MainWindow::on_pushButton_27_clicked()
{
    on_pushButton_15_clicked();
    // сохранить сеанс
}

void MainWindow::on_pushButton_28_clicked()
{
    on_pushButton_16_clicked();
    // открыть сохраненный сеанс
}

void MainWindow::on_pushButton_37_clicked()
{
    on_pushButton_14_clicked();
    // вставить текст
}

void MainWindow::on_pushButton_32_clicked()
{
    on_pushButton_6_clicked(); // обрезка
}

void MainWindow::on_pushButton_30_clicked()
{
    on_pushButton_3_clicked(); // назад
}

void MainWindow::on_pushButton_31_clicked()
{
    on_pushButton_4_clicked(); // назад
}

void MainWindow::on_pushButton_29_clicked()
{
    on_pushButton_5_clicked(); //печать
}

void MainWindow::on_pushButton_35_clicked()
{
    on_pushButton_7_clicked(); // настройка программы
}

void MainWindow::on_pushButton_33_clicked() // портретно
{
   on_pushButton_10_clicked();
}

void MainWindow::on_pushButton_34_clicked() // ландшафтно
{
    on_pushButton_11_clicked();
}


void MainWindow::on_checkBox_7_clicked(bool checked) // общая ориентация для всех листов
{
    if(exitFlag)return;
    on_checkBox_clicked(checked);
}

void MainWindow::on_checkBox_8_clicked(bool checked) // заполнить
{
    if(exitFlag)return;
    on_checkBox_4_clicked(checked);
}

void MainWindow::on_checkBox_9_clicked(bool checked) // быстрое редактирование
{
    ui->checkBox_2->setChecked(checked);
}

void MainWindow::on_checkBox_10_clicked(bool checked)
{
    if(exitFlag)return;
    on_checkBox_3_clicked(checked); // сохранять пропорции
}

void MainWindow::on_checkBox_11_clicked(bool checked)
{
    if(exitFlag)return;
    on_checkBox_5_clicked(checked); // показывать поля
}

void MainWindow::on_checkBox_12_clicked(bool checked)
{
    if(exitFlag)return;
    on_checkBox_6_clicked(checked); // включить подписи
}


void MainWindow::on_dial_3_valueChanged(int value) // зазоры
{
    if(exitFlag)return;
    ui->dial->setValue(value);
}

void MainWindow::on_dial_4_valueChanged(int value)  // поворот
{
    if(exitFlag)return;
    ui->dial_2->setValue(value);
}

void MainWindow::on_checkBox_13_clicked(bool checked) // без полей из ленты
{
    if(checked)
    {
        setty.setValue("left_m", left_m);
        setty.setValue("right_m", right_m);
        setty.setValue("top_m", top_m);
        setty.setValue("bottom_m", bottom_m);
        bottom_m=0;
        left_m=0;
        top_m=0;
        right_m=0;
    }
    else
    {
        top_m=setty.value("top_m", 5).toInt();
        left_m=setty.value("left_m", 5).toInt();
        right_m=setty.value("right_m", 5).toInt();
        bottom_m=setty.value("bottom_m", 5).toInt();
    }
    make_list();
}

void MainWindow::on_pushButton_36_clicked() // о программе
{
    on_pushButton_9_clicked();
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
        p.heigth=297;
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
                p.heigth=setty.value("H"+QString::number(i), 297).toDouble();
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
//    paper_w=p.width;
//    paper_h=p.heigth;
    make_list();
    recomp();
    ui->label_3->setText(s);
}
