#-------------------------------------------------
#
# Project created by QtCreator 2016-04-09T15:43:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = QSLAM
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/AStar.cpp \
    src/BFS.cpp \
    src/car.cpp \
    src/DPSearch.cpp \
    src/MapSearch.cpp \
    src/Point.cpp \
    src/PrintCharMap.cpp \
    src/RRT.cpp \
    src/Vehicle.cpp

HEADERS  += src/mainwindow.h \
    src/AStar.h \
    src/BFS.h \
    src/car.h \
    src/DPSearch.h \
    src/MapSearch.h \
    src/Point.h \
    src/PrintCharMap.h \
    src/RRT.h \
    src/Vehicle.h

FORMS    += mainwindow.ui
