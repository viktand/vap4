#include "dialog_paper.h"
#include "ui_dialog_paper.h"
#include <QSettings>

int ind;

Dialog_paper::Dialog_paper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_paper)
{
    ui->setupUi(this);
}

Dialog_paper::~Dialog_paper()
{
    delete ui;
}

void Dialog_paper::load_data(QString paper_name, int w, int h, int index)
{
    ui->lineEdit->setText(paper_name);
    ui->spinBox->setValue(w);
    ui->spinBox_2->setValue(h);
    ind=index;
}

void Dialog_paper::on_buttonBox_clicked() // Ok
{
    QSettings sett("vap", "vap");
    if (ui->radioButton->isChecked())
    {
        sett.beginGroup("Paper sizes");
            sett.beginGroup(QString::number(ind));
                sett.setValue("width", ui->spinBox->value());
                sett.setValue("heigth", ui->spinBox_2->value());
                sett.setValue("name", ui->lineEdit->text());
            sett.endGroup();
        sett.endGroup();
    }
    else
    {
        sett.beginGroup("Paper sizes");
            sett.remove(QString::number(ind));
        sett.endGroup();
    }
    emit close_form();
}
