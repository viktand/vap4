#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    QFont font("Ubuntu");
    font.setPointSize(10);
    ui->textEdit->setFont(font);
}

about::~about()
{
    delete ui;
}

void about::on_pushButton_clicked()
{
    this->close();
}
