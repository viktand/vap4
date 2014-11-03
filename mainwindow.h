#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QTextEdit>
#include <QTranslator>
#include <QKeyEvent>

// "сверхглобальные" переменные
extern int  paper_w;    // ширина бумаги
extern int  paper_h;    // высота бумаги
extern bool rap;        // выводить сообщения программы в терминал
extern bool all_rot;    // общая ориентация для всех листов
extern int  ul_hor;     // пользовательская компоновка - по горизонтале
extern int  ul_ver;     // пользовательская компоновка - по вертикале
extern QString p_name;  // имя принтера
extern bool set_orn;    // ориентация листа true - портретная, используется для программного нажатия кнопки при выборе пользовательской компоновки
extern bool print_color;// true - печать в цвете
extern QString list_n;  // описание текущего листа
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
extern bool testPrint;  // печать тестового креста (для проверки позиционирования)
extern int sheet_size;  // индекс размера листа (для передачи параметра из окна настроек)
extern bool all_sizes;  // true - все листы имеют одинаковый размер бумаги (по умолчанию)


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
    int fn_size;            // размер фонта
    struct arch {char *el[];};


private slots:

    // слоты, написанные руками
    void btn_comp_press(int i);             // завершение обработки на жатия на одну из кнопок компоновки
    void pct_press(int x, int y, int i);    // нажатие мышки на превьюшке
    void pct_move(int x, int y, int i);     // движение мышки по превьюшке
    void pct_up(int x, int y, int i);       // отпускание мышки на превьюшке
    void open_pct(QString filename);        // открыть картинку
    void load_param();                      // загрузить параметры из командной строки
    void show_pict();                       // показать картинки текущего листа
    void kill_pict();                       // удалить все превьюшки
    void set_z();                           // установить z-порядок для превьюшек
    void recomp();                          // перекомпоновать все
    void recomp_curlist();                  // перекомпоновать текущий лист
    double get_scaleX();                    // расчет масштаба печати по Х для листа i
    double get_scaleY();                    // расчет масштаба печати по Y для листа i
    void make_list();                       // построить шаблон для листа предпросмотра
    void get_pp();                          // расчитать точки на мм для принтера
    void set_printer(int index);            // настроить принтер
    void set_setting(int r);                // применить настройки
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
    void show_caption(int index);           // показать/скрыть подпись к картинке index - абсолютный номер sh "true" - показать
    void cp_press(int x, int y, int i);     // нажатие мышки на подписи
    void cp_move(int x, int y, int i);      // движение мышки по подписи
    void cp_up(int x, int y, int i);        // отпускание ммышки на подписе
    void cp_setGeometry(int index);         // задать геометрию для подписи к картинке № index
    void cp_setPixmap(int index);           // создать картинку подписи
    void turn_caption();                    // переключить состояние подписи (вкл/выкл) для bufpress2
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
    QString GetPathFrom(QString pFile);     // получить путь к файлу по его полному имени (отбросить имя из пути), без слеша на конце
    void  prePint();                        // прогон листов перед печатью.
    void  setAutoOrn();                     // Автоматически установить оринетацию бумаги
    void  setInterface();                   // Установить вид интерфейса (лента/обычный)
    void  setIconOrns(bool b);              // Установить иконки на кнопках ориентации бумаги true - портретная
    void  mouseWheel(int i, int index);     // Вращение колесика мыши - пролистывание страниц колесиком мыши
    void  img_size_ch(int step);            // Изменить размер текущей картинки на step точек по горизонтале
    void  set_printer_pap_size(int i);      // Установить размер бумаги принтера для листа i
    void  load_my_pSizes();                 // Загрузить список предпочтительных размеров бумаги на ленту
    void  set_rott_btn();                   // создать кнопку вращения картинки
    void  set_delt_btn();                   // создать кнопку удаления картинки
    void  set_resiz_btn();                  // создать кнопку изм. размера картинки
    void  set_clip_btn();                   // создать кнопку обрезки картинки
    void  set_timer();                      // создать таймер
    void  hide_interf();                    // скрытый интерфейс для запуска
    void  slfunc1();                        // служебная функция для сокращения повторов кода
    void  setPrinter();                     // общая настройка принтера
    void  setNewSizeOfPct();                // зафиксировать новый размер после изменения кнопкой из ленты (и положение)
    void  moveTo(int x, int y);             // сдвинуть выбранную картинку на соотв. количество точек
    void  resizeTo(float c);                // изменить размер картинки в с раз
    void  showPctBord(bool b);              // показать/скрыть рамку вокруг картинки - типа курсор на картинке
    void  setCaptionRect(int index, QRect r);// сохранить значения геометрии подписи для index
    QRect getCaptionRect(int index);        // получить значение геометрии подписи для index
    void  setNewPix(QPixmap p);             // применить результат трансформации
    QString get_run(QString s);             // выполнить команду s и вернуть вывод от нее

    // Здесь и далее слоты событий виджетов главной формы, сгенерированные автоматически
    void on_l1_2_clicked();
    void on_l2_2_clicked();
    void on_l3_2_clicked();
    void on_l4_2_clicked();
    void on_l5_2_clicked();
    void on_l6_2_clicked();
    void on_l7_2_clicked();
    void on_l8_2_clicked();
    void on_l9_2_clicked();
    void on_l10_2_clicked();
    void on_pushButton_38_clicked();
    void on_pushButton_39_clicked();
    void on_pushButton_25_clicked();
    void on_pushButton_26_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_28_clicked();
    void on_pushButton_37_clicked();
    void on_pushButton_32_clicked();
    void on_pushButton_30_clicked();
    void on_pushButton_31_clicked();
    void on_pushButton_29_clicked();
    void on_pushButton_35_clicked();
    void on_pushButton_33_clicked();
    void on_pushButton_34_clicked();
    void on_checkBox_7_clicked(bool checked);
    void on_checkBox_8_clicked(bool checked);
    void on_checkBox_10_clicked(bool checked);
    void on_checkBox_12_clicked(bool checked);
    void on_pushButton_36_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_dial_3_valueChanged(int value);
    void on_dial_4_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_18_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;                 // Рождение


public slots:
    void if_show();                     // Обработка события "Показать форму"
    void set_all_rot(bool b);           // Обработка изменения флага "Общая ориентация для всех листов"

signals:
    void loadpct(QString filename);     // Сигнал "Надо загрузить картинку из файла"

protected:
    void resizeEvent(QResizeEvent *e);   // событие изменение размера окна
    void closeEvent(QCloseEvent *cl);    // событие закрытия окна
    void moveEvent( QMoveEvent *e);
    void dragEnterEvent(QDragEnterEvent *event);  //
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
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
