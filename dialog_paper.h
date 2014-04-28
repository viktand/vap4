#ifndef DIALOG_PAPER_H
#define DIALOG_PAPER_H

#include <QDialog>

namespace Ui {
class Dialog_paper;
}

class Dialog_paper : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_paper(QWidget *parent = 0);
    ~Dialog_paper();

private slots:
    void    load_papes();              // загрузить в список варианты размеров бумаги
    void    load_myPap();              // загрузить список предпочитаемых размеров
    void    load_addSizes();           // загрузить дополнительные размеры
    double  get_height(int index);
    double  get_width(int index);
    void    listV_indexCh(int index); // изменение индекса первого сиписка
    void    list_index_cl(int index); // изм. индекса второго списка
    bool    dig(QString s);            // s - цифра?
    void    on_pushButton_2_clicked(); // Ok
    void    on_pushButton_4_clicked();
    void    on_Dialog_paper_finished(int result);
    void    on_pushButton_5_clicked();
    void    on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Dialog_paper *ui;

signals:
    void close_form(int result);

public slots:
    void load_lists();  //загрузить списки
};

#endif // DIALOG_PAPER_H
