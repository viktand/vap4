/********************************************************************************
** Form generated from reading UI file 'tofile.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOFILE_H
#define UI_TOFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_toFile
{
public:
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QLabel *label;
    QPushButton *pushButton;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *spinBox_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;

    void setupUi(QDialog *toFile)
    {
        if (toFile->objectName().isEmpty())
            toFile->setObjectName(QStringLiteral("toFile"));
        toFile->resize(400, 291);
        toFile->setMinimumSize(QSize(400, 291));
        toFile->setMaximumSize(QSize(400, 291));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/tofile"), QSize(), QIcon::Normal, QIcon::Off);
        toFile->setWindowIcon(icon);
        groupBox = new QGroupBox(toFile);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(40, 20, 281, 61));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(10, 20, 116, 22));
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(100, 20, 116, 22));
        radioButton_3 = new QRadioButton(groupBox);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));
        radioButton_3->setGeometry(QRect(180, 20, 116, 22));
        label = new QLabel(toFile);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 90, 70, 17));
        pushButton = new QPushButton(toFile);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(150, 250, 101, 27));
        horizontalSlider = new QSlider(toFile);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setEnabled(false);
        horizontalSlider->setGeometry(QRect(30, 110, 341, 29));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setValue(90);
        horizontalSlider->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(toFile);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(100, 150, 101, 27));
        spinBox->setMaximum(10000);
        spinBox->setValue(1024);
        label_2 = new QLabel(toFile);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 150, 31, 21));
        label_3 = new QLabel(toFile);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(70, 180, 31, 21));
        spinBox_2 = new QSpinBox(toFile);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setEnabled(false);
        spinBox_2->setGeometry(QRect(100, 180, 101, 27));
        spinBox_2->setMaximum(10000);
        spinBox_2->setValue(1024);
        checkBox = new QCheckBox(toFile);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(210, 170, 171, 22));
        checkBox->setChecked(true);
        checkBox_2 = new QCheckBox(toFile);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setEnabled(false);
        checkBox_2->setGeometry(QRect(40, 220, 341, 22));

        retranslateUi(toFile);

        QMetaObject::connectSlotsByName(toFile);
    } // setupUi

    void retranslateUi(QDialog *toFile)
    {
        toFile->setWindowTitle(QApplication::translate("toFile", "Save sheet to file", 0));
        groupBox->setTitle(QApplication::translate("toFile", "Format", 0));
        radioButton->setText(QApplication::translate("toFile", "jpg", 0));
        radioButton_2->setText(QApplication::translate("toFile", "png", 0));
        radioButton_3->setText(QApplication::translate("toFile", "bmp", 0));
        label->setText(QApplication::translate("toFile", "Quality", 0));
        pushButton->setText(QApplication::translate("toFile", "Save", 0));
        label_2->setText(QApplication::translate("toFile", "X =", 0));
        label_3->setText(QApplication::translate("toFile", "Y =", 0));
#ifndef QT_NO_TOOLTIP
        checkBox->setToolTip(QApplication::translate("toFile", "Automatically set the height", 0));
#endif // QT_NO_TOOLTIP
        checkBox->setText(QApplication::translate("toFile", "Auto", 0));
        checkBox_2->setText(QApplication::translate("toFile", "Transparent background (only png)", 0));
    } // retranslateUi

};

namespace Ui {
    class toFile: public Ui_toFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOFILE_H
