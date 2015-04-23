#include "avbutton.h"

avButton::avButton(QWidget *parent): QPushButton(parent){}

avButton::~avButton(){}

void avButton::wheelEvent(QWheelEvent *event)
// вращение колеса (1) - вверх (-1) - вниз
{
    if(event->delta()>0)
    {
        emit whellUp();
    }
    else
    {
        emit whellDown();
    }
}

