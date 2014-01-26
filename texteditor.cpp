#include "texteditor.h"
#include "ui_texteditor.h"

#include <QColorDialog>
#include <QFontDialog>


QColor  BcColor;
QColor  LtColor;
QFont   txfnt("Ubuntu");
bool    trBack=true;
int     alg=0;

TextEditor::TextEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    BcColor=ui->textEdit->textBackgroundColor();
    LtColor=ui->textEdit->textColor();
    txfnt=ui->textEdit->font();
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::set_alig(int a)
{
    alg=a;
    switch (a)
    {
        case 0:
            ui->textEdit->setAlignment(Qt::AlignLeft);
            break;
        case 1:
            ui->textEdit->setAlignment(Qt::AlignCenter);
            break;
        case 2:
            ui->textEdit->setAlignment(Qt::AlignRight);
            break;
        default:
            ui->textEdit->setAlignment(Qt::AlignLeft);
    }
}

void TextEditor::loadtext(QString text, QColor Backcolor, QColor Literscolor, QFont font, bool trans, int alg)
// загрузить текст в редактор
{
    if(!trans)ui->textEdit->setTextBackgroundColor(Backcolor);
    else ui->textEdit->setTextBackgroundColor(Qt::white);
    BcColor=Backcolor;
    ui->textEdit->setTextColor(Literscolor);
    LtColor=Literscolor;
    ui->textEdit->setFont(font);
    txfnt=font;
    ui->checkBox->setChecked(trans);
    ui->textEdit->clear();
    set_alig(alg);
    ui->textEdit->append(text);
}

void TextEditor::on_pushButton_4_clicked()
// Ok
{
    emit settext(ui->textEdit->toPlainText(), BcColor, LtColor, txfnt, trBack, alg);
    this->close();
}

void TextEditor::on_checkBox_clicked(bool checked)
// check transparent
{
    trBack=checked;
    ui->pushButton_2->setEnabled(!(checked));
}

void TextEditor::refresh()
{
    QString s=ui->textEdit->toPlainText();
    ui->textEdit->clear();
    set_alig(alg);
    ui->textEdit->append(s);
}

void TextEditor::on_pushButton_3_clicked()
// liters color
{
    const QColor color = QColorDialog::getColor(Qt::white, this, tr("Select background color"));
    if (color.isValid())
    {
        LtColor=color;
        ui->textEdit->setTextColor(color);
        refresh();
    }
}

void TextEditor::on_pushButton_2_clicked()
// back color
{
    const QColor color = QColorDialog::getColor(Qt::white, this, tr("Select background color"));
    if (color.isValid())
    {
        BcColor=color;
        ui->textEdit->setTextBackgroundColor(color);
        refresh();
    }
}

void TextEditor::on_pushButton_clicked()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, txfnt,this, tr("Select caption font"));
    if (ok)
    {
        txfnt=font;
        ui->textEdit->setFont(font);
    }
}

void TextEditor::on_pushButton_5_clicked() // exit without Ok
{
    this->close();
}



void TextEditor::on_pushButton_6_clicked()
{
    set_alig(0);
    refresh();
}

void TextEditor::on_pushButton_7_clicked()
{
    set_alig(1);
    refresh();
}

void TextEditor::on_pushButton_8_clicked()
{
    set_alig(2);
    refresh();
}
