#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T14:01:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = UnserProjekt
TEMPLATE = app

LIBS += -L/home/vmuser/qtprojects/UnserProjekt -lsrf02

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    srf02.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

target.path = /home/pi/srf02_visualize_from_stdin_pipe
INSTALLS += target

DISTFILES +=

