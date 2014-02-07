#ifndef PAGESETUP_H
#define PAGESETUP_H

#include "ui_pagesetup.h"

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
    QString get_run(QString s);     // получить вывод от команды s
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();
    void load_combobox();           // загрузить спиок форматов бумаг

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

    void on_spinBox_5_valueChanged(int arg1);

    void on_spinBox_6_valueChanged(int arg1);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_checkBox_9_clicked(bool checked);

    void on_radioButton_toggled();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_checkBox_10_clicked(bool checked);

    void on_checkBox_12_clicked(bool checked);

    void on_checkBox_13_clicked(bool checked);

    void on_checkBox_11_clicked(bool checked);

public slots:
    void load_data(); // загрузить данные в форму
    void set_all();   // уточнить состояние флага "ориентация для всех"
    void set_path(bool ch); // установить состояние флага сохранения пути

private:
    Ui::PageSetup *ui2;

signals:
    void end_set();
    void set_all_em(bool b);
};

#endif // PAGESETUP_H
