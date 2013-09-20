#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T12:05:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Tester
CONFIG   += console link_pkgconfig plugin
CONFIG   -= app_bundle

PKGCONFIG += accounts-qt5 libsignon-qt5

TEMPLATE = app


SOURCES += main.cpp \
    AccountHelper.cpp

HEADERS += \
    AccountHelper.h
