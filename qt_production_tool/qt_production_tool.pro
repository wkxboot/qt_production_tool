#-------------------------------------------------
#
# Project created by QtCreator 2019-04-30T10:10:26
#
#-------------------------------------------------
VERSION = 1.0.0

QMAKE_TARGET_PRODUCT = production tool
QMAKE_TARGET_COMPANY = hongmei
QMAKE_TARGET_DESCRIPTION = production tool
QMAKE_TARGET_COPYRIGHT = 2019@copyright

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_bm_production_tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        about_dialog.cpp \
        data_base.cpp \
        jlink_tool.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        about_dialog.h \
        data_base.h \
        jlink_arm.h \
        jlink_tool.h \
        mainwindow.h

FORMS += \
        about_dialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
