#ifndef REFRESH_H
#define REFRESH_H

#include <QDialog>

namespace Ui {
class refresh;
}

class refresh : public QDialog
{
    Q_OBJECT

public:
    explicit refresh(QWidget *parent = 0);
    ~refresh();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_checkBox_clicked(bool checked);

public slots:
    void clearCheck();          // сбросить флаг "не спрашивать"

private:
    Ui::refresh *ui;
};

#endif // REFRESH_H
