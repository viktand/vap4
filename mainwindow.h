#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void make_var_to_list();                // подготовить переменные для функции make_list
    void fn_1();                            // группа функций, которые несколько раз идут подряд
    void pct_press_rott();                  // нажатие на иконку попорота картинки (превьюшки)
    void istimer();                         // сработка таймера - погасить иконки бысторого редактирования по угла превьюшки
    void rotated();                         // вращение на угол 90 грд по часовой стрелке
    void reShow(int index);                 // перепоказать картинку index (она и так на экране, просто обновить)
    void resiz_press(int x, int y);         // слоты обработки сигналов иконки изменения размера картинки
    void resiz_move(int x, int y);          // ---
    void resiz_up();                        // ---
    void set_user_layout();                 // установить пользовательскую компоновку
    void img_size_cur_comp();               // вычислить максимальные размеры картинок для текущей компоновки

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
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_checkBox_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);


    void on_dial_valueChanged(int value);

private:
    Ui::MainWindow *ui;                 // Рождение


public slots:
    void if_show();                     // Обработка события "Показать форму"
    void new_margins();                 // Обработка обновления размеров полей
    void set_all_rot(bool b);           // Обработка изменения флага "Общая ориентация для всех листов"

signals:
    void loadpct(QString filename);     // Сигнал "Надо загрузить картинку из файла"
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
