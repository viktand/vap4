#include "captioneditor.h"
#include "ui_captioneditor.h"
#include "mainwindow.h"

#include <QColorDialog>
#include <QFontDialog>
#include <QFontMetrics>
#include <QDesktopWidget>

QColor  b_cl;       // цвет фона
QColor  f_cl;       // цвет букв
QFont   fnt;        // caption font

void captioneditor::on_buttonBox_accepted() // сигнaл завершения настроек
{
    emit end_edit(ui->lineEdit->text(), f_cl, b_cl,
            fnt, ui->checkBox->isChecked(),
                 ui->checkBox_3->isChecked());
}


captioneditor::captioneditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::captioneditor)
{
    ui->setupUi(this);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

captioneditor::~captioneditor()
{
    delete ui;
}

void captioneditor::load_caption()
{
    ui->lineEdit->setText(caption);
    ui->checkBox->setChecked(show_cap);
    font_set(font_cpt);
    back_color_set(back_cl);
    font_color_set(font_cl);
    ui->checkBox_3->setChecked(trans);
    on_checkBox_3_clicked(trans);
}

void captioneditor::on_pushButton_clicked()
{
    const QColor color = QColorDialog::getColor(Qt::white, this, tr("Select background color"));
    if (color.isValid())back_color_set(color);
}

void captioneditor::back_color_set(QColor cl)
{
    QPalette p = ui->lineEdit->palette();
    p.setColor(QPalette::Base, cl);
    ui->lineEdit->setPalette(p);
    ui->lineEdit->setFrame(false);
    b_cl=cl;
}

void captioneditor::on_pushButton_2_clicked()
{
    const QColor color = QColorDialog::getColor(Qt::black, this, tr("Select font color"));
    if (color.isValid())font_color_set(color);
}

void captioneditor::font_color_set(QColor cl)
{
    QPalette p = ui->lineEdit->palette();
    p.setColor(QPalette::Text, cl);
    ui->lineEdit->setPalette(p);
    ui->lineEdit->setFrame(false);
    f_cl=cl;
}


void captioneditor::on_pushButton_3_clicked()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, QFont(ui->lineEdit->text()),this, tr("Select caption font"));
    if (ok)font_set(font);
 }

void captioneditor::font_set(QFont fn)
{
    ui->lineEdit->setFont(fn);
    QRect rc=ui->lineEdit->geometry();
    QFontMetrics m(fn);
    rc.setHeight(m.height()+2);
    shift_widgets(m.height());
    ui->lineEdit->setGeometry(rc);
    fnt=fn;
}

void captioneditor::shift_widgets(int sh)
{
    QRect rc=ui->groupBox->geometry();
    int i=rc.height();
    rc.setTop(70+sh-23);
    rc.setHeight(i);
    ui->groupBox->setGeometry(rc);
    rc=this->geometry();
    i=rc.top();
    int l=rc.left();
    rc.setHeight(254+sh-23);
    rc.setTop(i);
    rc.setLeft(l);
    this->setGeometry(rc);
}



void captioneditor::on_checkBox_3_clicked(bool checked)
{
    ui->pushButton->setEnabled(!checked);
    QPalette p = ui->lineEdit->palette();
    if (checked) p.setColor(QPalette::Base, Qt::transparent);
        else p.setColor(QPalette::Base, b_cl);
    ui->lineEdit->setPalette(p);
    ui->lineEdit->setFrame(false);
}
