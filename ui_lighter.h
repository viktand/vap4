/********************************************************************************
** Form generated from reading UI file 'lighter.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTER_H
#define UI_LIGHTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_lighter
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton;
    QSlider *horizontalSlider;
    QLabel *label;
    QLabel *label_2;
    QWidget *tab_2;
    QSlider *horizontalSlider_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QSlider *horizontalSlider_3;
    QPushButton *pushButton_5;
    QWidget *tab_3;
    QSlider *horizontalSlider_4;
    QSlider *horizontalSlider_5;
    QPushButton *pushButton_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;

    void setupUi(QDialog *lighter)
    {
        if (lighter->objectName().isEmpty())
            lighter->setObjectName(QStringLiteral("lighter"));
        lighter->resize(514, 598);
        lighter->setMinimumSize(QSize(514, 598));
        lighter->setMaximumSize(QSize(514, 598));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icon"), QSize(), QIcon::Normal, QIcon::Off);
        lighter->setWindowIcon(icon);
        tabWidget = new QTabWidget(lighter);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 419, 491, 121));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(340, 60, 141, 27));
        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 30, 241, 29));
        horizontalSlider->setMaximum(255);
        horizontalSlider->setValue(128);
        horizontalSlider->setOrientation(Qt::Horizontal);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 271, 17));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(270, 30, 41, 17));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalSlider_2 = new QSlider(tab_2);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(10, 20, 231, 29));
        horizontalSlider_2->setMaximum(255);
        horizontalSlider_2->setValue(128);
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(15, 6, 121, 17));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(260, 20, 41, 17));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(15, 46, 121, 17));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(260, 60, 41, 17));
        horizontalSlider_3 = new QSlider(tab_2);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(10, 60, 231, 29));
        horizontalSlider_3->setMaximum(255);
        horizontalSlider_3->setValue(128);
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        pushButton_5 = new QPushButton(tab_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setEnabled(false);
        pushButton_5->setGeometry(QRect(340, 60, 141, 27));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalSlider_4 = new QSlider(tab_3);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setGeometry(QRect(10, 20, 231, 29));
        horizontalSlider_4->setMaximum(255);
        horizontalSlider_4->setValue(128);
        horizontalSlider_4->setOrientation(Qt::Horizontal);
        horizontalSlider_5 = new QSlider(tab_3);
        horizontalSlider_5->setObjectName(QStringLiteral("horizontalSlider_5"));
        horizontalSlider_5->setGeometry(QRect(10, 60, 231, 29));
        horizontalSlider_5->setMaximum(255);
        horizontalSlider_5->setValue(128);
        horizontalSlider_5->setOrientation(Qt::Horizontal);
        pushButton_6 = new QPushButton(tab_3);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setEnabled(false);
        pushButton_6->setGeometry(QRect(340, 60, 141, 27));
        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 45, 101, 17));
        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 5, 181, 17));
        label_9 = new QLabel(tab_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(250, 20, 70, 17));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(250, 60, 70, 17));
        tabWidget->addTab(tab_3, QString());
        pushButton_3 = new QPushButton(lighter);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(350, 560, 141, 27));
        pushButton_4 = new QPushButton(lighter);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 560, 141, 27));
        pushButton_2 = new QPushButton(lighter);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(190, 560, 141, 27));

        retranslateUi(lighter);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(lighter);
    } // setupUi

    void retranslateUi(QDialog *lighter)
    {
        lighter->setWindowTitle(QApplication::translate("lighter", "light", 0));
        pushButton->setText(QApplication::translate("lighter", "Next ray", 0));
        label->setText(QApplication::translate("lighter", "Brightness of lamp", 0));
        label_2->setText(QApplication::translate("lighter", "50%", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("lighter", "Ray", 0));
        label_3->setText(QApplication::translate("lighter", "Power", 0));
        label_4->setText(QApplication::translate("lighter", "50 %", 0));
        label_5->setText(QApplication::translate("lighter", "Length", 0));
        label_6->setText(QApplication::translate("lighter", "50 %", 0));
        pushButton_5->setText(QApplication::translate("lighter", "Next flash", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("lighter", "Flash", 0));
        pushButton_6->setText(QApplication::translate("lighter", "Next boll", 0));
        label_7->setText(QApplication::translate("lighter", "Radius", 0));
        label_8->setText(QApplication::translate("lighter", "Brightness of lamp", 0));
        label_9->setText(QApplication::translate("lighter", "50%", 0));
        label_10->setText(QApplication::translate("lighter", "128", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("lighter", "Boll", 0));
        pushButton_3->setText(QApplication::translate("lighter", "Ok", 0));
        pushButton_4->setText(QApplication::translate("lighter", "Reload", 0));
        pushButton_2->setText(QApplication::translate("lighter", "Back", 0));
    } // retranslateUi

};

namespace Ui {
    class lighter: public Ui_lighter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTER_H
