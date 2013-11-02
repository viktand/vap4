#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagesetup.h"
#include "avLabel.h"
#include "userlayout.h"
#include <math.h>
#include <qmath.h>

#include <QDesktopWidget>
#include <QPrinter>
#include <QPainter>
#include <QMovie>
#include <QThread>
#include <QPicture>
#include <QFileDialog>
#include <QPrintDialog>
#include <QTimer>
#include <QMatrix>
#include <QPointF>
#include <QSettings>

#define PI 3.14159265

#include <iostream>
using namespace std;



// глобальные переменные
QPrinter *printer;       // принтер, в него рисовать и настраивать
QPainter *pntr;          // устройство рисования (с него печатать)
PageSetup *ps;
userlayout *uslay;
QMovie *animGif;
QTimer *timer;
QSettings setty("vap", "vap");
//QThread *thread;
loadpicture *ldp;
QString str_time;
QavLabel *fon;
QavLabel *rez; // рамка обрезки
QRect virt_paper;       // виртуальный лист бумаги, на котором рисуется картинка
                        // т.е. лист накладывается на А4 и печатается.
bool flag2;
int  count = 0;
int	 img_count = -1; 	// количество превьюшек
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
//bool orn=true;          // ориентация листа true - портретная
int  buf=-1; 		    // количество загруженных картинок, отсчет с нуля
int  ttx, tty;			// временные значения для отработки перемещения объектов
int  imgpress=-1;		// № нажатой превьюшки
int  bufpress=-1;		// № картинки в общем списке, превью которой нажато
int  imgpress2=-1;		// № нажатой превьюшки
int  bufpress2=-1;		// № картинки в общем списке, превью которой нажато
int  curz=0;            // z порядок для текущей страницы, используется для расчета наложений
int  lists=0, curlist=0;// всего листов и текущий лист
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
bool set_orn;
bool print_color;
QString list_n;
int prn_size_x;
int prn_size_y;
double h_ofsett; // горизонтальное смещение позиции печати
int pap_sor;     // источник бумаги
int pap_name;
bool printer_a3; // true - A3 (big size paper)


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
    char      comm[100];     // комментарий (подпись) к картинке
    int       showcomm;      // показывать подпись или нет
    int       xl;            // Х подписи
    int       yl;            // Y подписи
    char      fnt_Name[31];  // имя фонта подписи
    int       fnt_Size;      // размер фонта подписи
    //fnt_Style: tfontstyles;   // стиль фонта подписи
    //fnt_Color: tColor;        // цвет
    int       dxl;           // относительное смещение положения подписи от картинки (по Х)
    int       dyl;           // относительное смещение положения подписи от картинки (по Y)
    int       prew;          // номер превьюшки для этой картинки (только в случае pict.list==curlist)
    double    compress;      // кэф. компрессии для уже показанной картинки
};

struct prew {
    QavLabel     *pct;        //картинка
    int          buf;         //абсолютный номер
};

QavLabel *rott;              // "ручка" вращения
QavLabel *resiz;             // "ручка" изменения размера
QavLabel *delt;              // кнопка удаления картинки
QavLabel *clip;              // кнопка ножницы


