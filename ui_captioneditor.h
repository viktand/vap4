/********************************************************************************
** Form generated from reading UI file 'captioneditor.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAPTIONEDITOR_H
#define UI_CAPTIONEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_captioneditor
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QGroupBox *groupBox;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QCheckBox *checkBox;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QCheckBox *checkBox_3;

    void setupUi(QDialog *captioneditor)
    {
        if (captioneditor->objectName().isEmpty())
            captioneditor->setObjectName(QStringLiteral("captioneditor"));
        captioneditor->resize(524, 254);
        captioneditor->setMinimumSize(QSize(396, 254));
        captioneditor->setMaximumSize(QSize(524, 254));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icon"), QSize(), QIcon::Normal, QIcon::Off);
        captioneditor->setWindowIcon(icon);
        lineEdit = new QLineEdit(captioneditor);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 40, 501, 23));
        label = new QLabel(captioneditor);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 191, 16));
        groupBox = new QGroupBox(captioneditor);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 70, 501, 171));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(340, 50, 151, 23));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 50, 151, 23));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(175, 50, 151, 23));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 20, 371, 21));
        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(330, 140, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 110, 181, 23));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(10, 140, 181, 23));
        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(10, 80, 401, 21));

        retranslateUi(captioneditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), captioneditor, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), captioneditor, SLOT(reject()));

        QMetaObject::connectSlotsByName(captioneditor);
    } // setupUi

    void retranslateUi(QDialog *captioneditor)
    {
        captioneditor->setWindowTitle(QApplication::translate("captioneditor", "Caption editor", 0));
        label->setText(QApplication::translate("captioneditor", "Caption:", 0));
        groupBox->setTitle(QApplication::translate("captioneditor", "Settings", 0));
        pushButton_3->setText(QApplication::translate("captioneditor", "Font setting", 0));
        pushButton->setText(QApplication::translate("captioneditor", "Back color", 0));
        pushButton_2->setText(QApplication::translate("captioneditor", "Font color", 0));
        checkBox->setText(QApplication::translate("captioneditor", "Show this caption", 0));
        pushButton_4->setText(QApplication::translate("captioneditor", "File name", 0));
        pushButton_5->setText(QApplication::translate("captioneditor", "Full file name", 0));
        checkBox_3->setText(QApplication::translate("captioneditor", "Transparent background", 0));
    } // retranslateUi

};

namespace Ui {
    class captioneditor: public Ui_captioneditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAPTIONEDITOR_H
