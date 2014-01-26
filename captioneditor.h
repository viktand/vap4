#ifndef CAPTIONEDITOR_H
#define CAPTIONEDITOR_H

#include <QDialog>

namespace Ui {
class captioneditor;
}

class captioneditor : public QDialog
{
    Q_OBJECT

public:
    explicit captioneditor(QWidget *parent = 0);
    ~captioneditor();
    QString fullFileName;

public slots:
    void load_caption();        // загрузить надпись в редактор

private slots:
    void on_pushButton_clicked();

    void back_color_set(QColor cl);     // установить цвет фона
    void font_color_set(QColor cl);     // установить цвет букв
    void font_set(QFont fn);            // установить шрифт
    void shift_widgets(int sh);         // сдвинуть виджеты окна, если пришлось изменить высоту строки предпросмотра

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_buttonBox_accepted();

    void on_checkBox_3_clicked(bool checked);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::captioneditor *ui;

signals:
    void end_edit(QString text, QColor f_cl,
                  QColor b_cl, QFont fn, bool shw, bool tr);  // set caption settings

};

#endif // CAPTIONEDITOR_H
