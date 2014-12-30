#include "tofile.h"
#include "ui_tofile.h"
#include <QFileDialog>

QString frm;       // формат
double  kSheet=1;  // соотношение сторон листа
QString fName;     // имя файла
QSize   fsz;       // размер картинки
QString path="/";  // путь к файлу

QFileDialog *fDial;// диалог выбора имени файла и пути к нему

toFile::toFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toFile)
{
    ui->setupUi(this);
}

toFile::~toFile()
{
    delete ui;
}

void toFile::on_radioButton_clicked()
{
    frm="jpg";
    ui->horizontalSlider->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->checkBox_2->setEnabled(false);
}

void toFile::on_radioButton_2_clicked()
{
    frm="png";
    ui->horizontalSlider->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
}

void toFile::on_radioButton_3_clicked()
{
    frm="bmp";
    ui->horizontalSlider->setEnabled(false);
    ui->pushButton->setEnabled(true);
    ui->checkBox_2->setEnabled(false);
}

void toFile::on_checkBox_clicked(bool checked)
{
    ui->spinBox_2->setEnabled(!checked);
    if(checked)ui->spinBox_2->setValue(ui->spinBox->value()*kSheet);
}

void toFile::setSheetSize(QSize sz)
{
    kSheet=double(sz.height())/double(sz.width());
    ui->spinBox_2->setValue(ui->spinBox->value()*kSheet);
}

void toFile::on_pushButton_clicked() // сохранить в файл
{
    fName = QFileDialog::getSaveFileName(this, tr("Save to file"),
                               path,tr("Images (*.png *.bmp *.jpg)"));
    QString s=fName.right(4);
    if(s[0]=='.')fName=fName.left(fName.length()-4);
    fName.append(".");
    fName.append(frm);
    bool t=ui->checkBox_2->isChecked() && ui->checkBox_2->isEnabled();
    emit end(fName,frm,ui->horizontalSlider->value(),QSize(ui->spinBox->value(),ui->spinBox_2->value()),t);
    close();
}

void toFile::on_spinBox_valueChanged(int arg1)
{
    ui->spinBox_2->setValue(arg1*kSheet);
}

void toFile::setPath(QString pt)
{
    path=pt;
}
