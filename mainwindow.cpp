#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pagesetup.h"
#include "setting.h"
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



#include <iostream>
using namespace std;


// глобальные переменные
QPrinter *printer;       // принтер, в него рисовать и настраивать
QPainter *pntr;          // устройство рисования (с него печатать)
PageSetup *ps;
setting *st;
userlayout *uslay;
QMovie *animGif;
QTimer *timer;
//QThread *thread;
loadpicture *ldp;
QString str_time;
bool flag2;
int  count = 0;
int	 img_count = -1; 	// количество превьюшек
int	 comp = 0;			// текущая компоновка
int  getx = 0;			// максимальный размер х для картинки  в этой компоновке
int  gety = 0;			// максимальный размер y для картинки  в этой компоновке
int  x_pg = 210;		// текущий размер листа x
int  y_pg = 297;		// текущий размер листа y
int  prt_x= 210;        // x для портретной ориентации
float  scl_pg = 2;		// масштаб листа предпросмотра
int  gor = 420;			// x листа предпросмотра
int  ver = 594;			// y листа предпросмотра
int  pg_mar = 10;		// поля листа бумаги
int  pol = 10;			// поля между картинок
int  crd[20][2];		// список стандартных координат для текущей компоновки
bool orn = true;        // ориентация листа true - портретная
int  buf=-1; 		    // количество загруженных картинок, отсчет с нуля
int  ttx, tty;			// временные значения для отработки перемещения объектов
int  imgpress=-1;		// № нажатой превьюшки
int  bufpress=-1;		// № картинки в общем списке, превью которой нажато
int  imgpress2=0;		// № нажатой превьюшки
int  bufpress2=0;		// № картинки в общем списке, превью которой нажато
int  curz=0;            // z порядок для текущей страницы, используется для расчета наложений
int  lists=0, curlist=0;// всего листов и текущий лист
int  img_on_list;       // кол-во картинок на листе в текущей компоновке
int  ppx, ppy;          // точек на мм по Х и У для текущего принтера
int  x_prw=190, y_prw=70; // координаты листа предпросмотра
bool rott_press=false;   // флаг нажатия на ручку вращения
bool prop=true;         // флаг пропорциональности при изменении размера
int  w_cou=1, h_cou=1;  // компоновка w_cou x h_cou
int  w_fon, h_fon;      // текущие размеры фона
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




struct pict {                // загруженная картинка и все ее свойства
    QString   pict;          // путь к файлу
    int       rot;           // 0 - нормально, угол поворота по часовой стрелке
    QImage    pct;           // картинка
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
};

struct prew {
    QavLabel     *pct;        //картинка
    int          buf;         //абсолютный номер
};

QavLabel *rott;              // "ручка" вращения
QavLabel *resiz;             // "ручка" изменения размера


