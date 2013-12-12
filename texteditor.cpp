#include "texteditor.h"
#include "ui_texteditor.h"

#include <QColorDialog>
#include <QFontDialog>


QColor  BcColor;
QColor  LtColor;
QFont   txfnt;
bool    trBack=true;

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

void TextEditor::loadtext(QString text, QColor Backcolor, QColor Literscolor, QFont font, bool trans)
// загрузить текст в редактор
{
    ui->textEdit->setText(text);
    ui->textEdit->setTextBackgroundColor(Backcolor);
    BcColor=Backcolor;
    ui->textEdit->setTextColor(Literscolor);
    LtColor=LtColor;
    ui->textEdit->setFont(font);
    txfnt=font;
    ui->checkBox->setChecked(trans);
}

void TextEditor::on_pushButton_4_clicked()
// Ok
{
    emit settext(ui->textEdit->toPlainText(), BcColor, LtColor, txfnt, trBack);
    this->close();
}

void TextEditor::on_checkBox_clicked(bool checked)
// check transparent
{
    trBack=checked;
    ui->pushButton_2->setEnabled(!(checked));
}

void TextEditor::on_pushButton_3_clicked()
// liters color
{
    const QColor color = QColorDialog::getColor(Qt::white, this, tr("Select background color"));
    if (color.isValid())
    {
        LtColor=color;
        ui->textEdit->setTextColor(color);
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

void TextEditor::on_pushButton_5_clicked()
{
    this->close();
}
