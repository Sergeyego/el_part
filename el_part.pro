#-------------------------------------------------
#
# Project created by QtCreator 2020-02-07T08:10:42
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = el_part
TEMPLATE = app

RC_FILE = ico.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwidget.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    models.cpp \
    rels.cpp \
    dialogloadchem.cpp \
    modelro.cpp \
    dialogtmp.cpp

HEADERS += \
        mainwidget.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    models.h \
    rels.h \
    dialogloadchem.h \
    modelro.h \
    dialogtmp.h

FORMS += \
        mainwidget.ui \
    db/dblogin.ui \
    dialogloadchem.ui \
    dialogtmp.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    ico.rc
