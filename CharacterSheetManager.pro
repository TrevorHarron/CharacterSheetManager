#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T14:12:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CharacterSheetManager
TEMPLATE = app


SOURCES += main.cpp\
    character.cpp \
    dndcharacter.cpp \
    characterfactory.cpp \
    dhcharacter.cpp \
    charactermemento.cpp \
    tabinstance.cpp \
    mainwindow.cpp

HEADERS  += \
    character.h \
    dndcharacter.h \
    characterfactory.h \
    dhcharacter.h \
    charactermemento.h \
    tabinstance.h \
    mainwindow.h

FORMS    += characterwindow.ui
