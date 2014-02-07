#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QDialog>

namespace Ui {
class OpenFileDialog;
}

class OpenFileDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenFileDialog(QWidget *parent = 0);
    ~OpenFileDialog();
    
private:
    Ui::OpenFileDialog *ui;

private slots:
    void load_files(QString path);       //загрузить (показать) файлы из папки path
};

#endif // OPENFILEDIALOG_H
