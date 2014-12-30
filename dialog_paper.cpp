#include "dialog_paper.h"
#include "ui_dialog_paper.h"
#include <QSettings>
#include <QVector>

int ind;
QSettings sett("vap", "vap");
int indexSel;                       // выбранный индекс в списке
bool chen=false;                    // было ли изменение списка
std::vector <double> Xses;          // массив ширин дополнительных размеров бумаги
std::vector <double> Yses;          // массив высот дополнительных размеров бумаги



Dialog_paper::Dialog_paper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_paper)
{
    ui->setupUi(this);
    ui->label_3->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
}

void Dialog_paper::on_listWidget_currentRowChanged(int currentRow)
{
    listV_indexCh(currentRow);
}

void Dialog_paper::on_listWidget_2_currentRowChanged(int currentRow)
{
    list_index_cl(currentRow);
}

void Dialog_paper::load_lists()
{
    load_papes();
    load_addSizes();
    load_myPap();
}

void Dialog_paper::listV_indexCh(int index)
{
     if(index>33) ui->pushButton_6->setVisible(true); else ui->pushButton_6->setVisible(false);
     indexSel=index;
     ui->pushButton_4->setEnabled(true);
     ui->pushButton_5->setEnabled(false);
}

Dialog_paper::~Dialog_paper()
{
    delete ui;
}

void Dialog_paper::load_papes()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("A4: 210 x 297 mm");
    ui->listWidget->addItem("B5: 176 x 250 mm");
    ui->listWidget->addItem("Letter: 215.9 x 279.4 mm");
    ui->listWidget->addItem("Legal: 215.9 x 355.6 mm");
    ui->listWidget->addItem("Executive: 190.5 x 254 mm");
    ui->listWidget->addItem("A0: 841 x 1189 mm");
    ui->listWidget->addItem("A1: 594 x 841 mm");
    ui->listWidget->addItem("A2: 420 x 594 mm");
    ui->listWidget->addItem("A3: 297 x 420 mm");
    ui->listWidget->addItem("A5: 148 x 210 mm");
    ui->listWidget->addItem("A6: 105 x 148 mm");
    ui->listWidget->addItem("A7: 74 x 105 mm");
    ui->listWidget->addItem("A8: 52 x 74 mm");
    ui->listWidget->addItem("A9: 37 x 52 mm");
    ui->listWidget->addItem("B0: 1000 x 1414 mm");
    ui->listWidget->addItem("B1: 707 x 1000 mm");
    ui->listWidget->addItem("B10: 31 x 44 mm");
    ui->listWidget->addItem("B2: 500 x 707 mm");
    ui->listWidget->addItem("B3: 353 x 500 mm");
    ui->listWidget->addItem("B4: 250 x 353 mm");
    ui->listWidget->addItem("B6: 125 x 176 mm");
    ui->listWidget->addItem("B7: 88 x 125 mm");
    ui->listWidget->addItem("B8: 62 x 88 mm");
    ui->listWidget->addItem("B9: 33 x 62 mm");
    ui->listWidget->addItem("C5E: 163 x 229 mm");
    ui->listWidget->addItem("Comm10E: 105 x 241 mm");
    ui->listWidget->addItem("DLE: 110 x 220 mm");
    ui->listWidget->addItem("Folio: 210 x 330 mm");
    ui->listWidget->addItem("Ledger: 431.8 x 279.4 mm");
    ui->listWidget->addItem("Tabloid: 279.4 x 431.8 mm");
    ui->listWidget->addItem("Photo 9x13: 90 x 127 mm");
    ui->listWidget->addItem("Photo 10x15: 102 x 152 mm");
    ui->listWidget->addItem("Photo 13x18: 127 x 178 mm");
    ui->listWidget->addItem("Photo 15x20: 152 x 203 mm");
    ui->pushButton_6->setVisible(false);
}

void Dialog_paper::load_myPap()
{
    ui->listWidget_2->clear();
    ui->listWidget_2->addItem("A4: 210 x 297 mm");
    sett.beginGroup("Paper sizes");
    bool go=true;
    int  i=0;
    while(go)
    {
        if (sett.value("index"+QString::number(i), -1).toInt()>0){
            ui->listWidget_2->addItem(sett.value("name"+QString::number(i++), "").toString());
        }
        else  go=false;
    }
    sett.endGroup();
}


