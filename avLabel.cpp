#include "avLabel.h"

QPixmap *mem_pix;
int bt=0;

QavLabel::QavLabel(QWidget *parent) : QLabel(parent)
{
    mem_pix=new QPixmap(1,1);
}

QavLabel::~QavLabel(){}

void QavLabel::mouseReleaseEvent(QMouseEvent *e)
{

        if(e->button() == Qt::LeftButton)
        {
            emit mouse_up(e->globalX(), e->globalY(), this->getImnum());             // левый клик
        }

        if(e->button() == Qt::RightButton)
        {
            emit rclicked(e->globalX(), e->globalY(), this->getImnum());             // правый клик
        }
    bt=0;
}

void QavLabel::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit mouse_press(e->globalX(), e->globalY(), this->getImnum());               // нажатие
        bt=1;
    }
}

void QavLabel::mouseMoveEvent(QMouseEvent *e)
{
    if(bt==1)
    {
        emit mouse_move(e->globalX(), e->globalY(), this->getImnum());                    // движение
    }
}


void QavLabel::wheelEvent(QWheelEvent *event)  // вращение колеса (1) - вверх (-1) - вниз
{
    int i=event->delta();
    if(i>0)
    {
        i=1;
    }
    else
    {
        i=-1;
    }
    emit mouse_wheel(i, Imnum);
}

