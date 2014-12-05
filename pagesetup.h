#ifndef PAGESETUP_H
#define PAGESETUP_H

#include "ui_pagesetup.h"
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class PageSetup : public QMainWindow, private Ui::PageSetup
{
    Q_OBJECT

    
public:
    explicit PageSetup(QWidget *parent = 0);
    ~PageSetup();

private slots:
    void closeEvent(QCloseEvent *event);
    QString get_run(QString s);     // получить вывод от команды s
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();
    void load_combobox(bool res);    // загрузить спиок форматов бумаг res - необходимость обновить список в основной форме
    void load_combobox2();           // загрузить спиок форматов бумаг (новая версия)
    void load_combobox3(int r);      // переходник на load_combobox
    QString get_real_index(int pap);    // получить текст (название) текущего размера бумаги в списке
    int  get_index(QString s);       // получить индекс по названию в списке размеров

    void on_checkBox_clicked(bool checked);
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void on_pushButton_clicked();   // Ok
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);
    void on_checkBox_5_clicked(bool checked);
    void on_checkBox_6_clicked(bool checked);
    void on_checkBox_7_clicked(bool checked);
    void on_checkBox_8_clicked(bool checked);
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(int index);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_radioButton_toggled();
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_checkBox_10_clicked(bool checked);
    void on_checkBox_13_clicked(bool checked);
    void on_checkBox_11_clicked(bool checked);
    void on_comboBox_4_activated(const QString &arg1);
    void on_checkBox_14_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

public slots:
    void load_data();       // загрузить данные в форму
    void set_all();         // уточнить состояние флага "ориентация для всех"
    void set_path(bool ch); // установить состояние флага сохранения пути
    void reset_result();    // установить код ответа в исходное состояние

private:
    Ui::PageSetup *ui2;

signals:
    void end_set(int result);
    void set_all_em(bool b);
};

#endif // PAGESETUP_H
