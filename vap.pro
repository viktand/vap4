#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:07:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4)
{
    qtHaveModule(printsupport): QT += printsupport
    QT += widgets
}

TRANSLATIONS += vap_ru.ts


TARGET = vap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pagesetup.cpp \
    avLabel.cpp \
    userlayout.cpp \
    integro.cpp \
    dialog_paper.cpp \
    about.cpp \
    captioneditor.cpp \
    texteditor.cpp \
    avlistbox.cpp

HEADERS  += mainwindow.h \
    pagesetup.h \
    avLabel.h \
    userlayout.h \
    integro.h \
    dialog_paper.h \
    about.h \
    captioneditor.h \
    texteditor.h \
    avlistbox.h

FORMS    += mainwindow.ui \
    pagesetup.ui \
    userlayout.ui \
    dialog_paper.ui \
    about.ui \
    captioneditor.ui \
    texteditor.ui

RESOURCES += \
    res.qrc
