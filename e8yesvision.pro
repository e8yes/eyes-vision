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
    test/testrunner.cpp \
    src/reconstructor.cpp \
    src/objectextractor.cpp \
    src/calibrator.cpp \
    src/camera.cpp \
    src/stereo.cpp \
    test/testextractor.cpp \
    src/util.cpp

HEADERS  += src/mainwindow.h \
    test/test.h \
    test/testrunner.h \
    src/reconstructor.h \
    src/objectextractor.h \
    src/calibrator.h \
    src/camera.h \
    src/stereo.h \
    test/testextractor.h \
    src/util.h

LIBS += -lopencv_core
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui

FORMS    += src/mainwindow.ui