void Dialog_paper::on_pushButton_2_clicked() // Ok
{
    this->close();
}

void Dialog_paper::on_pushButton_4_clicked() // добавить в список предпочтительных размеров
{
    QString nm=ui->listWidget->item(indexSel)->text();
    ui->listWidget_2->addItem(nm);
    sett.beginGroup("Paper sizes");
    int i=0;
    int j=-1;
    while (i!=-1) {
        j++;
        i=sett.value("index"+QString::number(j), -1).toInt();
    }
    sett.setValue("index"+QString::number(j), indexSel);
    sett.setValue("name"+QString::number(j), nm);
    sett.setValue("W"+QString::number(j), get_width(indexSel));
    sett.setValue("H"+QString::number(j), get_height(indexSel));
    sett.endGroup();
    chen=true;
}

double Dialog_paper::get_height(int index)
{
    if(index>33)return Yses[index-34];
    switch(index)
    {
    case 0: return 297;
    case 1: return 250;
    case 2: return 279.4;
    case 3: return 355.6;
    case 4: return 254;
    case 5: return 1189;
    case 6: return 841;
    case 7: return 594;
    case 8: return 420;
    case 9: return 210;
    case 10: return 148;
    case 11: return 105;
    case 12: return 74;
    case 13: return 52;
    case 14: return 1414;
    case 15: return 1000;
    case 16: return 44;
    case 17: return 707;
    case 18: return 500;
    case 19: return 353;
    case 20: return 176;
    case 21: return 125;
    case 22: return 88;
    case 23: return 62;
    case 24: return 229;
    case 25: return 241;
    case 26: return 220;
    case 27: return 330;
    case 28: return 279.4;
    case 29: return 431.8;
    case 30: return 127;
    case 31: return 152;
    case 32: return 178;
    case 33: return 203;
    }
    return 297;
}

double Dialog_paper::get_width(int index)
{
    if(index>33)return Xses[index-34];
    switch(index)
    {
    case 0: return 210;
    case 1: return 176;
    case 2: return 215.9;
    case 3: return 215.9;
    case 4: return 190.5;
    case 5: return 841;
    case 6: return 594;
    case 7: return 420;
    case 8: return 297;
    case 9: return 148;
    case 10: return 105;
    case 11: return 74;
    case 12: return 52;
    case 13: return 37;
    case 14: return 1000;
    case 15: return 707;
    case 16: return 31;
    case 17: return 500;
    case 18: return 353;
    case 19: return 250;
    case 20: return 125;
    case 21: return 88;
    case 22: return 62;
    case 23: return 33;
    case 24: return 163;
    case 25: return 105;
    case 26: return 110;
    case 27: return 210;
    case 28: return 431.8;
    case 29: return 979.4;
    case 30: return 90;
    case 31: return 102;
    case 32: return 127;
    case 33: return 152;
    }
    return 210;
}

void Dialog_paper::list_index_cl(int index)
// клик по списку выбранных
{
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_4->setEnabled(false);
    indexSel=index;
    ui->pushButton_6->setVisible(false);
}

void Dialog_paper::on_Dialog_paper_finished(int result)
// сигнал закрытия формы
{
    if(chen) result=1; else result=0;
    emit close_form(result);
 }

void Dialog_paper::on_pushButton_5_clicked() // убрать из списка предпочтений
{
    if(indexSel==-1)return;
    QString nm=ui->listWidget_2->item(indexSel)->text(), rm;
    // убрать из списка
    ui->listWidget_2->takeItem(indexSel);
    indexSel=-1;
    ui->pushButton_5->setEnabled(false);
    // убрать из файла
    sett.beginGroup("Paper sizes");
    bool go=true;
    int  i=0, j=0;
    while(go)
    {
        rm=sett.value("name"+QString::number(i), "").toString();
        if (nm!=rm){
            if(rm.isEmpty()){go=false;}else{
            sett.setValue("index"+QString::number(j), sett.value("index"+QString::number(i)).toInt());
            sett.setValue("name"+QString::number(j), rm);
            sett.setValue("W"+QString::number(j), sett.value("W"+QString::number(i),0).toDouble());
            sett.setValue("H"+QString::number(j++), sett.value("H"+QString::number(i)).toDouble());
            }
        }
        if(i>100)go=false;  // на всякий случай...
        i++;
    }
    sett.remove("index"+QString::number(j));
    sett.remove("name"+QString::number(j));
    sett.remove("W"+QString::number(j));
    sett.remove("H"+QString::number(j));
    sett.endGroup();
    chen=true;
}

