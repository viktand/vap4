/********************************************************************************
** Form generated from reading UI file 'qdeformation.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDEFORMATION_H
#define UI_QDEFORMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Qdeformation
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_3;
    QPushButton *pushButton_2;
    QFrame *line;
    QLabel *label_4;
    QPushButton *pushButton_3;
    QFrame *line_2;
    QLabel *label_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_6;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QFrame *line_3;
    QLabel *label_2;
    QPushButton *pushButton_10;

    void setupUi(QDialog *Qdeformation)
    {
        if (Qdeformation->objectName().isEmpty())
            Qdeformation->setObjectName(QStringLiteral("Qdeformation"));
        Qdeformation->resize(649, 566);
        Qdeformation->setMinimumSize(QSize(649, 566));
        Qdeformation->setMaximumSize(QSize(649, 566));
        frame = new QFrame(Qdeformation);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 10, 630, 400));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Plain);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 630, 400));
        label->setFrameShape(QFrame::NoFrame);
        label->setScaledContents(false);
        pushButton = new QPushButton(Qdeformation);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(540, 530, 101, 27));
        checkBox = new QCheckBox(Qdeformation);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(30, 420, 151, 22));
        checkBox->setChecked(false);
        doubleSpinBox = new QDoubleSpinBox(Qdeformation);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(30, 480, 67, 27));
        doubleSpinBox->setSingleStep(0.1);
        doubleSpinBox->setValue(2);
        label_3 = new QLabel(Qdeformation);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 460, 131, 17));
        pushButton_2 = new QPushButton(Qdeformation);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(150, 450, 40, 40));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/hor_rot"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon);
        pushButton_2->setIconSize(QSize(32, 32));
        line = new QFrame(Qdeformation);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(140, 420, 3, 85));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label_4 = new QLabel(Qdeformation);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(150, 420, 111, 17));
        pushButton_3 = new QPushButton(Qdeformation);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(200, 450, 40, 40));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/ver_rot"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon1);
        pushButton_3->setIconSize(QSize(32, 32));
        line_2 = new QFrame(Qdeformation);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(270, 420, 3, 85));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(Qdeformation);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(280, 420, 91, 17));
        pushButton_4 = new QPushButton(Qdeformation);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(280, 450, 40, 40));
        pushButton_4->setIcon(icon);
        pushButton_4->setIconSize(QSize(32, 32));
        pushButton_5 = new QPushButton(Qdeformation);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(330, 450, 40, 40));
        pushButton_5->setIcon(icon1);
        pushButton_5->setIconSize(QSize(32, 32));
        pushButton_6 = new QPushButton(Qdeformation);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(380, 450, 40, 40));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/to_mem"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_6->setIcon(icon2);
        pushButton_6->setIconSize(QSize(32, 32));
        pushButton_7 = new QPushButton(Qdeformation);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(430, 450, 40, 40));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/prefix1/from_mem"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_7->setIcon(icon3);
        pushButton_7->setIconSize(QSize(32, 32));
        label_6 = new QLabel(Qdeformation);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 530, 91, 20));
        QPalette palette;
        QBrush brush(QColor(248, 107, 21, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(161, 160, 159, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_6->setPalette(palette);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);
        pushButton_8 = new QPushButton(Qdeformation);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(490, 450, 40, 40));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/new/prefix1/undo"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_8->setIcon(icon4);
        pushButton_8->setIconSize(QSize(32, 32));
        pushButton_9 = new QPushButton(Qdeformation);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(430, 530, 101, 27));
        line_3 = new QFrame(Qdeformation);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(480, 420, 3, 85));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(Qdeformation);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(490, 420, 141, 17));
        pushButton_10 = new QPushButton(Qdeformation);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(540, 450, 40, 40));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/new/prefix1/reset"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_10->setIcon(icon5);
        pushButton_10->setIconSize(QSize(32, 32));

        retranslateUi(Qdeformation);

        QMetaObject::connectSlotsByName(Qdeformation);
    } // setupUi

    void retranslateUi(QDialog *Qdeformation)
    {
        Qdeformation->setWindowTitle(QApplication::translate("Qdeformation", "Deformation", 0));
        label->setText(QString());
        pushButton->setText(QApplication::translate("Qdeformation", "Ok", 0));
        checkBox->setText(QApplication::translate("Qdeformation", "Smoothing", 0));
#ifndef QT_NO_TOOLTIP
        doubleSpinBox->setToolTip(QApplication::translate("Qdeformation", "Change, if smoothing poor", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("Qdeformation", "Filling factor", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_2->setToolTip(QApplication::translate("Qdeformation", "Flip horizontally", 0));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QString());
        label_4->setText(QApplication::translate("Qdeformation", "Picture", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("Qdeformation", "Flip vertically", 0));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QString());
        label_5->setText(QApplication::translate("Qdeformation", "Polygon", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("Qdeformation", "Flip horizontally", 0));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_5->setToolTip(QApplication::translate("Qdeformation", "Flip vertically", 0));
#endif // QT_NO_TOOLTIP
        pushButton_5->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_6->setToolTip(QApplication::translate("Qdeformation", "Save polygon", 0));
#endif // QT_NO_TOOLTIP
        pushButton_6->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButton_7->setToolTip(QApplication::translate("Qdeformation", "Apply saved polygon", 0));
#endif // QT_NO_TOOLTIP
        pushButton_7->setText(QString());
        label_6->setText(QApplication::translate("Qdeformation", "Wait", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_8->setToolTip(QApplication::translate("Qdeformation", "Step back", 0));
#endif // QT_NO_TOOLTIP
        pushButton_8->setText(QString());
        pushButton_9->setText(QApplication::translate("Qdeformation", "Cancel", 0));
        label_2->setText(QApplication::translate("Qdeformation", "Others", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_10->setToolTip(QApplication::translate("Qdeformation", "Reload Image", 0));
#endif // QT_NO_TOOLTIP
        pushButton_10->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Qdeformation: public Ui_Qdeformation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDEFORMATION_H
