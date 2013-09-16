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
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

public slots:
    void load_data(); // загрузить данные в форму
    void set_all();   // уточнить состояние флага "ориентация для всех"

private:
    Ui::PageSetup *ui2;

signals:
    void end_set();
    void set_all_em(bool b);
};

#endif // PAGESETUP_H
