/********************************************************************************
** Form generated from reading UI file 'refresh.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFRESH_H
#define UI_REFRESH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_refresh
{
public:
    QLabel *label;
    QCheckBox *checkBox;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *refresh)
    {
        if (refresh->objectName().isEmpty())
            refresh->setObjectName(QStringLiteral("refresh"));
        refresh->resize(632, 147);
        refresh->setMinimumSize(QSize(632, 147));
        refresh->setMaximumSize(QSize(632, 147));
        refresh->setModal(true);
        label = new QLabel(refresh);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 30, 591, 17));
        label->setAlignment(Qt::AlignCenter);
        checkBox = new QCheckBox(refresh);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(20, 80, 421, 22));
        label_2 = new QLabel(refresh);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 10, 591, 20));
        label_2->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(refresh);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(410, 110, 99, 27));
        pushButton_2 = new QPushButton(refresh);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(520, 110, 99, 27));

        retranslateUi(refresh);

        QMetaObject::connectSlotsByName(refresh);
    } // setupUi

    void retranslateUi(QDialog *refresh)
    {
        refresh->setWindowTitle(QApplication::translate("refresh", "Update vap version", 0));
        label->setText(QApplication::translate("refresh", "Check for updates can be disabled in the settings.", 0));
        checkBox->setText(QApplication::translate("refresh", "Do not remind me until the new version", 0));
        label_2->setText(QApplication::translate("refresh", "It was found that a new version of vap", 0));
        pushButton->setText(QApplication::translate("refresh", "Download", 0));
        pushButton_2->setText(QApplication::translate("refresh", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class refresh: public Ui_refresh {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFRESH_H
