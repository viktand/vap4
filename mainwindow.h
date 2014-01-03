#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QResizeEvent>
#include <QtGui/QCloseEvent>
#include <QTextEdit>

// "сверхглобальные" переменные
extern int  ornl;       // ориентация листа
extern int  left_m;     // левое поле
extern int  right_m;    // правое поле
extern int  top_m;      // верхнее поле
extern int  bottom_m;   // нижнее поле
extern int  paper_w;    // ширина бумаги
extern int  paper_h;    // высота бумаги
extern bool rap;        // выводить сообщения программы в терминал
extern bool all_rot;    // общая ориентация для всех листов
extern int  ul_hor;     // пользовательская компоновка - по горизонтале
extern int  ul_ver;     // пользовательская компоновка - по вертикале
extern QString p_name;  // имя принтера
extern bool set_orn;    // ориентация листа true - портретная, используется для программного нажатия кнопки при выборе пользовательской компоновки
extern bool print_color;// true - печать в цвете
extern bool printer_a3; // true - A3 (big size paper)
extern QString list_n;  // описание текущего листа
extern int  prn_size_x; // базовый размер листа для принтера (ширина в мм)
extern int  prn_size_y; // базовый размер листа для принтера (высота в мм)
extern double h_ofsett; // горизонтальное смещение позиции печати
extern int pap_sor;     // источник бумаги
extern int pap_name;    // имя бумаги (по списку)
extern QString caption; // редактируемая подпись
extern bool show_cap;   // показывать подпись
extern QColor font_cl;  // цвет шрифта подписи
extern QColor back_cl;  // цвет шрифта фона
extern QFont  font_cpt; // шрифт подписи
extern bool frm_cpt;    // показывать рамку подписи
extern bool trans;      // прозрачный фон надписи
extern double font_scl; // масштаб шрифта в предпросмотре


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStringList prm;        // список параметров запуска программы
    int cou_prm;            // кол-во параметров
    struct arch {char *el[];};


