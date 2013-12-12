#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QDialog>

namespace Ui {
class TextEditor;
}

class TextEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = 0);
    ~TextEditor();

private:
    Ui::TextEditor *ui;

signals:
    void settext(QString text, QColor Backcolor, QColor Literscolor, QFont font, bool trans);

public slots:
    void loadtext(QString text, QColor Backcolor, QColor Literscolor, QFont font, bool trans);

private slots:
    void on_pushButton_4_clicked();
    void on_checkBox_clicked(bool checked);
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
};

#endif // TEXTEDITOR_H
