#ifndef AVLABEL_H
#define AVLABEL_H

#include <QtGui/QMouseEvent>
#include <QLabel>


class QavLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int Imnum READ getImnum WRITE setImnum) // свойство - номер объекта (чтобы не вычислять)

    private:
        int Imnum;
    public:
        QavLabel(QWidget *parent = 0);
        ~QavLabel();
        int  getImnum()
        {
            return Imnum;
        }
        void setImnum(int value)
        {
            Imnum=value;
        }

    signals:
        void mouse_up(int x, int y, int i);
        void rclicked();
        void mouse_press(int x, int y, int i);
        void mouse_move(int x, int y, int i);


    protected:
        void mouseReleaseEvent(QMouseEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);


};
#endif // AVLABEL_H
