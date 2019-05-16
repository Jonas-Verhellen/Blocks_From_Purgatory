#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T17:36:03
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia #na toeving van deze lijnen -> qmake runnen.
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project_Jonas_Verhellen
TEMPLATE = app


SOURCES += main.cpp\
        tetriswindow.cpp \
    tetrispiece.cpp \
    tetrisboard.cpp \
    savegame.cpp

HEADERS  += tetriswindow.h \
    tetrispiece.h \
    tetrisboard.h \
    savegame.h

FORMS    += tetriswindow.ui