private slots:

    void btn_comp_press(int i);             // завершение обработки на жатия на одну из кнопок компоновки
    void pct_press(int x, int y, int i);    // нажатие мышки на превьюшке
    void pct_move(int x, int y, int i);     // движение мышки по превьюшке
    void pct_up(int x, int y, int i);       // отпускание ммышки на превьюшке
    void open_pct(QString filename);        // открыть картинку
    void load_param();                      // загрузить параметры из командной строки
    void show_pict();                       // показать картинки текущего листа
    void kill_pict();                       // удалить все превьюшки
    void set_z();                           // установить z-порядок для превьюшек
    void recomp();                          // перекомпоновать все
    void recomp_curlist();                  // перекомпоновать текущий лист
    double get_scaleX();                    // расчет масштаба печати по Х
    double get_scaleY();                    // расчет масштаба печати по Y
    void make_list();                       // построить шаблон для листа предпросмотра
    void get_pp();                          // расчитать точки на мм для принтера
    void set_printer();                     // настроить принтер
    void set_setting();                     // применить настройки
    QString get_file();                     // получить имя файла из диалога для загрузки
    void ind_start();                       // старт индикатора
    void ind_stop();                        // стоп индикатора
    void start_load_picture();              // начать (да и совершить) загрузку картинки
    void end_load_picture(QImage image);    // завершение загрузки картинки
    void end_rotation();                    // завершение поворота листа
    void set_wind_size();                   // подогнать размер окна программы
    void set_indic_pos();                   // позиционирование индикатора загрузки
    void pct_press_rott();                  // нажатие на иконку поворота картинки (превьюшки)
    void istimer();                         // сработка таймера - погасить иконки бысторого редактирования по угла превьюшки
    void rotated(int g);                    // вращение на угол g грд по часовой стрелке
    void reShow(int index);                 // перепоказать картинку index (она и так на экране, просто обновить)
    void resiz_press(int x, int y);         // слоты обработки сигналов иконки изменения размера картинки
    void resiz_move(int x, int y);          // ---
    void resiz_up();                        // ---
    void set_user_layout();                 // установить пользовательскую компоновку
    void img_size_cur_comp();               // вычислить максимальные размеры картинок для текущей компоновки
    void pct_press_delete();                // удаление картинки
    void fill_all();                        // установить или снять режим заполнения
    void redraw();                          // перерисовать окно предпросмотра
    void layout_scale();                    // пересчитать компоновку пропорционально на текущий размер
    void save_wind_size();                  // сохранить размеры главного окна
    void rest_sind_size();                  // восстановить размеры главного окна
    void save_printer_sett();               // сохранить настройки принтера
    void rest_printer_sett();               // восстановить настройки принтера
    void set_btn_pos(int d);                // расстановка кнопок компоновки
    QString esc_to_utf(QString st);         // преобразовать escape последовательность в utf-строку
    void show_paper_size();                 // показать размер бумаги в окошке на панеле радактирования
    void save_view_sett();                  // сохранить некоторые настройки внешнего вида
    void rest_view_sett();                  // восстановить некоторые настройки внешнего вида
    void set_layout(int c);                 // Загрузить компоновку № с
    void show_pict_size();                  // показать размер картинки в окошке на панеле радактирования
    QPixmap cut_pix(QPixmap pix, QRect rc); // вырезать кусочек из пиксмэпа
    void show_clip();                       // включить рамку обрезки
    void quick_buttons_off();               // выключить быстрые кнопки
    void rez_press(int x, int y);           // нажатие на рамку обрезки
    void rez_move(int x, int y);            // движение по рамке обрезки
    void rez_up();                          // отпускание рамки
    void load_folder(QString fn);           // загрузить содержимое папки fn
    void show_r_menu(int x, int y, int i);  // показать контекстное меню, i - номер картинки, x,y -координаты мышки
    void set_rmenu(int index);              // собрать меню типа index
    void rot();                             // повернуть картинку на 90 грд;
    void make_menu_1();                     // сделать меню 1
    void make_menu_2();                     // сделать меню 2
    void paint_frame();                     // нарисовать рамку обрезки (саму рамку)
    void show_cap_editor();                 // открыть окно редактора подписи
    void set_caption(QString text,
         QColor f_cl, QColor b_cl,
         QFont fn, bool shw, bool tr);      // установить подпись
    void show_caption(int index, bool sh);  // показать/скрыть подпись к картинке index - абсолютный номер sh "true" - показать
    void cp_press(int x, int y, int i);     // нажатие мышки на подписи
    void cp_move(int x, int y, int i);      // движение мышки по подписи
    void cp_up(int x, int y, int i);        // отпускание ммышки на подписе
    void cp_setGeometry(int index);         // задать геометрию для подписи к картинке № index
    void cp_setPixmap(int index);           // создать картинку подписи
    void turn_caption();                    // переключить состояние подписи (вкл/выкл) для bufpress2
    void reScl();                           // пересчет масштаба и позиций подписей
    void setTextBlock(QString text,
         QColor BackColor, QColor LitColor,
         QFont TextFont, bool trans,
                      int alig);            // создать (изменить) текстовый блок
    void addTextPictute(QPixmap pixmap,
         QString text, QColor BackColor,
         QColor LitColor, QFont TextFont,
              bool trans, int alig);        // добавить картинку из текстового блока
    bool  isvap(QString s);                 // true. если s заканчивается на ".vap"
    bool  saveSassion(QString fileName);    // сохранить текущую сессию
    bool  openSassion(QString fileName);    // открыть сессию
    void  set_ornt_list();                  // установить ориентацию листа (не менять, а установить как уже было)
    void  move_prev();                      // переместить картинку на предыдущий лист
    void  move_next();                      // переместить картинку на следующий лист
    void  edit_textBlock();                 // редактировать текстовый блок
    void  open_textblockEd();               // открыть редактор текстовых блоков
    void  get_marg();                       // расчет области непечати

    void on_l1_clicked();                   // Здесь и далее слоты событий виджетов главной формы
    void on_l2_clicked();
    void on_l3_clicked();
    void on_l4_clicked();
    void on_l5_clicked();
    void on_l6_clicked();
    void on_l7_clicked();
    void on_l8_clicked();
    void on_l9_clicked();
    void on_l10_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_checkBox_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_dial_valueChanged(int value);
    void on_dial_2_valueChanged(int value);
    void on_checkBox_4_clicked(bool checked);
    void on_verticalScrollBar_valueChanged(int value);
    void on_checkBox_5_clicked(bool checked);
    void on_pushButton_6_clicked();
    void on_pushButton_9_clicked();
    void on_checkBox_6_clicked();
    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;                 // Рождение


public slots:
    void if_show();                     // Обработка события "Показать форму"
    void new_margins();                 // Обработка обновления размеров полей
    void set_all_rot(bool b);           // Обработка изменения флага "Общая ориентация для всех листов"

signals:
    void loadpct(QString filename);     // Сигнал "Надо загрузить картинку из файла"

protected:
    void resizeEvent(QResizeEvent *e);   // событие изменение размера окна
    void closeEvent(QCloseEvent *cl);    // событие закрытия окна
    void dragEnterEvent(QDragEnterEvent *event);  //
    void dropEvent(QDropEvent *event);
};

// Класс, загружающий картинку. Выносится в отдельный поток, чтобы не подвешивать основное окно

class loadpicture : public QObject
{
    Q_OBJECT

public:
    explicit loadpicture(QObject *parent = 0);

signals:
    void sendImage(QImage);             // вернуть результат загрузки
    void start();                       // готов к работе

public slots:
    void start_load(QString filename);  // загрузка
};



#endif // MAINWINDOW_H
