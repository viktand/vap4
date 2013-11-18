#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T22:07:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  DEFINES += HAVE_QT5
}

greaterThan(QT_MAJOR_VERSION, 5) {
  DEFINES += HAVE_QT4
}

#ifdef HAVE_QT5
    qtHaveModule(printsupport): QT += printsupport
#endif


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    captioneditor.cpp

HEADERS  += mainwindow.h \
    pagesetup.h \
    avLabel.h \
    userlayout.h \
    integro.h \
    dialog_paper.h \
    about.h \
    captioneditor.h

FORMS    += mainwindow.ui \
    pagesetup.ui \
    userlayout.ui \
    dialog_paper.ui \
    about.ui \
    captioneditor.ui

RESOURCES += \
    res.qrc
