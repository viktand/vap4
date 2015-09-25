/********************************************************************************
** Form generated from reading UI file 'userlayout.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERLAYOUT_H
#define UI_USERLAYOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_userlayout
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *userlayout)
    {
        if (userlayout->objectName().isEmpty())
            userlayout->setObjectName(QStringLiteral("userlayout"));
        userlayout->resize(400, 174);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icon"), QSize(), QIcon::Normal, QIcon::Off);
        userlayout->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(userlayout);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(210, 130, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(userlayout);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 10, 105, 150));
        label->setFrameShape(QFrame::StyledPanel);
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/sheet")));
        label->setScaledContents(true);
        spinBox = new QSpinBox(userlayout);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(339, 12, 48, 24));
        QFont font;
        font.setFamily(QStringLiteral("Ubuntu"));
        spinBox->setFont(font);
        spinBox->setMinimum(1);
        spinBox->setValue(3);
        spinBox_2 = new QSpinBox(userlayout);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(339, 42, 48, 24));
        spinBox_2->setMinimum(1);
        spinBox_2->setValue(3);
        label_2 = new QLabel(userlayout);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(259, 19, 81, 16));
        label_3 = new QLabel(userlayout);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(260, 50, 71, 16));

        retranslateUi(userlayout);
        QObject::connect(buttonBox, SIGNAL(accepted()), userlayout, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), userlayout, SLOT(reject()));

        QMetaObject::connectSlotsByName(userlayout);
    } // setupUi

    void retranslateUi(QDialog *userlayout)
    {
        userlayout->setWindowTitle(QApplication::translate("userlayout", "User layout", 0));
        label->setText(QString());
        label_2->setText(QApplication::translate("userlayout", "In horizontal", 0));
        label_3->setText(QApplication::translate("userlayout", "In vertical", 0));
    } // retranslateUi

};

namespace Ui {
    class userlayout: public Ui_userlayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERLAYOUT_H
