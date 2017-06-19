#-------------------------------------------------
#
# Project created by QtCreator 2017-06-18T16:18:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = e8yesvision
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    test/test.cpp \
    test/testrunner.cpp

HEADERS  += src/mainwindow.h \
    test/test.h \
    test/testrunner.h

FORMS    += src/mainwindow.ui
