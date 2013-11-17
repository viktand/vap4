#include "avLabel.h"

//#include <iostream>
//using namespace std;

QPixmap *mem_pix;
int bt=0;

QavLabel::QavLabel(QWidget *parent) : QLabel(parent)
{
    mem_pix=new QPixmap(1,1);
}


QavLabel::~QavLabel()
{
   //cout << "kill preview" << endl;
}

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


