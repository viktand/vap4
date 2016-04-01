#include "pagesetup.h"
#include <QDesktopWidget>
#include <mainwindow.h>
#include <QPrinterInfo>
#include <QList>
#include <QProcess>
#include "integro.h"
#include "dialog_paper.h"
#include <QSettings>
#include <vector>
#include <QMessageBox>
#include <refresh.h>


#include <iostream>
using namespace std;


QNetworkAccessManager *manager1;
bool clse=true;
bool flag_ret=false;
QSize mysz;
Dialog_paper *dp;
int pw;
int ph;
QSettings setts("vap", "vap");
QString nms[8];             // список имен бумаг
std::vector<int> p_w;       // список размеров
std::vector<int> p_h;
struct paps{                // описание размеров листов бумаги
    QString name;           // имя
    int     index;          // условный индекс
    int     W;              // ширина листа, mm
    int     H;              // высота листа, mm
};

std::vector<paps> nmsp;     // список
bool auto_orn=true;         // автоматический выбор ориентации бумаги
QString lngv;               // язык программы
bool ribbon=true;           // вид интерфейса
int result=0;               // код, возвращаемый при закрытии
refresh *rf;                // форма проверки обновлений

void PageSetup::load_combobox3(int r)
{
    load_combobox(r==1);
}

void PageSetup::load_combobox(bool res)
{
    if (res)result=1;
    load_combobox2();
    comboBox_3->clear();
    comboBox_3->addItem("OnlyOne");         // 0
    comboBox_3->addItem("Lower");           // 1
    comboBox_3->addItem("Middle");          // 2
    comboBox_3->addItem("Manual");          // 3
    comboBox_3->addItem("Envelope");        // 4
    comboBox_3->addItem("EnvelopeManual");	// 5
    comboBox_3->addItem("Auto");            // 6
    comboBox_3->addItem("Tractor");         // 7
    comboBox_3->addItem("SmallFormat");     // 8
    comboBox_3->addItem("LargeFormat");     // 9
    comboBox_3->addItem("LargeCapacity");	// 10
    comboBox_3->addItem("Cassette");        // 11
    comboBox_3->addItem("FormSource");      // 12
    comboBox_3->addItem("MaxPageSource");   // 13
}


void PageSetup::load_combobox2() // загрузка списка "любимых" размеров бумаги
{
    paps p;
    comboBox->clear();
    nmsp.clear();
    comboBox->addItem("A4: 210 x 297 mm");
    p.name="A4: 210 x 297 mm"; p.index=0; p.W=210; p.H=297;
    nmsp.push_back(p);
    setts.beginGroup("Paper sizes");
    bool go=true;
    int  i=0;
    while(go)
    {
        p.name=setts.value("name"+QString::number(i), "").toString();
        p.index=setts.value("index"+QString::number(i), -1).toInt();
        p.W=setts.value("W"+QString::number(i), -1).toInt();
        p.H=setts.value("H"+QString::number(i++), -1).toInt();
        if (p.index>0){
            comboBox->addItem(p.name);
            nmsp.push_back(p);
        }
        else  go=false;
    }
    setts.endGroup();
}

PageSetup::PageSetup(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    mysz.setWidth(210);
    mysz.setHeight(297);
    tabWidget->setCurrentIndex(0);
    checkBox->setChecked(all_rot);
    // список принтеров
    QString l;
    int j=0;
    l.append(get_run("lpstat -a"));
    for (int i=0; i<l.count(); i++)
    {
       if (l.mid(i, 1)=="\n" || i==l.count()-1)
       {
           QString st =l.mid(j, i-j);
           st=st.mid(0,st.indexOf(" "));
           comboBox_2->addItem(st);
           j=i+1;
       }
    }
    // принтер по умолчанию
    l.clear();
    l.append(get_run("lpstat -d"));
    for(int i=0;i<comboBox_2->count();i++){
        if(l.indexOf(comboBox_2->itemText(i))>-1){
            label_8->setText(comboBox_2->itemText(i));
        }
    }
    flag_ret=true;
    checkBox_3->setChecked(nautilus_check());
    checkBox_4->setChecked(dolphin_check());
    checkBox_5->setChecked(marlin_check());
    checkBox_6->setChecked(nemo_check());
    checkBox_7->setChecked(gnome_check());
    checkBox_8->setChecked(caja_check());
    checkBox_10->setChecked(thunar_check());
    checkBox_11->setChecked(pcman_check());
    checkBox_2->setChecked(spacefm_check());
    flag_ret=false;
 }


