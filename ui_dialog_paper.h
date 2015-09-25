/********************************************************************************
** Form generated from reading UI file 'dialog_paper.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_PAPER_H
#define UI_DIALOG_PAPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_paper
{
public:
    QPushButton *pushButton_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_6;
    QListWidget *listWidget;
    QListWidget *listWidget_2;

    void setupUi(QDialog *Dialog_paper)
    {
        if (Dialog_paper->objectName().isEmpty())
            Dialog_paper->setObjectName(QStringLiteral("Dialog_paper"));
        Dialog_paper->resize(606, 442);
        QFont font;
        font.setPointSize(10);
        Dialog_paper->setFont(font);
        pushButton_2 = new QPushButton(Dialog_paper);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(510, 410, 81, 23));
        label = new QLabel(Dialog_paper);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 241, 16));
        label_2 = new QLabel(Dialog_paper);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(340, 30, 251, 16));
        pushButton_3 = new QPushButton(Dialog_paper);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 340, 251, 23));
        pushButton_4 = new QPushButton(Dialog_paper);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(280, 130, 41, 23));
        pushButton_5 = new QPushButton(Dialog_paper);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(280, 160, 41, 23));
        label_3 = new QLabel(Dialog_paper);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 370, 451, 16));
        lineEdit = new QLineEdit(Dialog_paper);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 390, 301, 23));
        pushButton = new QPushButton(Dialog_paper);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(320, 390, 141, 23));
        pushButton_6 = new QPushButton(Dialog_paper);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(280, 190, 41, 23));
        listWidget = new QListWidget(Dialog_paper);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 50, 256, 281));
        listWidget_2 = new QListWidget(Dialog_paper);
        listWidget_2->setObjectName(QStringLiteral("listWidget_2"));
        listWidget_2->setGeometry(QRect(330, 50, 261, 281));

        retranslateUi(Dialog_paper);

        QMetaObject::connectSlotsByName(Dialog_paper);
    } // setupUi

    void retranslateUi(QDialog *Dialog_paper)
    {
        Dialog_paper->setWindowTitle(QApplication::translate("Dialog_paper", "Paper size editor", 0));
        pushButton_2->setText(QApplication::translate("Dialog_paper", "Ok", 0));
        label->setText(QApplication::translate("Dialog_paper", "Possible options for size:", 0));
        label_2->setText(QApplication::translate("Dialog_paper", "Preferred sizes:", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("Dialog_paper", "new type", 0));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QApplication::translate("Dialog_paper", "Add new", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("Dialog_paper", "Add to list", 0));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QApplication::translate("Dialog_paper", ">>", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_5->setToolTip(QApplication::translate("Dialog_paper", "Remove from list", 0));
#endif // QT_NO_TOOLTIP
        pushButton_5->setText(QApplication::translate("Dialog_paper", "<<", 0));
        label_3->setText(QApplication::translate("Dialog_paper", "Enter the new size as \"name: width x height mm\"", 0));
#ifndef QT_NO_TOOLTIP
        pushButton->setToolTip(QApplication::translate("Dialog_paper", "Save this size", 0));
#endif // QT_NO_TOOLTIP
        pushButton->setText(QApplication::translate("Dialog_paper", "Save", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_6->setToolTip(QApplication::translate("Dialog_paper", "Delete", 0));
#endif // QT_NO_TOOLTIP
        pushButton_6->setText(QApplication::translate("Dialog_paper", "X", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog_paper: public Ui_Dialog_paper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_PAPER_H
