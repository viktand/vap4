#ifndef SETTING_H
#define SETTING_H

#include "ui_setting.h"

class setting : public QMainWindow, private Ui::setting
{
    Q_OBJECT
    
public:
    explicit setting(QWidget *parent = 0);
private slots:
    void on_checkBox_clicked(bool checked);
    QString GetNautilusVer();
    void check_intg();
};

#endif // SETTING_H
