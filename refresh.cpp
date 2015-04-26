#include "refresh.h"
#include "ui_refresh.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

QSettings sett1("vap", "vap");

refresh::refresh(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::refresh)
{
    ui->setupUi(this);
    sett1.beginGroup("Version");
        ui->checkBox->setChecked(sett1.value("dontask", false).toBool());
    sett1.endGroup();
}

refresh::~refresh()
{
    delete ui;
}

void refresh::on_pushButton_2_clicked() // просто закрыть окно
{
    this->close();
}

void refresh::on_pushButton_clicked() // открыть страницу загрузки программы
{
    QDesktopServices::openUrl(QUrl("https://sites.google.com/site/viktandvap"));
    this->close();
}

void refresh::on_checkBox_clicked(bool checked)
{
    sett1.beginGroup("Version");
    sett1.setValue("dontask", checked);
    sett1.endGroup();
}

void refresh::clearCheck()
{
    ui->checkBox->setChecked(false);
    sett1.beginGroup("Version");
    sett1.setValue("dontask", false);
    sett1.endGroup();
}
