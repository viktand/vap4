#ifndef USERLAYOUT_H
#define USERLAYOUT_H

#include <QDialog>

namespace Ui {
class userlayout;
}

class userlayout : public QDialog
{
    Q_OBJECT

public:
    explicit userlayout(QWidget *parent = 0);
    ~userlayout();

private slots:
    void on_buttonBox_accepted();
    void kill_prv();                // удалить все картинки в окошке предпросмотра компоновки

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_checkBox_clicked(bool checked);

public slots:
    void make_preview();            // построить картинку предпросмотра компоновки
    QSize set_size();               // подогнать размер

private:
    Ui::userlayout *ui;

signals:
    void is_ok();                   // нажат ok

};

#endif // USERLAYOUT_H
