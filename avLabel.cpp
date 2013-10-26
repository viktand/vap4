#include "avLabel.h"

//#include <iostream>
//using namespace std;

QPixmap *mem_pix;

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

        if(e->button() == Qt::RightButton) emit rclicked();                          // правый клик

}

void QavLabel::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit mouse_press(e->globalX(), e->globalY(), this->getImnum());               // нажатие
    }
}

void QavLabel::mouseMoveEvent(QMouseEvent *e)
{
    emit mouse_move(e->globalX(), e->globalY(), this->getImnum());                    // движение
}


