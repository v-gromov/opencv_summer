#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T18:37:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_summer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    find_face_and_eyes.cpp

HEADERS  += mainwindow.h \
    find_face_and_eyes.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_core -lopencv_face -lopencv_imgproc -lopencv_objdetect -lopencv_videoio
