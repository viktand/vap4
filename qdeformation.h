#ifndef QDEFORMATION_H
#define QDEFORMATION_H

#include <QDialog>

namespace Ui {
class Qdeformation;
}

class Qdeformation : public QDialog
{
    Q_OBJECT

public:
    explicit Qdeformation(QWidget *parent = 0);
    ~Qdeformation();

private:
    Ui::Qdeformation *ui;
};

#endif // QDEFORMATION_H
