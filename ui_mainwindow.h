/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *sheet;
    QLabel *label;
    QLabel *list;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QPushButton *pushButton_25;
    QPushButton *pushButton_26;
    QPushButton *pushButton_27;
    QPushButton *pushButton_28;
    QPushButton *pushButton_30;
    QPushButton *pushButton_31;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QPushButton *pushButton_36;
    QFrame *line_6;
    QPushButton *pushButton_37;
    QLabel *label_2;
    QFrame *line_9;
    QPushButton *pushButton_7;
    QWidget *tab;
    QPushButton *pushButton;
    QSpinBox *spinBox;
    QFrame *line_10;
    QLabel *label_3;
    QPushButton *pushButton_4;
    QWidget *tab_4;
    QPushButton *l1_2;
    QPushButton *l2_2;
    QPushButton *l3_2;
    QPushButton *l4_2;
    QPushButton *l5_2;
    QPushButton *l6_2;
    QPushButton *l7_2;
    QPushButton *l8_2;
    QPushButton *l9_2;
    QPushButton *l10_2;
    QPushButton *pushButton_38;
    QPushButton *pushButton_39;
    QLabel *label_17;
    QLabel *checkset_2;
    QWidget *tab_5;
    QCheckBox *checkBox_7;
    QFrame *line_4;
    QCheckBox *checkBox_8;
    QCheckBox *checkBox_12;
    QDial *dial_3;
    QDial *dial_4;
    QLabel *label_8;
    QLabel *label_14;
    QFrame *line_7;
    QPushButton *pushButton_32;
    QPushButton *pushButton_3;
    QWidget *tab_7;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QLabel *label_18;
    QFrame *line_8;
    QPushButton *pushButton_19;
    QPushButton *pushButton_20;
    QPushButton *pushButton_21;
    QPushButton *pushButton_22;
    QLabel *label_19;
    QFrame *line_11;
    QLabel *label_4;
    QPushButton *pushButton_23;
    QWidget *tab_2;
    QPushButton *pushButton_5;
    QPushButton *pushButton_2;
    QPushButton *pushButton_6;
    QWidget *tab_6;
    QPushButton *pushButton_35;
    QComboBox *comboBox;
    QLabel *label_15;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox;
    QLabel *label_16;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(655, 616);
        MainWindow->setMinimumSize(QSize(621, 200));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icon"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setIconSize(QSize(32, 32));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sheet = new QFrame(centralWidget);
        sheet->setObjectName(QStringLiteral("sheet"));
        sheet->setGeometry(QRect(150, 70, 420, 561));
        sheet->setFrameShape(QFrame::NoFrame);
        sheet->setFrameShadow(QFrame::Plain);
        sheet->setLineWidth(4);
        label = new QLabel(sheet);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 210, 128, 128));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/indicator")));
        list = new QLabel(centralWidget);
        list->setObjectName(QStringLiteral("list"));
        list->setGeometry(QRect(560, 10, 57, 15));
        list->setFrameShape(QFrame::Box);
        list->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/sheet")));
        list->setScaledContents(true);
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(0, 540, 651, 70));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        pushButton_25 = new QPushButton(tab_3);
        pushButton_25->setObjectName(QStringLiteral("pushButton_25"));
        pushButton_25->setGeometry(QRect(0, 0, 44, 38));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/op_file"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_25->setIcon(icon1);
        pushButton_25->setIconSize(QSize(32, 32));
        pushButton_26 = new QPushButton(tab_3);
        pushButton_26->setObjectName(QStringLiteral("pushButton_26"));
        pushButton_26->setGeometry(QRect(43, 0, 44, 38));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/op_folder"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_26->setIcon(icon2);
        pushButton_26->setIconSize(QSize(32, 32));
        pushButton_27 = new QPushButton(tab_3);
        pushButton_27->setObjectName(QStringLiteral("pushButton_27"));
        pushButton_27->setGeometry(QRect(110, 0, 44, 38));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/prefix1/save"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_27->setIcon(icon3);
        pushButton_27->setIconSize(QSize(32, 32));
        pushButton_28 = new QPushButton(tab_3);
        pushButton_28->setObjectName(QStringLiteral("pushButton_28"));
        pushButton_28->setGeometry(QRect(153, 0, 44, 38));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/new/prefix1/load"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_28->setIcon(icon4);
        pushButton_28->setIconSize(QSize(32, 32));
        pushButton_30 = new QPushButton(tab_3);
        pushButton_30->setObjectName(QStringLiteral("pushButton_30"));
        pushButton_30->setGeometry(QRect(330, 0, 44, 38));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/new/prefix1/down"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_30->setIcon(icon5);
        pushButton_30->setIconSize(QSize(32, 32));
        pushButton_31 = new QPushButton(tab_3);
        pushButton_31->setObjectName(QStringLiteral("pushButton_31"));
        pushButton_31->setGeometry(QRect(500, 0, 44, 38));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/new/prefix1/up"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_31->setIcon(icon6);
        pushButton_31->setIconSize(QSize(32, 32));
        line = new QFrame(tab_3);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(550, 2, 3, 35));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(tab_3);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(325, 0, 3, 35));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(tab_3);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(270, 0, 3, 35));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        pushButton_36 = new QPushButton(tab_3);
        pushButton_36->setObjectName(QStringLiteral("pushButton_36"));
        pushButton_36->setGeometry(QRect(600, 0, 44, 38));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/new/prefix1/about"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_36->setIcon(icon7);
        pushButton_36->setIconSize(QSize(32, 32));
        line_6 = new QFrame(tab_3);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setGeometry(QRect(590, 0, 3, 35));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        pushButton_37 = new QPushButton(tab_3);
        pushButton_37->setObjectName(QStringLiteral("pushButton_37"));
        pushButton_37->setGeometry(QRect(275, 0, 44, 38));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/new/prefix1/text"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_37->setIcon(icon8);
        pushButton_37->setIconSize(QSize(32, 32));
        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(370, 10, 131, 20));
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignCenter);
        line_9 = new QFrame(tab_3);
        line_9->setObjectName(QStringLiteral("line_9"));
        line_9->setGeometry(QRect(100, 0, 3, 35));
        line_9->setFrameShape(QFrame::VLine);
        line_9->setFrameShadow(QFrame::Sunken);
        pushButton_7 = new QPushButton(tab_3);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(200, 0, 44, 38));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/new/prefix1/tofile"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_7->setIcon(icon9);
        pushButton_7->setIconSize(QSize(40, 40));
        tabWidget_2->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(160, 0, 44, 38));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/new/prefix1/pdf"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon10);
        pushButton->setIconSize(QSize(32, 32));
        spinBox = new QSpinBox(tab);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(590, 5, 47, 27));
        spinBox->setMinimum(1);
        line_10 = new QFrame(tab);
        line_10->setObjectName(QStringLiteral("line_10"));
        line_10->setGeometry(QRect(230, 2, 3, 32));
        line_10->setFrameShape(QFrame::VLine);
        line_10->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(410, 9, 171, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(240, 0, 44, 38));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/new/prefix1/vap_dial"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon11);
        pushButton_4->setIconSize(QSize(32, 32));
        tabWidget_2->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        l1_2 = new QPushButton(tab_4);
        l1_2->setObjectName(QStringLiteral("l1_2"));
        l1_2->setGeometry(QRect(1, 1, 34, 34));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/new/prefix1/1c"), QSize(), QIcon::Normal, QIcon::Off);
        l1_2->setIcon(icon12);
        l1_2->setIconSize(QSize(32, 32));
        l1_2->setCheckable(true);
        l1_2->setChecked(false);
        l2_2 = new QPushButton(tab_4);
        l2_2->setObjectName(QStringLiteral("l2_2"));
        l2_2->setGeometry(QRect(40, 1, 34, 34));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/new/prefix1/1t"), QSize(), QIcon::Normal, QIcon::Off);
        l2_2->setIcon(icon13);
        l2_2->setIconSize(QSize(32, 32));
        l2_2->setCheckable(true);
        l3_2 = new QPushButton(tab_4);
        l3_2->setObjectName(QStringLiteral("l3_2"));
        l3_2->setGeometry(QRect(80, 1, 34, 34));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/new/prefix1/1x2"), QSize(), QIcon::Normal, QIcon::Off);
        l3_2->setIcon(icon14);
        l3_2->setIconSize(QSize(32, 32));
        l3_2->setCheckable(true);
        l4_2 = new QPushButton(tab_4);
        l4_2->setObjectName(QStringLiteral("l4_2"));
        l4_2->setGeometry(QRect(120, 1, 34, 34));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/new/prefix1/1x3"), QSize(), QIcon::Normal, QIcon::Off);
        l4_2->setIcon(icon15);
        l4_2->setIconSize(QSize(32, 32));
        l4_2->setCheckable(true);
        l5_2 = new QPushButton(tab_4);
        l5_2->setObjectName(QStringLiteral("l5_2"));
        l5_2->setGeometry(QRect(160, 1, 34, 34));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/new/prefix1/2x2"), QSize(), QIcon::Normal, QIcon::Off);
        l5_2->setIcon(icon16);
        l5_2->setIconSize(QSize(32, 32));
        l5_2->setCheckable(true);
        l6_2 = new QPushButton(tab_4);
        l6_2->setObjectName(QStringLiteral("l6_2"));
        l6_2->setGeometry(QRect(200, 1, 34, 34));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/new/prefix1/2x3"), QSize(), QIcon::Normal, QIcon::Off);
        l6_2->setIcon(icon17);
        l6_2->setIconSize(QSize(32, 32));
        l6_2->setCheckable(true);
        l7_2 = new QPushButton(tab_4);
        l7_2->setObjectName(QStringLiteral("l7_2"));
        l7_2->setGeometry(QRect(240, 1, 34, 34));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/new/prefix1/2x4"), QSize(), QIcon::Normal, QIcon::Off);
        l7_2->setIcon(icon18);
        l7_2->setIconSize(QSize(32, 32));
        l7_2->setCheckable(true);
        l8_2 = new QPushButton(tab_4);
        l8_2->setObjectName(QStringLiteral("l8_2"));
        l8_2->setGeometry(QRect(280, 1, 34, 34));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/new/prefix1/3x3"), QSize(), QIcon::Normal, QIcon::Off);
        l8_2->setIcon(icon19);
        l8_2->setIconSize(QSize(32, 32));
        l8_2->setCheckable(true);
        l9_2 = new QPushButton(tab_4);
        l9_2->setObjectName(QStringLiteral("l9_2"));
        l9_2->setGeometry(QRect(320, 1, 34, 34));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/new/prefix1/3x5"), QSize(), QIcon::Normal, QIcon::Off);
        l9_2->setIcon(icon20);
        l9_2->setIconSize(QSize(32, 32));
        l9_2->setCheckable(true);
        l10_2 = new QPushButton(tab_4);
        l10_2->setObjectName(QStringLiteral("l10_2"));
        l10_2->setGeometry(QRect(360, 1, 34, 34));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/new/prefix1/4x5"), QSize(), QIcon::Normal, QIcon::Off);
        l10_2->setIcon(icon21);
        l10_2->setIconSize(QSize(32, 32));
        l10_2->setCheckable(true);
        pushButton_38 = new QPushButton(tab_4);
        pushButton_38->setObjectName(QStringLiteral("pushButton_38"));
        pushButton_38->setGeometry(QRect(400, 1, 34, 34));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/new/prefix1/all"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_38->setIcon(icon22);
        pushButton_38->setIconSize(QSize(32, 32));
        pushButton_38->setCheckable(true);
        pushButton_39 = new QPushButton(tab_4);
        pushButton_39->setObjectName(QStringLiteral("pushButton_39"));
        pushButton_39->setGeometry(QRect(440, 1, 34, 34));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/new/prefix1/my"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_39->setIcon(icon23);
        pushButton_39->setIconSize(QSize(32, 32));
        pushButton_39->setCheckable(true);
        label_17 = new QLabel(tab_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(480, 10, 161, 21));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label_17->setFont(font);
        checkset_2 = new QLabel(tab_4);
        checkset_2->setObjectName(QStringLiteral("checkset_2"));
        checkset_2->setGeometry(QRect(580, 0, 16, 16));
        checkset_2->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/check")));
        checkset_2->setScaledContents(true);
        checkset_2->setWordWrap(false);
        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        checkBox_7 = new QCheckBox(tab_5);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(70, 10, 111, 21));
        QFont font1;
        font1.setFamily(QStringLiteral("Ubuntu"));
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        checkBox_7->setFont(font1);
        checkBox_7->setChecked(true);
        line_4 = new QFrame(tab_5);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(190, 0, 3, 35));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        checkBox_8 = new QCheckBox(tab_5);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));
        checkBox_8->setGeometry(QRect(200, 1, 121, 16));
        checkBox_8->setTristate(false);
        checkBox_12 = new QCheckBox(tab_5);
        checkBox_12->setObjectName(QStringLiteral("checkBox_12"));
        checkBox_12->setGeometry(QRect(200, 20, 161, 16));
        dial_3 = new QDial(tab_5);
        dial_3->setObjectName(QStringLiteral("dial_3"));
        dial_3->setGeometry(QRect(540, 0, 31, 31));
        dial_3->setMaximum(100);
        dial_3->setValue(10);
        dial_3->setOrientation(Qt::Horizontal);
        dial_4 = new QDial(tab_5);
        dial_4->setObjectName(QStringLiteral("dial_4"));
        dial_4->setGeometry(QRect(600, 0, 31, 31));
        dial_4->setMinimum(-180);
        dial_4->setMaximum(180);
        dial_4->setPageStep(1);
        label_8 = new QLabel(tab_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(515, 5, 21, 21));
        label_8->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/marg")));
        label_8->setScaledContents(true);
        label_14 = new QLabel(tab_5);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(580, 5, 21, 21));
        label_14->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/rotor")));
        label_14->setScaledContents(true);
        line_7 = new QFrame(tab_5);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setGeometry(QRect(510, 0, 3, 35));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);
        pushButton_32 = new QPushButton(tab_5);
        pushButton_32->setObjectName(QStringLiteral("pushButton_32"));
        pushButton_32->setGeometry(QRect(360, 0, 44, 38));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/new/prefix1/clip"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_32->setIcon(icon24);
        pushButton_32->setIconSize(QSize(32, 32));
        pushButton_3 = new QPushButton(tab_5);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 0, 44, 38));
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/new/prefix1/rotbtn"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon25);
        pushButton_3->setIconSize(QSize(32, 32));
        tabWidget_2->addTab(tab_5, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        pushButton_17 = new QPushButton(tab_7);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(110, 10, 31, 27));
        QIcon icon26;
        icon26.addFile(QStringLiteral(":/new/prefix1/pls"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_17->setIcon(icon26);
        pushButton_18 = new QPushButton(tab_7);
        pushButton_18->setObjectName(QStringLiteral("pushButton_18"));
        pushButton_18->setGeometry(QRect(140, 10, 31, 27));
        QIcon icon27;
        icon27.addFile(QStringLiteral(":/new/prefix1/mns"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_18->setIcon(icon27);
        label_18 = new QLabel(tab_7);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(10, 10, 91, 20));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line_8 = new QFrame(tab_7);
        line_8->setObjectName(QStringLiteral("line_8"));
        line_8->setGeometry(QRect(190, 10, 3, 30));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);
        pushButton_19 = new QPushButton(tab_7);
        pushButton_19->setObjectName(QStringLiteral("pushButton_19"));
        pushButton_19->setGeometry(QRect(330, 10, 31, 27));
        QIcon icon28;
        icon28.addFile(QStringLiteral(":/new/prefix1/toleft"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_19->setIcon(icon28);
        pushButton_20 = new QPushButton(tab_7);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));
        pushButton_20->setGeometry(QRect(360, 10, 31, 27));
        QIcon icon29;
        icon29.addFile(QStringLiteral(":/new/prefix1/toright"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_20->setIcon(icon29);
        pushButton_21 = new QPushButton(tab_7);
        pushButton_21->setObjectName(QStringLiteral("pushButton_21"));
        pushButton_21->setGeometry(QRect(390, 10, 31, 27));
        QIcon icon30;
        icon30.addFile(QStringLiteral(":/new/prefix1/toup"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_21->setIcon(icon30);
        pushButton_22 = new QPushButton(tab_7);
        pushButton_22->setObjectName(QStringLiteral("pushButton_22"));
        pushButton_22->setGeometry(QRect(420, 10, 31, 27));
        QIcon icon31;
        icon31.addFile(QStringLiteral(":/new/prefix1/todown"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_22->setIcon(icon31);
        label_19 = new QLabel(tab_7);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(180, 10, 141, 17));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line_11 = new QFrame(tab_7);
        line_11->setObjectName(QStringLiteral("line_11"));
        line_11->setGeometry(QRect(460, 10, 3, 30));
        line_11->setFrameShape(QFrame::VLine);
        line_11->setFrameShadow(QFrame::Sunken);
        label_4 = new QLabel(tab_7);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(465, 10, 131, 17));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_23 = new QPushButton(tab_7);
        pushButton_23->setObjectName(QStringLiteral("pushButton_23"));
        pushButton_23->setGeometry(QRect(610, 10, 31, 27));
        QIcon icon32;
        icon32.addFile(QStringLiteral(":/new/prefix1/tocenter"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_23->setIcon(icon32);
        pushButton_23->setIconSize(QSize(24, 24));
        tabWidget_2->addTab(tab_7, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        pushButton_5 = new QPushButton(tab_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setEnabled(false);
        pushButton_5->setGeometry(QRect(0, 0, 44, 38));
        QIcon icon33;
        icon33.addFile(QStringLiteral(":/new/prefix1/colors"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_5->setIcon(icon33);
        pushButton_5->setIconSize(QSize(32, 32));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(50, 0, 44, 38));
        QIcon icon34;
        icon34.addFile(QStringLiteral(":/new/prefix1/transform"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon34);
        pushButton_2->setIconSize(QSize(32, 32));
        pushButton_6 = new QPushButton(tab_2);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setEnabled(false);
        pushButton_6->setGeometry(QRect(100, 0, 44, 38));
        QIcon icon35;
        icon35.addFile(QStringLiteral(":/new/prefix1/lamp"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_6->setIcon(icon35);
        pushButton_6->setIconSize(QSize(32, 32));
        tabWidget_2->addTab(tab_2, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        pushButton_35 = new QPushButton(tab_6);
        pushButton_35->setObjectName(QStringLiteral("pushButton_35"));
        pushButton_35->setGeometry(QRect(2, 0, 44, 38));
        QIcon icon36;
        icon36.addFile(QStringLiteral(":/new/prefix1/sett"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_35->setIcon(icon36);
        pushButton_35->setIconSize(QSize(32, 32));
        comboBox = new QComboBox(tab_6);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(470, 15, 171, 23));
        label_15 = new QLabel(tab_6);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(470, -2, 171, 20));
        QFont font2;
        font2.setPointSize(9);
        label_15->setFont(font2);
        label_15->setAlignment(Qt::AlignCenter);
        checkBox_9 = new QCheckBox(tab_6);
        checkBox_9->setObjectName(QStringLiteral("checkBox_9"));
        checkBox_9->setGeometry(QRect(45, 0, 171, 21));
        checkBox_9->setChecked(true);
        checkBox_10 = new QCheckBox(tab_6);
        checkBox_10->setObjectName(QStringLiteral("checkBox_10"));
        checkBox_10->setGeometry(QRect(45, 20, 181, 16));
        checkBox_10->setChecked(true);
        checkBox = new QCheckBox(tab_6);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(230, 0, 161, 22));
        tabWidget_2->addTab(tab_6, QString());
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(0, 50, 451, 20));
        label_16->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        list->raise();
        label_16->raise();
        sheet->raise();
        tabWidget_2->raise();

        retranslateUi(MainWindow);

        tabWidget_2->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fast print", 0));
        label->setText(QString());
        list->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_25->setToolTip(QApplication::translate("MainWindow", "Open file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        pushButton_25->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        pushButton_25->setText(QString());
        pushButton_25->setShortcut(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_26->setToolTip(QApplication::translate("MainWindow", "Open folder", 0));
#endif // QT_NO_TOOLTIP
        pushButton_26->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_27->setToolTip(QApplication::translate("MainWindow", "Save current session", 0));
#endif // QT_NO_TOOLTIP
        pushButton_27->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_28->setToolTip(QApplication::translate("MainWindow", "Open saved session", 0));
#endif // QT_NO_TOOLTIP
        pushButton_28->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_30->setToolTip(QApplication::translate("MainWindow", "Previous sheet", 0));
#endif // QT_NO_TOOLTIP
        pushButton_30->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_31->setToolTip(QApplication::translate("MainWindow", "Next sheet", 0));
#endif // QT_NO_TOOLTIP
        pushButton_31->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_36->setToolTip(QApplication::translate("MainWindow", "About", 0));
#endif // QT_NO_TOOLTIP
        pushButton_36->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_37->setToolTip(QApplication::translate("MainWindow", "Insert text block", 0));
#endif // QT_NO_TOOLTIP
        pushButton_37->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Sheet 0 of 0", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_7->setToolTip(QApplication::translate("MainWindow", "Save sheet as image file", 0));
#endif // QT_NO_TOOLTIP
        pushButton_7->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("MainWindow", "File", 0));
#ifndef QT_NO_TOOLTIP
        pushButton->setToolTip(QApplication::translate("MainWindow", "Print to PDF file", 0));
#endif // QT_NO_TOOLTIP
        pushButton->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Number of copies", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("MainWindow", "Print using the system print dialog", 0));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QApplication::translate("MainWindow", "Print", 0));
#ifndef QT_NO_TOOLTIP
        l1_2->setToolTip(QApplication::translate("MainWindow", "1 in center", 0));
#endif // QT_NO_TOOLTIP
        l1_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l2_2->setToolTip(QApplication::translate("MainWindow", "1 top", 0));
#endif // QT_NO_TOOLTIP
        l2_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l3_2->setToolTip(QApplication::translate("MainWindow", "1x2", 0));
#endif // QT_NO_TOOLTIP
        l3_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l4_2->setToolTip(QApplication::translate("MainWindow", "1x3", 0));
#endif // QT_NO_TOOLTIP
        l4_2->setText(QString());
        l4_2->setShortcut(QApplication::translate("MainWindow", "Left", 0));
#ifndef QT_NO_TOOLTIP
        l5_2->setToolTip(QApplication::translate("MainWindow", "2x2", 0));
#endif // QT_NO_TOOLTIP
        l5_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l6_2->setToolTip(QApplication::translate("MainWindow", "2x3", 0));
#endif // QT_NO_TOOLTIP
        l6_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l7_2->setToolTip(QApplication::translate("MainWindow", "2x4", 0));
#endif // QT_NO_TOOLTIP
        l7_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l8_2->setToolTip(QApplication::translate("MainWindow", "3x3", 0));
#endif // QT_NO_TOOLTIP
        l8_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l9_2->setToolTip(QApplication::translate("MainWindow", "3x4", 0));
#endif // QT_NO_TOOLTIP
        l9_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        l10_2->setToolTip(QApplication::translate("MainWindow", "4x5", 0));
#endif // QT_NO_TOOLTIP
        l10_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_38->setToolTip(QApplication::translate("MainWindow", "All in one", 0));
#endif // QT_NO_TOOLTIP
        pushButton_38->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_39->setToolTip(QApplication::translate("MainWindow", "Users layout", 0));
#endif // QT_NO_TOOLTIP
        pushButton_39->setText(QString());
        label_17->setText(QApplication::translate("MainWindow", "1x1", 0));
        checkset_2->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("MainWindow", "Layout", 0));
        checkBox_7->setText(QApplication::translate("MainWindow", "For all", 0));
        checkBox_8->setText(QApplication::translate("MainWindow", "Fill", 0));
        checkBox_12->setText(QApplication::translate("MainWindow", "With captions", 0));
#ifndef QT_NO_TOOLTIP
        dial_3->setToolTip(QApplication::translate("MainWindow", "Interval between pictures", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_4->setToolTip(QApplication::translate("MainWindow", "Rotate a picture", 0));
#endif // QT_NO_TOOLTIP
        label_8->setText(QString());
        label_14->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_32->setToolTip(QApplication::translate("MainWindow", "Clip", 0));
#endif // QT_NO_TOOLTIP
        pushButton_32->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("MainWindow", "Change the page orientation", 0));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("MainWindow", "Edit", 0));
        pushButton_17->setText(QString());
        pushButton_18->setText(QString());
        label_18->setText(QApplication::translate("MainWindow", " Size 1%", 0));
        pushButton_19->setText(QString());
        pushButton_20->setText(QString());
        pushButton_21->setText(QString());
        pushButton_22->setText(QString());
        label_19->setText(QApplication::translate("MainWindow", "Position", 0));
        label_4->setText(QApplication::translate("MainWindow", "To the center", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_23->setToolTip(QApplication::translate("MainWindow", "Put the current image in the center of the sheet", 0));
#endif // QT_NO_TOOLTIP
        pushButton_23->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QApplication::translate("MainWindow", "Exact", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_5->setToolTip(QApplication::translate("MainWindow", "Color management", 0));
#endif // QT_NO_TOOLTIP
        pushButton_5->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_2->setToolTip(QApplication::translate("MainWindow", "Transform the selected image", 0));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_6->setToolTip(QApplication::translate("MainWindow", "Lighting effects", 0));
#endif // QT_NO_TOOLTIP
        pushButton_6->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Effects", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_35->setToolTip(QApplication::translate("MainWindow", "Settings", 0));
#endif // QT_NO_TOOLTIP
        pushButton_35->setText(QString());
#ifndef QT_NO_TOOLTIP
        comboBox->setToolTip(QApplication::translate("MainWindow", "Only for the current session", 0));
#endif // QT_NO_TOOLTIP
        label_15->setText(QApplication::translate("MainWindow", "Paper size", 0));
        checkBox_9->setText(QApplication::translate("MainWindow", "Quick edit", 0));
        checkBox_10->setText(QApplication::translate("MainWindow", "Keep ratio", 0));
        checkBox->setText(QApplication::translate("MainWindow", "No margins", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QApplication::translate("MainWindow", "Settings", 0));
        label_16->setText(QApplication::translate("MainWindow", "Downloading files. It may take some time ...", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
