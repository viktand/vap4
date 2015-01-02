/*
 Класс, создающий форму для управление освещением, т.е. подгружаемая картинка подвергается обработке
 в виде создания различных градиентов яркости.
*/

#ifndef LIGHTER_H
#define LIGHTER_H

#include <QDialog>

namespace Ui {
class lighter;
}

class lighter : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QPixmap pix READ getPixmap WRITE setPixmap) // свойство - трансформируемое изображение

private:
    QPixmap pix;

public:
    explicit lighter(QWidget *parent = 0);
    ~lighter();
    QPixmap  getPixmap()
    {
        return pix;
    }
    void setPixmap(QPixmap value)
    {
        pix=value;
        loadPicture();
    }

// тут много слотов с похожим описанием, потому что определяются функции для однотипных расчетов,
// из которых собирается основной слот.

private slots:
    void loadPicture();                             // Загрузить картинку в форму
    void reset();                                   // сбросить "прицелы"
    QPixmap getPixOfSize(QPixmap p, QSize sz);      // преобразовать pixmap p до размера sz
    void loadInVector();                            // загрузить картинку в вектор пикселов
    void load();                                    // загрузка в вектор (сам цикл сканирования картинки)
    void setPoint(int x, int y);                    // установить точку - вершину треугольника луча света
    void get1point(int x, int y);                   // получить первую точку вектора луча света
    void get2point(int x, int y);                   // получить вторую точку вектора луча света
    void get3point(int x, int y);                   // получить третью точку вектора луча света и выполнить обработку
    void getTriangle();                             // расчитать треугольник луча света
    void lightRay();                                // наложение луча света
    bool inRay(QPoint xy);                          // определить, принадлежность точки лучу, точнее треугольнику с высотой, заданной опорными точками
    int  getCrossPoint(QPoint p1, QPoint p2, int x);// расчитать точку пересечения прямой, проходящей через р1, р2 с осью x=x
    QPoint funPoint(QPoint pls);                    // расчитать точку пересечения стороны луча с краем экрана
    QPoint funPoint2(QPoint pls);                   // расчитать точку пересечения стороны луча с краем картинки
    int to255(int i);                               // привести i к диапазону 0..255
    double getDist(QPoint p1,QPoint ps);            // определить расстояние от точки p1 до ps
    void showPict();                                // показать картинку из рабочего массива
    void reput1(int x, int y);                      // переустановка лампы
    void reput2(int x, int y);                      // переустановка pl2
    void reput3(int x, int y);                      // переустановка pl3
    void takeLamp();                                // взять лампу, чтобы переставить
    void takeP1();                                  // взять первую точку стороны треугольника
    void takeP2();                                  // взять вторую точку стороны треугольника
    bool side(QPoint ps);                           // определить с какой стороны от основания треугольника находится точка ps
    void rayToVector();                             // сохранить текущий луч в массиве
    void resultToMain();                            // отправить результат в основную форму
    void setFlash(int x,int y);                     // поставить вспышку в указанную точку
    void resetSliders();                            // установка слайдеров в начальное состояние
    void saveFlash();                               // сохранить текущую вспышку в массиве
    void putBollPoint1(int x, int y);               // установить 1 точку шара
    void putBollPoint2(int x, int y);               // установить 2 точку шара
    void paintBoll();                               // нарисовать шар
    int  sgn(double x);                             // знак числа x
    void saveBoll();                                // сохранить текущий шар в массив
    void reputLampBoll(int x,int y);                // переустановить лампу шара
    void reputCenBoll(int x,int y);                 // переустановить направление к центру шара
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_pushButton_5_clicked();
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_pushButton_6_clicked();

private:
    Ui::lighter *ui;

signals:
    void endLighter(QPixmap px);                          // результат обработки
};

#endif // LIGHTER_H
