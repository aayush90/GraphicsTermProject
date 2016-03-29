#-------------------------------------------------
#
# Project created by QtCreator 2016-03-26T16:14:53
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComputerGraphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    illuminationmodel.cpp \
    glwidget.cpp \
    voxelmerge.cpp

HEADERS  += mainwindow.h \
    illuminationmodel.h \
    glwidget.h \
    voxelmerge.h

FORMS    += mainwindow.ui

LIBS += -lGL
LIBS += -lGLU
LIBS += -lglut

CONFIG += c++11
