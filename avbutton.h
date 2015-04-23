#ifndef AVBUTTON_H
#define AVBUTTON_H

#include <QWheelEvent>
#include <QPushButton>

class avButton : public QPushButton
{
    Q_OBJECT
public:
    explicit avButton(QWidget *parent = 0);
    ~avButton();

protected:
    void wheelEvent(QWheelEvent *event);

signals:
    void whellUp();         // сигналы вращения колесика
    void whellDown();
};

#endif // AVBUTTON_H
