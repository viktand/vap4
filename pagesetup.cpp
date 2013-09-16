#include "pagesetup.h"
#include <QDesktopWidget>
#include <mainwindow.h>



PageSetup::PageSetup(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    comboBox->addItem("A4");
    comboBox->addItem("A5");
    comboBox->addItem("10x15");
    spinBox->setValue(top_m);
    checkBox->setChecked(all_rot);
 }

PageSetup::~PageSetup()
{
    delete ui2;
}

void PageSetup::on_radioButton_2_clicked()
{
     label_4->setGeometry(100,215, 75, 50);
}

void PageSetup::on_radioButton_clicked()
{
     label_4->setGeometry(110,200, 50, 75);
}

void PageSetup::on_pushButton_2_clicked() // Ok
{
   top_m=spinBox->value();
   left_m=spinBox_2->value();
   right_m=spinBox_3->value();
   bottom_m=spinBox_4->value();
   if(radioButton->isChecked()) ornl=1; else ornl=0;
   emit end_set();
   this->close();
}

void PageSetup::load_data()
{
   spinBox->setValue(top_m);
   spinBox_2->setValue(left_m);
   spinBox_3->setValue(right_m);
   spinBox_4->setValue(bottom_m);
   if (ornl==0)
   {
       on_radioButton_2_clicked();
       radioButton_2->setChecked(true);
   }
   else
   {
       on_radioButton_clicked();
       radioButton->setChecked(true);
   }
   set_all();
}

void PageSetup::set_all()
{
    checkBox->setChecked(all_rot);
}

void PageSetup::on_checkBox_clicked(bool checked)
{
    emit set_all_em(checked);
}