std::vector<pict> toprint;      //массив загруженных картинок
std::vector<prew> toshow;       //массив превьющек
std::vector<bool> list_orn;     // массив ориентаций листов true - портретная ориентация


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  // конструктор
{
    ui->setupUi(this);
    on_l1_clicked();
    top_m=5;
    left_m=5;
    right_m=5;
    bottom_m=5;
    all_rot=true;
    ul_hor=3;
    ul_ver=3;
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
    rott = new QavLabel(ui->fon);
    QImage im(":/new/prefix1/rotation");
    rott->setPixmap(QPixmap::fromImage(im));
    rott->setScaledContents(true);
    rott->setGeometry(0,0,16,16);
    rott->setCursor(Qt::PointingHandCursor);
    rott->hide();
    QObject::connect(rott, SIGNAL(mouse_press(int, int, int)), this, SLOT(pct_press_rott()));
    resiz = new QavLabel(ui->fon);
    QImage im2(":/new/prefix1/resize");
    resiz->setPixmap(QPixmap::fromImage(im2));
    resiz->setScaledContents(true);
    resiz->setGeometry(0,0,16,16);
    resiz->setCursor(Qt::PointingHandCursor);
    resiz->hide();
    QObject::connect(resiz, SIGNAL(mouse_press(int, int, int)), this, SLOT(resiz_press(int, int)));
    QObject::connect(resiz, SIGNAL(mouse_move(int, int, int)),  this, SLOT(resiz_move(int, int)));
    QObject::connect(resiz, SIGNAL(mouse_up(int, int, int)), this, SLOT(resiz_up()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(istimer()));
    ui->checkBox_3->hide();
    make_list();
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
}

void MainWindow::open_pct(QString filename) // добавить картинку в список
{

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

void MainWindow::set_wind_size()
{
    int w,h;
    h=688;
    w=x_prw+ui->list->geometry().width()+10;
    this->resize(w,h);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

void MainWindow::make_var_to_list()
{
    orn=list_orn[curlist-1];
    if (orn)
    {
        if (prt_x != x_pg)
        {
            ui->pushButton_10->setIcon(ui->pushButton_11->icon());
            ui->pushButton_11->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
            fn_1();
        }
    }
    else
    {
        if (prt_x == x_pg)
        {
            ui->pushButton_11->setIcon(ui->pushButton_10->icon());
            ui->pushButton_10->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
            fn_1();
        }
    }
}

void MainWindow::on_pushButton_2_clicked() //открыть 1 файл
{
    QString fileName = get_file();
    if (!fileName.isEmpty())
    {
        ind_start();
        open_pct(fileName);
        ind_stop();
    }
}

void MainWindow::on_pushButton_clicked() // all in oun
{
   btn_comp_press(10);
   img_on_list=buf+1;
   recomp();
}

void MainWindow::on_pushButton_8_clicked() // my layout
{
    btn_comp_press(11);
    if(uslay == 0)
    {
        uslay=new userlayout();
        connect(uslay, SIGNAL(is_ok()), this, SLOT(set_user_layout()));
        uslay->make_preview();
    }
    uslay->show();
    //img_on_list=0;
    //recomp();
}



void MainWindow::on_pushButton_4_clicked() // вперед
{
    if (curlist<lists)
    {
        curlist++;
        if (list_orn.size()<lists) list_orn.push_back(list_orn.back());
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

void MainWindow::on_pushButton_6_clicked() // настройка принтера
{

    QPrintDialog dialog(printer);
    printer->Landscape;
    dialog.setWindowTitle(tr("Printer setup"));
    if (dialog.exec() != QDialog::Accepted)
         return;
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
   ind_stop();
}

void MainWindow::on_pushButton_9_clicked()
{
    if(st == 0) st=new setting();
    st->show();
}

void MainWindow::on_pushButton_11_clicked() // ландшафтная ориентация
{
    orn=false;
    if (prt_x != x_pg) return;
    ui->pushButton_11->setIcon(ui->pushButton_10->icon());
    ui->pushButton_10->setIcon(QIcon::fromTheme(":/new/prefix1/sheet"));
    end_rotation();
}

void MainWindow::on_pushButton_10_clicked() // портретная ориентация
{
    orn=true;
    if (prt_x == x_pg) return;
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
        for(int i=0; i<=buf; i++)  if (list_orn[toprint[i].list] != orn) toprint[i].show=0;
        for(int i=0; i<lists; i++) list_orn[i]=orn;
    }
}

void MainWindow::set_all_rot(bool b)
{
   all_rot=b;
   ui->checkBox->setChecked(b);
}

void MainWindow::on_pushButton_5_clicked() //печать
{
    set_printer(); // настроить принтер
    QRect rc;   // шаблон для печати
    double sclX, sclY; // коэф. масштабирования при пересчете размеров и координат
    double t;   // time var
    sclX=get_scaleX();
    sclY=get_scaleY();
    for (int i=0; i<=lists; i++)
    {
        pntr->begin(printer); // начать рисование
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
                    pntr->drawImage(rc, toprint[j].pct);
                    cout << "rect size: heigth = "<< rc.height() << "; width = " << rc.width() << " " << endl;
                    cout << "picture " << j << " : " << " painted" << endl;

             }
        }
        pntr->end(); // отправить на печать
        cout << "end printing" << endl;
    }
}

void MainWindow::fn_1()
{
    swap(x_pg, y_pg);
    gor=x_pg*scl_pg;
    ver=y_pg*scl_pg;
    make_list();
    set_wind_size();
}

void MainWindow::end_rotation()
{
    fn_1();
    if (ui->checkBox->checkState()) for (int i=0; i<lists; i++) list_orn[i]=orn;
    btn_comp_press(comp);
    if (all_rot)
    {
        for(int i=0; i<lists; i++) list_orn[i]=orn;
        recomp();
    }
        else
    {
        list_orn[curlist-1]=orn;
        recomp_curlist();
    }
    set_indic_pos();

}

void MainWindow::end_load_picture(QImage image)
{
        if (image.isNull()) { } // неудачная загрузка
        double d;
        toprint[buf].pct = image;
        toprint[buf].show= 0;
        d=buf+1;
        d=ceil(d/img_on_list);
        toprint[buf].list=d;
        lists=0;
        for(int i=0; i<=buf; i++) if(toprint[i].list>lists) lists=toprint[i].list;
        if (rap) cout << "picture was loaded" << endl;
        img_size_cur_comp();
        show_pict();
        flag2=false;
}

void MainWindow::start_load_picture()
{
    ldp->start_load(str_time);
}

void MainWindow::pct_press(int x, int y, int i) // нажатие на превьюшку
{
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
    int rx, ry;
    rx=toprint[bufpress].left-8;
    if (rx<0) rx=0;
    ry=toprint[bufpress].top-8;
    if (ry<0) ry=0;
    rott->show();
    rott->move(rx,ry);
    rott->raise();
    rx=toprint[bufpress].left-8+toprint[bufpress].width;
    if (rx>ui->fon->width()) rx=ui->fon->width()-16;
    ry=toprint[bufpress].top-8+toprint[bufpress].heigth;
    if (ry>ui->fon->height()) ry=ui->fon->height()-16;
    resiz->show();
    resiz->move(rx,ry);
    resiz->raise();
    ui->checkBox_3->setChecked(prop);
    ui->checkBox_3->show();
}

void MainWindow::pct_move(int x, int y, int i)
{
    {
        int x1=0, y1=0;
        x1=ttx;
        y1=tty;
        ttx = x;
        tty = y;
        x1=toprint[bufpress].left+ttx-x1;
        y1=toprint[bufpress].top+tty-y1;
        toshow[i].pct->move(x1,y1);
        toprint[bufpress].left=x1;
        toprint[bufpress].top=y1;
        rott->move(x1-8, y1-8);
        x1=x1+toprint[bufpress].width-8;
        y1=y1+toprint[bufpress].heigth-8;
        resiz->move(x1,y1);
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
    }
}

void MainWindow::istimer()
{
    timer->stop();
    rott->hide();
    resiz->hide();
    ui->checkBox_3->hide();
}

void MainWindow::recomp_curlist()
{
    for(int i=0; i<=buf; i++)
        if (toprint[i].list==curlist)
            toprint[i].show=0;
    show_pict();
}

double MainWindow::get_scaleX()
{
   double x_size;
   double x_prew;
   x_size=printer->pageRect().width();
   x_prew=ui->fon->width();
   cout << "scale X for printer: " << x_size/x_prew << endl;
   return x_size/x_prew;
}

double MainWindow::get_scaleY()
{
   double x_size;
   double x_prew;
   x_size=printer->pageRect().height();
   x_prew=ui->fon->height();
   cout << "scale Y for printer: " << x_size/x_prew << endl;
   return x_size/x_prew;
}

void MainWindow::set_printer()
{
     printer->setPageMargins(left_m,top_m,right_m, bottom_m,QPrinter::Millimeter);
}

void MainWindow::set_setting()
{
    new_margins();
}

void MainWindow::new_margins()
{
    make_list();
    show_pict();
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
    int x, y;
    comp=i;
    img_size_cur_comp ();
    // вычсляем координаты галки
    x = 5+(i%2)*65;
    y = 70+85*(i/2);
    // ставим метку на кнопку
    ui->checkset->setGeometry(x,y,32,32);
}

void MainWindow::set_user_layout()
{
    img_on_list=ul_hor*ul_ver;
    w_cou=ul_hor;
    h_cou=ul_ver;
    btn_comp_press(11);
    recomp();
}

void MainWindow::get_pp()
{
    ppx=ui->list->width()/x_pg;
    ppy=ui->list->height()/y_pg;
}

void MainWindow::set_indic_pos()
{
    int x,y;
    x=(ui->sheet->geometry().width()-128)/2;
    y=(ui->sheet->geometry().height()-128)/2;
    ui->label->setGeometry(x,y,128,128);
}

void MainWindow::make_list() // создать лист предпросмотра
{
    int x, y, w, h;
    ui->list->setGeometry(x_prw, y_prw, x_pg*scl_pg, y_pg*scl_pg);
    get_pp();
    x=x_prw+left_m*ppx;
    y=y_prw+top_m*ppy;
    w=x_pg*scl_pg-left_m*ppx-right_m*ppx;
    h=y_pg*scl_pg-top_m*ppy-bottom_m*ppy;
    ui->sheet->setGeometry(x, y, w, h);
    ui->fon->setGeometry(0, 0, w, h);
    w_fon=w;
    h_fon=h;
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
    if (buf>-1)
    {
        rott->hide();
        resiz->hide();
    }
    QSize sz;
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
          toshow[img_count].pct = new QavLabel(ui->fon);
          toshow[img_count].pct->setBackgroundRole(QPalette::Base);
          toshow[img_count].pct->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
          toshow[img_count].pct->setScaledContents(true);
          toshow[img_count].pct->setPixmap(QPixmap::fromImage(toprint[i].pct));
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
                x=(img_count%w_cou)*(w_fon/w_cou)+(getx-sz.width())/2;
                y=ceil(img_count/w_cou)*(h_fon/(h_cou*n))+(gety-sz.height())/2;
                toshow[img_count].pct->setGeometry(x, y, sz.width(), sz.height());
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
}

void MainWindow::on_l1_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(0);
    recomp();
}


void MainWindow::on_l2_clicked()
{
    img_on_list=1;
    w_cou=1;
    h_cou=1;
    btn_comp_press(1);
    recomp();
}

void MainWindow::on_l3_clicked()
{
    w_cou=1;
    h_cou=2;
    btn_comp_press(2);
    img_on_list=2;
    recomp();
}

void MainWindow::on_l4_clicked()
{
    img_on_list=3;
    w_cou=1;
    h_cou=3;
    btn_comp_press(3);
    recomp();
}

void MainWindow::on_l5_clicked()
{
    img_on_list=4;
    w_cou=2;
    h_cou=2;
    btn_comp_press(4);
    recomp();
}

void MainWindow::on_l6_clicked()
{
    img_on_list=6;
    w_cou=2;
    h_cou=3;
    btn_comp_press(5);
    recomp();
}

void MainWindow::on_l7_clicked()
{
    img_on_list=8;
    w_cou=2;
    h_cou=4;
    btn_comp_press(6);
    recomp();
}

void MainWindow::on_l8_clicked()
{
    img_on_list=9;
    w_cou=3;
    h_cou=3;
    btn_comp_press(7);
    recomp();
}

void MainWindow::on_l9_clicked()
{
    img_on_list=15;
    w_cou=3;
    h_cou=5;
    btn_comp_press(8);
    recomp();
}

void MainWindow::on_l10_clicked()
{
    img_on_list=20;
    w_cou=4;
    h_cou=5;
    btn_comp_press(9);
    recomp();
}


void MainWindow::pct_press_rott() // нажатие на ручку вращения
{
    rott->raise(); // поднять выше всех
    rotated();
    timer->stop();
    timer->start(3000);
}


void MainWindow::rotated()
{
    int w, h, x, y;
    QPixmap shipPixels(*toshow[imgpress2].pct->pixmap());
    QSize size;
    QRect rc;
    x=shipPixels.width();
    y=shipPixels.height();
    size=shipPixels.size();
    rc=toshow[imgpress2].pct->geometry();
    w=rc.width();
    h=rc.height();
    rc.setWidth(h);
    rc.setHeight(w);
    QPixmap rotatePixmap(size*2);
    rotatePixmap.fill(Qt::transparent);
    QPainter p(&rotatePixmap);
    p.translate(rotatePixmap.size().width()/2, rotatePixmap.size().height()/2);

    p.rotate(90); // градус
    p.translate(-rotatePixmap.size().width()/2, -rotatePixmap.size().height()/2);
    p.drawPixmap(y,x, shipPixels);
    p.end();
    shipPixels = rotatePixmap.copy(0,2*y-x, y,x) ;

    toprint[bufpress2].pct=shipPixels.toImage();
    x=toprint[bufpress2].width;
    y=toprint[bufpress2].heigth;
    toprint[bufpress2].heigth=x;
    toprint[bufpress2].width=y;
    reShow(imgpress2);
    int rx, ry;
    rx=toprint[bufpress2].left-8+toprint[bufpress2].width;
    if (rx>ui->fon->width()) rx=ui->fon->width()-16;
    ry=toprint[bufpress2].top-8+toprint[bufpress2].heigth;
    if (ry>ui->fon->height()) ry=ui->fon->height()-16;
    resiz->move(rx,ry);
    resiz->raise();
    if (rap) cout << "picture turned successfully" << endl;
}

void MainWindow::reShow(int index)
{
    QSize sz;
    int x, y, i;
    i=toshow[index].buf;
    if (toprint[i].list==curlist)
      {   // обновить превью для картинки index (№ превью)
          toshow[index].pct->setPixmap(QPixmap::fromImage(toprint[i].pct));
          sz=setsize(toshow[img_count].pct->pixmap()->size());
          x=toprint[i].left;
          y=toprint[i].top;
          toshow[index].pct->setGeometry(x, y,toprint[i].width, toprint[i].heigth);
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
        int x1, y1;
        int x2, y2;
        float k, k1;
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
        k=toshow[imgpress2].pct->pixmap()->height();
        k1=toshow[imgpress2].pct->pixmap()->width();
        k=k/k1;
        rc.setWidth(rc.width()+(ttx-x2));
        if (prop) rc.setHeight(rc.width()*k);
        else      rc.setHeight(rc.height()+(tty-y2));
        toshow[imgpress2].pct->setGeometry(rc);
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
    if (rx>ui->fon->width()) rx=ui->fon->width()-16;
    ry=toprint[bufpress2].top-8+toprint[bufpress2].heigth;
    if (ry>ui->fon->height()) ry=ui->fon->height()-16;
    resiz->move(rx,ry);
}

void MainWindow::on_checkBox_3_clicked(bool checked)
{
    prop=checked;
}


