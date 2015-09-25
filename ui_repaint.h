/********************************************************************************
** Form generated from reading UI file 'repaint.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPAINT_H
#define UI_REPAINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QRepaintPixmap
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QSlider *horizontalSlider;
    QLabel *label_2;
    QSlider *horizontalSlider_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QSlider *horizontalSlider_3;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *tab_2;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QSlider *horizontalSlider_4;
    QLabel *label_11;
    QSlider *horizontalSlider_5;
    QLabel *label_12;
    QLabel *label_13;
    QSlider *horizontalSlider_6;
    QWidget *tab_3;
    QSlider *horizontalSlider_7;
    QLabel *label_14;
    QLabel *label_box;
    QDial *dial;
    QSpinBox *spinBox;
    QLabel *label;
    QLabel *label_15;
    QFrame *line;
    QLabel *label_16;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *QRepaintPixmap)
    {
        if (QRepaintPixmap->objectName().isEmpty())
            QRepaintPixmap->setObjectName(QStringLiteral("QRepaintPixmap"));
        QRepaintPixmap->resize(498, 636);
        QRepaintPixmap->setMinimumSize(QSize(498, 636));
        QRepaintPixmap->setMaximumSize(QSize(498, 636));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/icon"), QSize(), QIcon::Normal, QIcon::Off);
        QRepaintPixmap->setWindowIcon(icon);
        tabWidget = new QTabWidget(QRepaintPixmap);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 440, 481, 151));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(130, 10, 301, 29));
        horizontalSlider->setMinimum(-255);
        horizontalSlider->setMaximum(255);
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 10, 111, 20));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_2 = new QSlider(tab);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(130, 40, 301, 29));
        horizontalSlider_2->setMinimum(-359);
        horizontalSlider_2->setMaximum(359);
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 40, 111, 20));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(440, 10, 31, 17));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(440, 40, 31, 17));
        horizontalSlider_3 = new QSlider(tab);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(130, 70, 301, 29));
        horizontalSlider_3->setMinimum(-255);
        horizontalSlider_3->setMaximum(255);
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(440, 70, 31, 17));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 70, 111, 20));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(9, 40, 111, 20));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 10, 111, 20));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 70, 111, 20));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalSlider_4 = new QSlider(tab_2);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setGeometry(QRect(130, 10, 301, 29));
        horizontalSlider_4->setMinimum(-255);
        horizontalSlider_4->setMaximum(255);
        horizontalSlider_4->setOrientation(Qt::Horizontal);
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(440, 40, 31, 17));
        horizontalSlider_5 = new QSlider(tab_2);
        horizontalSlider_5->setObjectName(QStringLiteral("horizontalSlider_5"));
        horizontalSlider_5->setGeometry(QRect(130, 40, 301, 29));
        horizontalSlider_5->setMinimum(-255);
        horizontalSlider_5->setMaximum(255);
        horizontalSlider_5->setOrientation(Qt::Horizontal);
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(440, 70, 31, 17));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(440, 10, 31, 17));
        horizontalSlider_6 = new QSlider(tab_2);
        horizontalSlider_6->setObjectName(QStringLiteral("horizontalSlider_6"));
        horizontalSlider_6->setGeometry(QRect(130, 70, 301, 29));
        horizontalSlider_6->setMinimum(-255);
        horizontalSlider_6->setMaximum(255);
        horizontalSlider_6->setOrientation(Qt::Horizontal);
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalSlider_7 = new QSlider(tab_3);
        horizontalSlider_7->setObjectName(QStringLiteral("horizontalSlider_7"));
        horizontalSlider_7->setGeometry(QRect(160, 10, 241, 29));
        horizontalSlider_7->setMinimum(-255);
        horizontalSlider_7->setMaximum(255);
        horizontalSlider_7->setOrientation(Qt::Horizontal);
        label_14 = new QLabel(tab_3);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(420, 10, 41, 17));
        label_box = new QLabel(tab_3);
        label_box->setObjectName(QStringLiteral("label_box"));
        label_box->setGeometry(QRect(390, 50, 71, 21));
        label_box->setFrameShape(QFrame::Box);
        label_box->setAlignment(Qt::AlignCenter);
        dial = new QDial(tab_3);
        dial->setObjectName(QStringLiteral("dial"));
        dial->setGeometry(QRect(250, 70, 51, 41));
        dial->setValue(5);
        spinBox = new QSpinBox(tab_3);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(190, 80, 61, 27));
        spinBox->setValue(5);
        label = new QLabel(tab_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 80, 171, 20));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(tab_3);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 50, 371, 17));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line = new QFrame(tab_3);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 34, 461, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_16 = new QLabel(tab_3);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(10, 10, 141, 17));
        pushButton_3 = new QPushButton(tab_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(false);
        pushButton_3->setGeometry(QRect(420, 90, 41, 27));
        tabWidget->addTab(tab_3, QString());
        pushButton = new QPushButton(QRepaintPixmap);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 600, 101, 27));
        pushButton_2 = new QPushButton(QRepaintPixmap);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(390, 600, 101, 27));

        retranslateUi(QRepaintPixmap);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(QRepaintPixmap);
    } // setupUi

    void retranslateUi(QDialog *QRepaintPixmap)
    {
        QRepaintPixmap->setWindowTitle(QApplication::translate("QRepaintPixmap", "Repaint", 0));
        label_2->setText(QApplication::translate("QRepaintPixmap", "Brightness", 0));
        label_3->setText(QApplication::translate("QRepaintPixmap", "Hue", 0));
        label_4->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        label_5->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        label_6->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        label_7->setText(QApplication::translate("QRepaintPixmap", "Saturation", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QRepaintPixmap", "HSV", 0));
        label_8->setText(QApplication::translate("QRepaintPixmap", "Green", 0));
        label_9->setText(QApplication::translate("QRepaintPixmap", "Red", 0));
        label_10->setText(QApplication::translate("QRepaintPixmap", "Blue", 0));
        label_11->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        label_12->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        label_13->setText(QApplication::translate("QRepaintPixmap", "0", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QRepaintPixmap", "RGB", 0));
        label_14->setText(QApplication::translate("QRepaintPixmap", "0", 0));
#ifndef QT_NO_TOOLTIP
        label_box->setToolTip(QApplication::translate("QRepaintPixmap", "Transparent color", 0));
#endif // QT_NO_TOOLTIP
        label_box->setText(QApplication::translate("QRepaintPixmap", "no", 0));
#ifndef QT_NO_TOOLTIP
        dial->setToolTip(QApplication::translate("QRepaintPixmap", "Accuracy of the transparent color selection", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        spinBox->setToolTip(QApplication::translate("QRepaintPixmap", "Accuracy of the transparent color selection", 0));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("QRepaintPixmap", "Tolerance color, %", 0));
        label_15->setText(QApplication::translate("QRepaintPixmap", "Click on the image to assign a transparent color:", 0));
        label_16->setText(QApplication::translate("QRepaintPixmap", "Total transparency", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("QRepaintPixmap", "Cancel one last step", 0));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QApplication::translate("QRepaintPixmap", "<<", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("QRepaintPixmap", "Transparency", 0));
        pushButton->setText(QApplication::translate("QRepaintPixmap", "Reset", 0));
        pushButton_2->setText(QApplication::translate("QRepaintPixmap", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class QRepaintPixmap: public Ui_QRepaintPixmap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPAINT_H