QString PageSetup::get_run(QString s)
{
    QProcess proc;
    proc.start(s);
    proc.waitForFinished(-1);
    QByteArray btar;
    btar = proc.readAll();
    QString ver;
    ver.append(btar);
    return ver;
}

PageSetup::~PageSetup()
{
    emit end_set(result);
    delete ui2;

}

void PageSetup::reset_result()
{
    result=0;
}

void PageSetup::closeEvent(QCloseEvent *event)
{
    emit end_set(result);
    event->~QEvent();
}

void PageSetup::on_radioButton_2_clicked()
{
     label_4->setGeometry(130,185, 75, 50);
}

void PageSetup::on_radioButton_clicked()
{
     label_4->setGeometry(150,170, 50, 75);
}

void PageSetup::load_data()
{
   clse=true;
   doubleSpinBox_2->setValue(font_scl);
   ph=paper_h;
   pw=paper_w;
   if (ph<pw)
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
   QString s;
   s.append(p_name);
   for (int i=0; i<comboBox_2->count(); i++)
       if (comboBox_2->itemText(i)==p_name)
       {
           comboBox_2->setCurrentIndex(i);
       }
   load_combobox(false);
   doubleSpinBox->setValue(h_ofsett);
   comboBox_3->setCurrentIndex(pap_sor);
   clse=false; // разрешить изменение настроек
   comboBox->setCurrentIndex(get_index(get_real_index(pap_name)));
   setts.beginGroup("Settings");
   spinBox_7->setValue(setts.value("font", 0).toInt());
   lngv=setts.value("lng", "Auto").toString();
   auto_orn=setts.value("autoOrn", true).toBool();
   // Замута для совместимости с Qt 4.8 вместо comboBox_4->setCurrentText(lngv);
   for(int i=0; i<comboBox_4->count(); i++)if(comboBox_4->itemText(i)==lngv)comboBox_4->setCurrentIndex(i);
   setts.endGroup();
}

QString PageSetup::get_real_index(int pap)
{
    if(pap==0) return "@";
    setts.beginGroup("Paper sizes");
    int  i=0,j=0;
    while(j>-1)
    {
        j=setts.value("index"+QString::number(i), -1).toInt();
        if(j>-1){
            if(j==pap) {
                QString s=setts.value("name"+QString::number(i), "").toString();
                setts.endGroup();
                return s;
            }
        }
        i++;
    }
    setts.endGroup();
    return "";
}

int PageSetup::get_index(QString s)
{
    if(s=="@") return 0;
    for(int i=0; i<comboBox->count(); i++) {
        if(s==comboBox->itemText(i)){
            return i;
        }
    }
    return -1;
}

void PageSetup::set_all()
{
    checkBox->setChecked(all_rot);
}

void PageSetup::on_checkBox_clicked(bool checked)
{
    emit set_all_em(checked);
}

void PageSetup::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    if (clse) return;
    p_name=arg1;
}

void PageSetup::on_pushButton_clicked() // Ok
{
    on_pushButton_4_clicked();
    on_pushButton_3_clicked();
}


void PageSetup::on_pushButton_4_clicked() // apply
{
    if(radioButton->isChecked()) emit end_set(3); else emit end_set(4);
    paper_h=ph;
    paper_w=pw;
    list_n=nmsp[comboBox->currentIndex()].name;
    setts.beginGroup("Settings");
        setts.setValue("font", spinBox_7->value());
        setts.setValue("lng", lngv);
        setts.setValue("autoOrn", auto_orn);
        setts.setValue("heigth", ph);
        setts.setValue("width", pw);
        setts.setValue("printer_name", p_name);
    setts.endGroup();
    emit end_set(2);
    result=0;
}

void PageSetup::on_pushButton_3_clicked() // close
{
    this->close();
}