std::vector<pict> toprint;      //массив загруженных картинок
std::vector<prew> toshow;       //массив превьющек
std::vector<bool> list_orn;     // массив ориентаций листов true - портретная ориентация


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  // конструктор
{
    ui->setupUi(this);
    fon=new QavLabel(ui->sheet);
    connect(fon, SIGNAL(mouse_press(int,int,int)), this, SLOT(show_paper_size()));
    list_n.append("A4 210x297 mm");
    fon->show();
    print_color=true;
    set_orn = true;
    top_m=5;
    left_m=5;
    right_m=5;
    bottom_m=5;
    all_rot=true;
    ul_hor=3;
    ul_ver=3;
    h_ofsett=0;
    pap_sor=0;
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    printer = new QPrinter(QPrinter::HighResolution);
    list_orn.push_back(true);
    pntr = new QPainter();
    if (printer->orientation() == QPrinter::Portrait) ornl=1; else ornl=0;
    printer->setPageSize(QPrinter::A4);
    rap=true;
    animGif = new QMovie(":/new/prefix1/indicator");
    ui->label->setMovie(animGif);
    set_indic_pos();
    rott = new QavLabel(fon);
    QImage im(":/new/prefix1/rotation");
    rott->setPixmap(QPixmap::fromImage(im));
    rott->setScaledContents(true);
    rott->setGeometry(0,0,16,16);
    rott->setCursor(Qt::PointingHandCursor);
    rott->hide();
    QObject::connect(rott, SIGNAL(mouse_press(int, int, int)), this, SLOT(pct_press_rott()));
    delt = new QavLabel(fon);
    QImage im1(":/new/prefix1/delete");
    delt->setPixmap(QPixmap::fromImage(im1));
    delt->setScaledContents(true);
    delt->setGeometry(0,0,16,16);
    delt->setCursor(Qt::PointingHandCursor);
    delt->hide();
    QObject::connect(delt, SIGNAL(mouse_press(int, int, int)), this, SLOT(pct_press_delete()));
    resiz = new QavLabel(fon);
    QImage im2(":/new/prefix1/resize");
    resiz->setPixmap(QPixmap::fromImage(im2));
    resiz->setScaledContents(true);
    resiz->setGeometry(0,0,16,16);
    resiz->setCursor(Qt::PointingHandCursor);
    resiz->hide();
    QObject::connect(resiz, SIGNAL(mouse_press(int, int, int)), this, SLOT(resiz_press(int, int)));
    QObject::connect(resiz, SIGNAL(mouse_move(int, int, int)),  this, SLOT(resiz_move(int, int)));
    QObject::connect(resiz, SIGNAL(mouse_up(int, int, int)), this, SLOT(resiz_up()));
    clip = new QavLabel(fon);
    QImage im3(":/new/prefix1/rez");
    clip->setPixmap(QPixmap::fromImage(im3));
    clip->setScaledContents(true);
    clip->setGeometry(0,0,16,16);
    clip->setCursor(Qt::PointingHandCursor);
    clip->hide();
    connect(clip, SIGNAL(mouse_press(int,int,int)), this, SLOT(show_clip()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(istimer()));
    set_btn_pos(0);
    rest_sind_size();
    rest_printer_sett();
    rest_view_sett();
    ui->pushButton_6->hide();
    double d1, d2;
    d1=prn_size_y=paper_h;
    d2=prn_size_x=paper_w;
    paper_ratio=d1/d2;
    if(printer_a3)
    {
        prn_size_y=420;
        prn_size_x=297;
    }
    make_list();
    show_paper_size();
 }

void MainWindow::resizeEvent(QResizeEvent *e)
{
    gor_old=gor;
    wind_sz=e->size();
    make_list();
    save_wind_size();
}

void MainWindow::redraw()
{
    // изменение размера окна программы
    // расчет окна предпросмотра
    double x, y, w, h;           //будущий рект окна
    y=paper_h;
    x=paper_w;
    paper_ratio=x/y;
    w=x=wind_sz.width()-155;       //допустимая область для размещения
    h=y=wind_sz.height()-75;       // --
    if (w*paper_ratio>h) w=h*paper_ratio;
    else h=w/paper_ratio;
    x=(x-w)/2.0;
    y=(y-h)/2.0;
    x_prw=x+150;
    y_prw=70;
    gor=w;
    ver=h;
    scl_pg=w/210.0;
    layout_scale();
    ui->label_3->setText(list_n);
}

void MainWindow::make_list() // создать лист предпросмотра
{
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
    if (buf>-1) show_pict();
}

void MainWindow::get_pp()
{
    double d1, d2;
    d1=ui->list->width();
    d2=paper_w;
    ppx=d1/d2;
    d1=ui->list->height();
    d2=paper_h;
    ppy=d1/d2;
}

void MainWindow::set_wind_size()
{
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
    ind_start();
    load_param();
    ind_stop();
}

void MainWindow::load_param() // обработка параметров командной строки
{
    if (cou_prm<2) return;
    ind_start();
    for(int i=1;i<cou_prm; i++)
     {

        flag2=true;
         //if (rap) cout << "File " << i << " : " << prm[i] << endl;
        open_pct(prm[i]);
        while (flag2) { QApplication::processEvents(); }
     }
    show_pict();
}

void MainWindow::open_pct(QString filename) // добавить картинку в список
{
    if (filename.contains("file:///")) filename=esc_to_utf(filename);
        buf++;
        if(lists==0)
        {
            lists=1;
            curlist=1;
        }
        str_time.clear();
        str_time.append(filename);
        toprint.push_back(pict());
        toprint[buf].pict= filename;
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
        if (image.isNull()) { } // неудачная загрузка
        double d;
        //toprint[buf].pct = image;
        toprint[buf].pix0 = toprint[buf].pix = QPixmap::fromImage(image);
        toprint[buf].show= 0;
        d=buf+1;
        d=ceil(d/img_on_list);
        toprint[buf].list=d;
        lists=0;
        for(int i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
        if (rap) cout << "picture was loaded" << endl;
        img_size_cur_comp();
        flag2=false;
}


//  class loadpicture

loadpicture::loadpicture(QObject *parent) :  QObject(parent)
{
    emit start();
}

void loadpicture::start_load(QString filename) // start load
{
    QImage image(filename);
    emit sendImage(image);
}


// end



void MainWindow::make_var_to_list()
{
    if (list_orn[curlist-1])
    {
        if(paper_h<paper_w) swap(paper_h, paper_w);
        ui->pushButton_10->setIcon(ui->pushButton_11->icon());
        ui->pushButton_11->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    }
    else
    {
        if(paper_h>paper_w) swap(paper_h, paper_w);
        ui->pushButton_11->setIcon(ui->pushButton_10->icon());
        ui->pushButton_10->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    }
}

void MainWindow::on_pushButton_2_clicked() //открыть 1 файл
{
    QString fileName = get_file();
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
    ind_start();
    QString dirf = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
   QDir dir(dirf);
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
   show_pict();
   ind_stop();
}

void MainWindow::on_pushButton_clicked() // all in oun
{
   for (int i=0; i<=buf; i++) toprint[i].list=1;
   btn_comp_press(10);
   img_on_list=buf+1;
   recomp();
   ui->label_10->setText("All in oun");
}

void MainWindow::on_pushButton_8_clicked() // my layout
{
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
    if (curlist<lists)
    {
        curlist++;
        if (int(list_orn.size())<lists) list_orn.push_back(list_orn.back());
        make_var_to_list();
        show_pict();
    }
}

void MainWindow::on_pushButton_3_clicked() // назад
{
    if(curlist>1)
    {
        curlist--;
        make_var_to_list();
        show_pict();
    }
}

void MainWindow::on_pushButton_7_clicked() // настройка бумаги
{
    if(ps == 0)
    {
        ps=new PageSetup();
        QObject::connect(ps, SIGNAL(end_set()), this, SLOT(set_setting()));
    }
    ps->load_data();
    ps->show();
    connect(ps, SIGNAL(set_all_em(bool)), this, SLOT(set_all_rot(bool)));

}

void MainWindow::on_pushButton_11_clicked() // ландшафтная ориентация
{
    if(paper_w<paper_h) swap(paper_h, paper_w);
    ui->pushButton_11->setIcon(ui->pushButton_10->icon());
    ui->pushButton_10->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    end_rotation();
}

void MainWindow::on_pushButton_10_clicked() // портретная ориентация
{
    if(paper_w>paper_h) swap(paper_h, paper_w);
    ui->pushButton_10->setIcon(ui->pushButton_11->icon());
    ui->pushButton_11->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    end_rotation();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    all_rot=checked;
    if(ps!=0) ps->set_all();
    if (checked)
    {
        for(int i=0; i<=buf; i++)  if (list_orn[toprint[i].list] != (paper_w<paper_h)) toprint[i].show=0;
        for(int i=0; i<lists; i++) list_orn[i]=(paper_w<paper_h);
    }
}

void MainWindow::set_all_rot(bool b)
{
   all_rot=b;
   ui->checkBox->setChecked(b);
}

double MainWindow::get_scaleX()
{
   double x_size;
   double x_prew;
   x_size=x_pg*prx;
   x_prew=fon->width();
   return x_size/x_prew;
}

double MainWindow::get_scaleY()
{
   double y_size;
   double y_prew;
   y_size=y_pg*pry;
   y_prew=fon->height();
   return y_size/y_prew;
}


void MainWindow::set_printer() // настройка принтера
{
     printer->setPrinterName(p_name);
     printer->setDocName("vap_pictures");
     if (print_color) printer->setColorMode(QPrinter::Color);
         else printer->setColorMode(QPrinter::GrayScale);
     printer->setOutputFormat(QPrinter::NativeFormat);
     if (prn_size_x+prn_size_y!=717) printer->setPaperSize(QPrinter::A4);
        else printer->setPaperSize(QPrinter::A3);
     int dx;
     QRect rc=printer->paperRect();
     if (paper_w<paper_h)
     {
        prx=rc.width()/prn_size_x;
        pry=rc.height()/prn_size_y;
        dx=(prn_size_x-x_pg)/2;
        printer->setOrientation(QPrinter::Portrait);
     }
     else
     {
         prx=rc.width()/prn_size_y;
         pry=rc.height()/prn_size_x;
         dx=(prn_size_y-x_pg)/2;
         printer->setOrientation(QPrinter::Landscape);
     }
     printer->setPageMargins(dx+left_m, top_m, right_m, bottom_m, QPrinter::Millimeter);
}

void MainWindow::on_pushButton_5_clicked() //печать
{
    set_printer(); // настроить принтер
    QRect rc;   // шаблон для печати
    double sclX, sclY; // коэф. масштабирования при пересчете размеров и координат
    double t;   // time var
    bool f_prn=true;
    sclX=get_scaleX();
    sclY=get_scaleY();
    for (int i=0; i<=lists; i++)
    {
        if (f_prn)pntr->begin(printer); // начать рисование
        f_prn=false;
        for(int j=0; j<=buf; j++)
        {
            if (toprint[j].list==i)
            {
                    t=toprint[j].top;    rc.setTop(t*sclY);
                    t=toprint[j].left;   rc.setLeft(t*sclX);
                    cout << "picture position: top = " << rc.top() << "; left = " << rc.left() << endl;
                    t=toprint[j].heigth; rc.setHeight(t*sclY);
                    t=toprint[j].width;  rc.setWidth(t*sclX);
                    cout << "picture size: heigth = " << toprint[j].heigth << "; width = " << toprint[j].width << endl;
                    pntr->drawImage(rc, toprint[j].pix.toImage());
                    cout << "rect size: heigth = "<< rc.height() << "; width = " << rc.width() << " " << endl;
                    cout << "picture " << j+1 << " : " << " painted" << endl;
                    f_prn=true;
             }
        }
        if (f_prn)
        {
            pntr->end(); // отправить на печать
            cout << "end printing" << endl;
        }
    }
   rc=printer->paperRect();
   cout << "Parer: rect size: heigth = "<< rc.height() << "; width = " << rc.width() << endl;
   cout << "sclX= "<< sclX << " sclY= "<< sclY << endl;
}


void MainWindow::end_rotation()
{
    make_list();
    if (ui->checkBox->checkState()) for (int i=0; i<lists; i++) list_orn[i]=(paper_w<paper_h);
    btn_comp_press(comp);
    if (all_rot)
    {
        for(int i=0; i<lists; i++) list_orn[i]=(paper_w<paper_h);
        recomp();
    }
        else
    {
        list_orn[curlist-1]=(paper_w<paper_h);
        recomp_curlist();
    }
    set_indic_pos();
    swap(x_pg, y_pg);
}


void MainWindow::start_load_picture()
{
    ldp->start_load(str_time);
}

void MainWindow::pct_press(int x, int y, int i) // нажатие на превьюшку
{
    if(rez!=0) rez->hide();
    toshow[i].pct->setCursor(Qt::ClosedHandCursor); // поменять курсор
    toshow[i].pct->raise(); // поднять выше всех
    toprint[toshow[i].buf].z=++curz;
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
    ui->dial_2->setValue(toprint[bufpress].rot);
    out_rot=false;
    ui->label_7->setText(QString::number(ui->dial_2->value()));
    show_pict_size();
}

void MainWindow::pct_move(int x, int y, int i)
{
    {
        int x1=0, y1=0, dy, cx;
        x1=ttx;
        y1=tty;
        ttx = x;
        tty = y;
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
    for(int i=0; i<=buf; i++)
        if (toprint[i].list==curlist)
            toprint[i].show=0;
    show_pict();
}


void MainWindow::set_setting()
{
    new_margins();
    x_pg=paper_w;
    y_pg=paper_h;
}

void MainWindow::new_margins()
{
    end_rotation();
    //show_pict();
    save_printer_sett();
}

QString MainWindow::get_file()
{
    return QFileDialog::getOpenFileName(this, tr("Open File"),
            "/home", tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.ico)"));
}

void MainWindow::ind_start()
{
    ui->label->show();
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
    img_on_list=ul_hor*ul_ver;
    w_cou=ul_hor;
    h_cou=ul_ver;
    if (set_orn) on_pushButton_10_clicked();
    else on_pushButton_11_clicked();
    btn_comp_press(11);
    recomp();
    ui->label_10->setText(QString::number(w_cou) + " x " + QString::number(h_cou));
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
            if (toprint[i].z>c) c=toprint[i].z;
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
    img_count=-1;
    curz=0;
}

void MainWindow::on_dial_valueChanged(int value)
{
    if (pol==value) return;
    pol=value;
    img_size_cur_comp ();
    ui->label_5->setText(QString::number(value));
    for(int i=0; i<=buf; i++) toprint[i].show=0;
    show_pict();
}

void MainWindow::show_pict() // показать картинки текущего листа
{
    kill_pict(); // очистить лист предпросмотра
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

          if (toprint[i].show==0)
            {   // картинка рисуется первый раз
                sz=setsize(toshow[img_count].pct->pixmap()->size());
                if (img_count<(w_cou*h_cou))
                {
                    x=(img_count%w_cou)*(w_fon/w_cou)+(getx-sz.width())/2;
                    y=ceil(img_count/w_cou)*(h_fon/(h_cou*n))+(gety-sz.height())/2;
                }
                else //подбор координат для "лишних" картинок
                {
                    x=w_fon;
                    while (x>w_fon-sz.width()) x=rand()%(w_fon-sz.width());
                    y=h_fon;
                    while (y>h_fon-sz.height()) y=rand()%(h_fon-sz.height());
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
          }
          if (rap) cout << "picture was shown" << endl;
      }
    }
    set_z();
    QString s, c;
    s.append("List ");
    c = QString::number(curlist);
    s.append(c);
    s.append(" from ");
    c = QString::number(lists);
    s.append(c);
    ui->label_2->setText(s);
}

void MainWindow::set_z()
{
    // определяем глубину
    curz=max_z(curlist);
    //поднимаем-опускаем
    for(int i=0; i<=curz; i++)
        for(int j=0; j<=buf; j++)
            if(toprint[j].list==curlist && toprint[j].z==i)
                toshow[toprint[j].prew].pct->raise(); // поднять выше всех
}


void MainWindow::recomp()
{
    double d;
    for(int i=0; i<=buf; i++)
    {
        toprint[i].show=0;
        d=i+1;
        d=ceil(d/img_on_list);
        toprint[i].list=d;
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
    ui->label_10->setText("1 in center");
}


void MainWindow::on_l2_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(1);
    recomp();
    ui->label_10->setText("1 in top");
}

void MainWindow::on_l3_clicked()
{
    w_cou=1;
    h_cou=2;
    btn_comp_press(2);
    img_on_list=2;
    recomp();
    ui->label_10->setText("1 x 2");
}

void MainWindow::on_l4_clicked()
{
    img_on_list=3;
    w_cou=1;
    h_cou=3;
    btn_comp_press(3);
    recomp();
    ui->label_10->setText("1 x 3");
}

void MainWindow::on_l5_clicked()
{
    img_on_list=4;
    w_cou=2;
    h_cou=2;
    btn_comp_press(4);
    recomp();
    ui->label_10->setText("2 x 2");
}

void MainWindow::on_l6_clicked()
{
    img_on_list=6;
    w_cou=2;
    h_cou=3;
    btn_comp_press(5);
    recomp();
    ui->label_10->setText("2 x 3");
}

void MainWindow::on_l7_clicked()
{
    img_on_list=8;
    w_cou=2;
    h_cou=4;
    btn_comp_press(6);
    recomp();
    ui->label_10->setText("2 x 4");
}

void MainWindow::on_l8_clicked()
{
    img_on_list=9;
    w_cou=3;
    h_cou=3;
    btn_comp_press(7);
    recomp();
    ui->label_10->setText("3 x 3");
}

void MainWindow::on_l9_clicked()
{
    img_on_list=15;
    w_cou=3;
    h_cou=5;
    btn_comp_press(8);
    recomp();
    ui->label_10->setText("3 x 5");
}

void MainWindow::on_l10_clicked()
{
    img_on_list=20;
    w_cou=4;
    h_cou=5;
    btn_comp_press(9);
    recomp();
    ui->label_10->setText("4 x 5");
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
    QPixmap canv_pix(sz*2);
    canv_pix.fill(Qt::transparent); // залить пустотой
    // центр холста
    int x=sz.width();
    int y=sz.height();
    QPainter p(&canv_pix);
    p.translate(x,y);
    p.rotate(g);
    p.translate(-x,-y);
    p.drawPixmap(x/2,y/2, pix);
    p.end();
    int h=x*fabs(sin(r))+ y*fabs(cos(r));
    int w=x*fabs(cos(r))+ y*fabs(sin(r));
    x=x-w/2;
    y=y-h/2;
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
    ui->dial_2->setValue(toprint[bufpress2].rot);
    out_rot=false;
    if (rap) cout << "picture turned successfully" << endl;
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
        int x2, y2;
        float k, k1;
        double d1, d2;
        QRect rc;
        x1=ttx;
        y1=tty;
        x2=x1;
        y2=y1;
        ttx = x;
        tty = y;
        rc=resiz->geometry();
        x1=rc.left()+ttx-x1;
        y1=rc.top()+tty-y1;
        resiz->move(x1,y1);
        rc=toshow[imgpress2].pct->geometry();
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

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    prop=checked;
}

void MainWindow::pct_press_delete()
{
    toprint.erase(toprint.begin()+bufpress2);
    buf--;
    quick_buttons_off();
    make_var_to_list();
    show_pict();
}


void MainWindow::on_dial_2_valueChanged(int value) // вращение картинки
{
    ui->label_7->setText(QString::number(ui->dial_2->value()));
    if (imgpress2 == -1 || out_rot) return;
    rotated(value);
}



void MainWindow::on_checkBox_4_clicked(bool checked)
{
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

void MainWindow::layout_scale()
{
    double d1, d2, k;
    d1=gor_old;
    d2=gor;
    k=d2/d1;
    for(int i=0; i<=buf; i++)
    {
        if(toprint[i].show==1)
        {
            d1=toprint[i].left;   d1=d1*k; toprint[i].left=d1;
            d1=toprint[i].top;    d1=d1*k; toprint[i].top=d1;
            d1=toprint[i].heigth; d1=d1*k; toprint[i].heigth=d1;
            d1=toprint[i].width;  d1=d1*k; toprint[i].width=d1;
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
// долбаный марлин...
{
    int h;
    QString res;
    QByteArray ba=st.toLatin1();
    QString alp;
    alp.append("АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя");
    int i=7;
    while (i<st.length())
    {
        if (st.mid(i, 4)=="%D0%" || st.mid(i, 4)=="%D1%")
           {
             h=1024 + HexToInt(ba[i+2]) * 64 + 16 * (HexToInt(ba[i+4])-8) + HexToInt(ba[i+5]);
             if (h==1025)res.append("Ё");
             if (h==1105)res.append("ё");
             h=h-1040;
             if (h>-1 && h<64)  res.append(alp.mid(h,1));
             i=i+5;
           }
        else res.append(st.mid(i,1));
        i++;
    }
    return res;
}

void MainWindow::show_paper_size()
{
    ui->label_11->setText("Your paper:");
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
}

void MainWindow::show_pict_size()
{
    QSize sz=toshow[imgpress2].pct->size();
    ui->label_11->setText("Curent picture:");
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

void MainWindow::on_checkBox_5_clicked(bool checked)
{
    if (checked) ui->sheet->setFrameStyle(6);
    else  ui->sheet->setFrameStyle(0);
    save_view_sett();
    ui->checkBox_5->setChecked(checked);
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
    rez->setFrameStyle(1);
    rez->raise();
    rez->show();
    quick_buttons_off();
    ui->pushButton_6->show();
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
    toprint[bufpress2].left=rc.left();
    toprint[bufpress2].top=rc.top();
    toprint[bufpress2].width=rc.width();
    toprint[bufpress2].heigth=rc.height();
    toshow[imgpress2].pct->setPixmap(p);
    toshow[imgpress2].pct->setGeometry(rc);
    cout << "clipped successfully!" << endl;
}
