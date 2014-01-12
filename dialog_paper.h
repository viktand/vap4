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
    void load_data(QString paper_name, int w, int h, int index);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_paper *ui;

signals:
    void close_form();
};

#endif // DIALOG_PAPER_H
