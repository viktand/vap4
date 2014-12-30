#ifndef TOFILE_H
#define TOFILE_H

#include <QDialog>

namespace Ui {
class toFile;
}

class toFile : public QDialog
{
    Q_OBJECT

public:
    explicit toFile(QWidget *parent = 0);
    ~toFile();

private:
    Ui::toFile *ui;

signals:
    void end(QString fName,
             QString form,
             int quality, QSize sz, bool trns); // сигнал завершения настроек
private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_checkBox_clicked(bool checked);
    void on_pushButton_clicked();
    void on_spinBox_valueChanged(int arg1);

public slots:
    // Эти слоты следует обязательно вызвать при открытии для правильной настройки диалога!!!
    // Без этого будут не самые лучшие значения по умолчанию - квадратный рисунок и корень файловой системы
    void setSheetSize(QSize sz);    // указать размер текущего листа
    void setPath(QString pt);       // указать текущий каталог для сохранения файла
};

#endif // TOFILE_H
