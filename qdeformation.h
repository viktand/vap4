#ifndef QDEFORMATION_H
#define QDEFORMATION_H

#include <QDialog>
#include <avLabel.h>

namespace Ui {
class Qdeformation;
}

class Qdeformation : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QPixmap pix READ getPixmap WRITE setPixmap) // свойство - трансформируемое изображение

private:
    QPixmap pix;

public:
    explicit Qdeformation(QWidget *parent = 0);
    ~Qdeformation();
    QPixmap  getPixmap()
    {
        return pix;
    }
    void setPixmap(QPixmap value)
    {
        pix=value;
        loadPixmap();
    }

private slots:
    void    loadPixmap();                           // загрузить редактириемую картинку в QLabel для отображения
    void    setBolls();                             // создать шарики по углам картинки
    void    set_lTop(QPoint pn);                    // создать lTop по указанным координатам index=1
    void    set_rTop(QPoint pn);                    // создать rTop по указанным координатам index=2
    void    set_lBot(QPoint pn);                    // создать lBot по указанным координатам index=4
    void    set_rBot(QPoint pn);                    // создать rBot по указанным координатам index=3
    void    makeBoll(QavLabel &lb, int x, int y, int index);  // создать "прихватку" (общее "тело" предыдущих 4 функций)
    // index - номер "прихватки" 1..4
    void    ms_press(int x,int y,int i);            // нажатие на "прихватку"
    void    ms_up(int x,int y,int i);               // отжатие "прихватки"
    void    ms_move(int x,int y,int i);             // движение "прихватки"
    QPoint  bollMove(QavLabel &lb, int dx, int dy); // процесс движения "прихватки"
    void    distortion();                           // самое главное - процесс искажения картинки
    QPixmap getPixOfSize(QPixmap p, QSize sz);      // преобразовать pixmap p до размера sz
    QPixmap makePixForLabel(QPixmap p);             // создать pixmap для демонстрации на экране
    QPixmap getNewPix();                            // получить новый pixmap из исходного по установленному правилу
    void    getD();                                 // расчитать все значения смещений прихваток
    QSize   getNewPolygon();                        // расчитать размер для нового pixmap'а после деформации
    double  getNewX(int x, int y);                  // пересчет искаженных координат
    double  getNewY(int x, int y);
    double  minValue(double v1,double v2,
                     double v3, double v4);         // вернуть наименьшее значение из четырех
    double  maxValue(double v1,double v2,
                     double v3, double v4);         // вернуть наибольшее значение из четырех
    void    saveStep();                             // сохранить в стеке шаг трансформации
    QPoint  inFrame(QPoint pn);                     // уточнить и, при необходимости, вернуть координаты шарика в рамку

    void on_pushButton_clicked();
    void on_checkBox_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_checkBox_clicked(bool checked);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Qdeformation *ui;

signals:
    void pixFin(QPixmap p);                     // вернуть pixmap
};

#endif // QDEFORMATION_H
