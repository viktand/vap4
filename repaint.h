#ifndef QREPAINTPIXMAP_H
#define QREPAINTPIXMAP_H

#include <QDialog>

namespace Ui {
class QRepaintPixmap;
}

class QRepaintPixmap : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QPixmap pix READ getPixmap WRITE setPixmap) // свойство - трансформируемое изображение

private:
    QPixmap pix;

public:
    explicit QRepaintPixmap(QWidget *parent = 0);
    ~QRepaintPixmap();
    QPixmap  getPixmap()
    {
        return pix;
    }
    void setPixmap(QPixmap value)
    {
        pix=value;
        loadPicture();
    }

private slots:
    void loadPicture();                             // Загрузить картинку в форму
    QPixmap getPixOfSize(QPixmap p, QSize sz);      // преобразовать pixmap p до размера sz
    int getHue(int h);                              // получить значения оттенка в рамках допустимого диапазона
    int getSaturation(int s);                       // получить значения насыщенности или яркости в рамках допустимого диапазона
    void putPixel(int x,int y);                     // поставить точку в im по координатам x,y из tempPixmep
    void putPixelRGB(int x,int y);                  // поставить точку в im по координатам x,y из tempPixmep по RGB
    void resetSliders();                            // установить слайдеры в исходное положение
    void loadInVector();                            // загрузить картинку в вектор пикселов
    void setTransparentColor(int X, int Y);         // задать прозрачный цвет - точка на картинке
    bool campColor(QColor c1, QColor c2, int p);    // да, если цвета одинаковые или близкие на p%
    int  diffInts(int i1, int i2);                  // возвр. разницу двух чисел в процентах (0..255)
    void load();                                    // загрузка в вектор (сам цикл сканирования картинки)

    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_pushButton_2_clicked();
    void on_horizontalSlider_4_valueChanged(int value);
    void on_tabWidget_currentChanged(int index);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_pushButton_clicked();
    void on_horizontalSlider_7_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_dial_valueChanged(int value);

private:
    Ui::QRepaintPixmap
    *ui;

signals:
    void endRepaint(QPixmap px);                          // результат обработки
};

#endif // QREPAINTPIXMAP_H
