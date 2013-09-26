#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:07:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VAP4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pagesetup.cpp \
    setting.cpp \
    avLabel.cpp \
    userlayout.cpp

HEADERS  += mainwindow.h \
    pagesetup.h \
    setting.h \
    avLabel.h \
    userlayout.h

FORMS    += mainwindow.ui \
    pagesetup.ui \
    setting.ui \
    userlayout.ui

RESOURCES += \
    res.qrc