void Dialog_paper::on_pushButton_3_clicked() // включить добавление размеров
{
    ui->label_3->setVisible(!ui->label_3->isVisible());
    ui->lineEdit->setVisible(!ui->lineEdit->isVisible());
    ui->pushButton->setVisible(!ui->pushButton->isVisible());
    if(ui->pushButton->isVisible()) ui->pushButton_3->setText(tr("Add new (click to hide)"));
        else ui->pushButton_3->setText(tr("Add new"));
}

bool Dialog_paper::dig(QString s)
{
    QString d="0123456789.";
    if(s.length()==1)return d.contains(s);
    return false;
}

void Dialog_paper::on_pushButton_clicked() // сохранить вариант бумаги
{
    if(ui->lineEdit->text().isEmpty()) return;
    double x=0,y=0;
    bool   f1=false, f2=false;
    QString s, d, n;
    d.clear();
    for(int i=0; i<ui->lineEdit->text().length(); i++)
    {
        s=ui->lineEdit->text().mid(i,1);
        if(s==":") {
            n=ui->lineEdit->text().mid(1,i); //имя
            f1=true;
        }
        if(f1 and dig(s)) d.push_back(s); else
        {
            if (s=="x" or s=="X") {
                f2=true;
                x=d.toDouble(); // ширина
                d.clear();
                f1=false;
            }
        }
        if(f2 and dig(s)) d.push_back(s); else
        {
          if (s=="m") {
                y=d.toDouble(); // высота
            }
            }
    }
    if(x>0 and y>0)ui->listWidget->addItem(ui->lineEdit->text());
    sett.beginGroup("Paper add sizes");
    int j=sett.value("count",0).toInt(); // кол-во уже сохраненных размеров
    sett.setValue("index"+QString::number(j), indexSel);
    sett.setValue("name"+QString::number(j), ui->lineEdit->text());
    sett.setValue("W"+QString::number(j), x);
    sett.setValue("H"+QString::number(j), y);
    sett.setValue("count", j+1);
    sett.endGroup();
}

void Dialog_paper::load_addSizes() // загрузить дополнительные размеры
{
    sett.beginGroup("Paper add sizes");
    int j=sett.value("count",0).toInt(); // кол-во уже сохраненных размеров
    if (j==0)
    {
        sett.endGroup();
        return;
    }
    Xses.clear();
    Yses.clear();
    for(int i=0; i<j; i++)
    {
        ui->listWidget->addItem(sett.value("name"+QString::number(i), "???").toString());
        Xses.push_back(sett.value("W"+QString::number(i), 0).toDouble());
        Yses.push_back(sett.value("H"+QString::number(i), 0).toDouble());
    }
    sett.endGroup();
}

void Dialog_paper::on_pushButton_6_clicked() // Удалить выбранный в списке дополнительный размер бумаги
{
    if(indexSel==-1)return;
    QString nm=ui->listWidget->item(indexSel)->text(), rm;
    // убрать из списка
    ui->listWidget->takeItem(indexSel);
    indexSel=-1;
    ui->pushButton_6->setVisible(false);
    // убрать из файла
    sett.beginGroup("Paper add sizes");
    int count=sett.value("count",0).toInt();
    bool go=true;
    int  i=0, j=0;
    while(go)
    {
        rm=sett.value("name"+QString::number(i), "").toString();
        if (nm!=rm){
            if(rm.isEmpty()){go=false;}else{
            sett.setValue("index"+QString::number(j), sett.value("index"+QString::number(i)).toInt());
            sett.setValue("name"+QString::number(j), rm);
            sett.setValue("W"+QString::number(j), sett.value("W"+QString::number(i),0).toDouble());
            sett.setValue("H"+QString::number(j++), sett.value("H"+QString::number(i)).toDouble());
            }
        }
        if(i>100)go=false;  // на всякий случай...
        i++;
    }
    sett.remove("index"+QString::number(j));
    sett.remove("name"+QString::number(j));
    sett.remove("W"+QString::number(j));
    sett.remove("H"+QString::number(j));
    sett.setValue("count", count-1);
    sett.endGroup();
    chen=true;
}




