#include "pagesetup.h"
#include <QDesktopWidget>
#include <mainwindow.h>
#include <QPrinterInfo>
//#include <QtGui/QPrinterInfo>
#include <QList>
#include <QProcess>
#include "integro.h"
#include "dialog_paper.h"
#include <QSettings>
#include <vector>

bool clse=true;
bool flag_ret=false;
QSize mysz;
Dialog_paper *dp;
int pw=210;
int ph=297;
QSettings setts("vap", "vap");
QString nms[8];             // список имен бумаг
std::vector<int> p_w;  // список размеров
std::vector<int> p_h;

void set_paper(int p)
{
    switch (p)
    {
    case 0:
        paper_h=297; paper_w=210; break;
    case 1:
        paper_h=210; paper_w=148; break;
    case 2:
        paper_h=148; paper_w=105; break;
    case 3:
        paper_h=127; paper_w=90; break;
    case 4:
        paper_h=152; paper_w=102; break;
    case 5:
        paper_h=178; paper_w=127; break;
    case 6:
        paper_h=203; paper_w=152; break;
    case 7:
        paper_h=420; paper_w=297; break;
    case 8:
        paper_h=mysz.height(); paper_w=mysz.width();
    }
}

void PageSetup::load_combobox()
{
    int i=0;
    bool go=true;
    p_w.clear(); p_h.clear();
    p_w.push_back(210); p_h.push_back(297);
    p_w.push_back(148); p_h.push_back(210);
    p_w.push_back(105); p_h.push_back(148);
    p_w.push_back(90); p_h.push_back(127);
    p_w.push_back(102); p_h.push_back(152);
    p_w.push_back(127); p_h.push_back(178);
    p_w.push_back(152); p_h.push_back(203);
    p_w.push_back(297); p_h.push_back(420);
    p_w.push_back(50); p_h.push_back(50);
    QString n;
    comboBox->clear();
    while(go)
    {
    setts.beginGroup("Paper sizes");
        setts.beginGroup(QString::number(i));
        n.clear();
        n.append(setts.value("name", "").toString());
        if (n.length()!=0)
        {
            comboBox->addItem(n);
            p_w[i]=setts.value("width", 0).toInt();
            p_h[i]=setts.value("heigth", 0).toInt();
            p_w.push_back(i);
            p_h.push_back(i);
        }
        else if (i<8) comboBox->addItem(nms[i]);
        setts.endGroup();
    setts.endGroup();
    go=!(i>7 && n.length()==0);
    i++;
    }
    comboBox->addItem("My size (make new size)");
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

PageSetup::PageSetup(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    nms[0].append("A4 - 210 x 297 mm");         // 0
    nms[1].append("A5 - 148 x 210 mm");         // 9
    nms[2].append("A6 - 105 x 148 mm");         // 10
    nms[3].append("Photo 9x13 (90x127 mm)");    // 90x127
    nms[4].append("Photo 10x15 (102x152 mm)");  // 102x152
    nms[5].append("Photo 13x18 (127x178 mm)");  // 127x178
    nms[6].append("Photo 15x20 (152x203 mm)");  // 152x203
    nms[7].append("A3 - 297 x 420 mm");         // 8
    mysz.setWidth(210);
    mysz.setHeight(297);
    spinBox_5->setValue(210);
    spinBox_6->setValue(297);
    checkBox->setChecked(all_rot);
    // список принтеров
    QString l;
    int j=0;
    bool f1=false;
    l.append(get_run("lpstat -v"));
    for (int i=0; i<l.count(); i++)
    {
       if (l.mid(i,6)=="device")
       {
           f1=true;
           j=i+11;
       }
       if (f1 && l.mid(i, 1)==":")
       {
           comboBox_2->addItem(l.mid(j, i-j));
           f1=false;
       }
    }
    // принтер по умолчанию
    l.clear();
    l.append(get_run("lpstat -d"));
    label_8->setText(l.mid(28, l.length()-29));
    flag_ret=true;
    checkBox_3->setChecked(nautilus_check());
    checkBox_4->setChecked(dolphin_check());
    checkBox_5->setChecked(marlin_check());
    checkBox_6->setChecked(nemo_check());
    checkBox_7->setChecked(gnome_check());
    checkBox_8->setChecked(caja_check());
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

void PageSetup::load_data()
{
   clse=true;
   checkBox_2->setChecked(print_color);
   checkBox_9->setChecked(printer_a3);
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
   QString s;
   s.append(p_name);
   for (int i=0; i<comboBox_2->count(); i++)
       if (comboBox_2->itemText(i)==p_name)
       {
           comboBox_2->setCurrentIndex(i);
       }
   load_combobox();
   doubleSpinBox->setValue(h_ofsett);
   comboBox_3->setCurrentIndex(pap_sor);
   clse=false; // разрешить изменение настроек
   comboBox->setCurrentIndex(pap_name);
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
    top_m=spinBox->value();
    left_m=spinBox_2->value();
    right_m=spinBox_3->value();
    bottom_m=spinBox_4->value();
    if(radioButton->isChecked()) ornl=1; else ornl=0;
    print_color=checkBox_2->isChecked();
    set_paper(comboBox->currentIndex());
    paper_h=ph;
    paper_w=pw;
    list_n.clear();
    if (comboBox->currentIndex()<7) list_n.append(nms[comboBox->currentIndex()]);
        else
        {
            list_n.append("Users size ");
            list_n.append(QString::number(pw));
            list_n.append(" x ");
            list_n.append(QString::number(ph));
            list_n.append(" mm");
        }
    prn_size_x=210;
    prn_size_y=297;
    if (checkBox_9->isChecked())
    {
        prn_size_x=297;
        prn_size_y=420;
    }
    emit end_set();
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

void PageSetup::on_pushButton_2_clicked()
{
    if (dp==0)
    {
        dp=new Dialog_paper(this);
        connect(dp, SIGNAL(close_form()), this, SLOT(load_combobox()));
    }
    dp->load_data(comboBox->currentText(), pw, ph, comboBox->currentIndex());
    dp->show();
}

void PageSetup::on_comboBox_currentIndexChanged(int index)
{
    if (clse) return;
    spinBox_5->setValue(p_w[index]);
    spinBox_6->setValue(p_h[index]);
    pw=p_w[index];
    ph=p_h[index];
    pap_name=index;
}

void PageSetup::on_spinBox_5_valueChanged(int arg1)
{
    pw=arg1;
}

void PageSetup::on_spinBox_6_valueChanged(int arg1)
{
    pw=arg1;
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

void PageSetup::on_checkBox_9_clicked(bool checked)
{
    if (clse) return;
    printer_a3=checked;
}
