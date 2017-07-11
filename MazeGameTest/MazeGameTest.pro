#-------------------------------------------------
#
# Project created by QtCreator 2017-04-11T17:52:00
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = mazegametest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += mazegametest.cpp \
    game.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    game.h
