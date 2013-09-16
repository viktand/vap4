#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

extern int ornl;
extern int left_m;
extern int right_m;
extern int top_m;
extern int bottom_m;
extern int paper_w;
extern int paper_h;
extern bool rap;
extern bool all_rot;


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
    int cou_prm;        // кол-во параметров
    struct arch {char *el[];};


private slots:

    void btn_comp_press(int i);
    //void show_img (QString fl);
    //void load_file(QString fl);
    void pct_press(int x, int y, int i);
    void pct_move(int x, int y, int i);
    void pct_up(int x, int y, int i);
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
    void start_load_picture();
    void end_load_picture(QImage image);
    void end_rotation();                    // завершение поворота листа
    void set_wind_size();                   // подогнать размер окна программы
    void set_indic_pos();                   // позиционирование индикатора загрузки
    void make_var_to_list();                // подготовить переменные для функции make_list
    void fn_1();                            // группа функций, которые несколько раз идут подряд

    void on_l1_clicked();

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

private:
    Ui::MainWindow *ui;


public slots:
    void on_show();
    void new_margins();
    void set_all_rot(bool b);

signals:
    void loadpct(QString filename);




};

class loadpicture : public QObject
{
    Q_OBJECT

public:
    explicit loadpicture(QObject *parent = 0);

signals:
    void sendImage(QImage); // вернуть результат
    void start();  // готов к работе

public slots:
    void start_load(QString filename); // загрузка
};

#endif // MAINWINDOW_H
