#include "qdeformation.h"
#include "ui_qdeformation.h"

Qdeformation::Qdeformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Qdeformation)
{
    ui->setupUi(this);
}

Qdeformation::~Qdeformation()
{
    delete ui;
}