void PageSetup::on_checkBox_3_clicked(bool checked) // интеграция в наутилус
{
    if (flag_ret) return;
    nautilus_set(checked);
}

void PageSetup::on_checkBox_4_clicked(bool checked)
{
    if (flag_ret) return;
    dolphin_set(checked);
}

void PageSetup::on_checkBox_5_clicked(bool checked)
{
    if (flag_ret) return;
    marlin_set(checked);
}

void PageSetup::on_checkBox_6_clicked(bool checked)
{
    if (flag_ret) return;
    nemo_set(checked);
}

void PageSetup::on_checkBox_7_clicked(bool checked)
{
    if (flag_ret) return;
    gnome_set(checked);
}


void PageSetup::on_checkBox_8_clicked(bool checked)
{
    if (flag_ret) return;
    caja_set(checked);
}

void PageSetup::on_pushButton_2_clicked() // Добавление нового варианта размера бумаги в список
{
    if (dp==0)
    {
        dp=new Dialog_paper(this);
        dp->setFont(checkBox->font());
        connect(dp, SIGNAL(close_form(int)), this, SLOT(load_combobox3(int)));
    }
    dp->load_lists();
    dp->show();
}

void PageSetup::on_comboBox_currentIndexChanged(int index)
{
    if (clse) return;
    radioButton->setChecked(true);
    label_4->setGeometry(110,170, 50, 75);
    pw=nmsp[index].W;
    ph=nmsp[index].H;
    pap_name=nmsp[index].index;
}

void PageSetup::on_comboBox_3_currentIndexChanged(int index)
{
    if (clse) return;
    pap_sor=index;
}

void PageSetup::on_doubleSpinBox_valueChanged(double arg1)
{
    if (clse) return;
    h_ofsett=arg1;
}

void PageSetup::on_radioButton_toggled()
{
    if (clse) return;
    swap(pw, ph);
}

void PageSetup::on_doubleSpinBox_2_valueChanged(double arg1)
{
    font_scl=arg1;
}

void PageSetup::on_checkBox_10_clicked(bool checked)
{
    if (flag_ret) return;
    thunar_set(checked);
}

void PageSetup::on_checkBox_13_clicked(bool checked)
{
    if(flag_ret)return;
    setts.beginGroup("Settings");
    setts.setValue("path", checked);
    setts.endGroup();
}

void PageSetup::set_path(bool ch)
{
    checkBox_13->setChecked(ch);
}


void PageSetup::on_checkBox_11_clicked(bool checked)
{
    if (flag_ret) return;
    pcman_set(checked);
}

void PageSetup::on_comboBox_4_activated(const QString &arg1)
{
    // Установка языка интерфейса программы
    lngv=arg1;
    QMessageBox msgBox;
    QString ms;
    ms.append(tr("The interface will be changed at the next start program! \n"));
    msgBox.setText(ms);
    msgBox.exec();
}

void PageSetup::on_checkBox_14_clicked(bool checked)
{
    auto_orn=checked;
}


void PageSetup::on_checkBox_2_clicked(bool checked)
{
    if(flag_ret)return;
    spacefm_set(checked);
}

void PageSetup::on_checkBox_9_clicked(bool checked) // Управление автоматической проверкой обновлений
{
    setts.beginGroup("Version");
        setts.setValue("check", checked);
    setts.endGroup();
}

void PageSetup::on_pushButton_5_clicked() // проверить обновление
{
    if(manager1==0){
        manager1 = new QNetworkAccessManager(this);
        connect(manager1, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    }
    manager1->get(QNetworkRequest(QUrl("http://qvap.ru/version.html")));
}

void PageSetup::replyFinished(QNetworkReply *reply)
{
    if(!(reply->error()))
        {
            QString ver=QString::fromStdString(reply->readAll().toStdString());
            int i=ver.indexOf(":");
            int index=ver.left(i).toInt();
            ver=ver.right(ver.length()-i-1);
            if(vapIndex<index){
                if(vapIndex!=index){
                    if (rf==0){
                        rf=new refresh(this);
                    }
                    rf->show();
                }
            } else {
                QMessageBox msgBox;
                msgBox.setText(tr("No updates."));
                msgBox.exec();
            }
        }
    reply->deleteLater();
}

