#-------------------------------------------------
#
# Project created by QtCreator 2018-03-17T21:01:39
#
#-------------------------------------------------

QT       += core gui network

# 如果存在printsupport模块，则增加打印支持模块
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextEditor
TEMPLATE = app

win32-msvc*:QMAKE_CXXFLAGS += /wd"4100"
include(./../../Path.pri)

INCLUDEPATH += ../../Libraries/qscintilla

LIBS += -L$${DESTDIR} -lqscintilla2_qt5d

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        texteditor.cpp \
    mainwindow.cpp \
    my_plug-in/md5widget.cpp \
    my_plug-in/finddialog.cpp \
    my_plug-in/md5filewidget.cpp

HEADERS += \
        texteditor.h \
    mainwindow.h \
    my_plug-in/md5widget.h \
    my_plug-in/myheaders.h \
    my_plug-in/finddialog.h \
    my_plug-in/md5filewidget.h

FORMS += \
    my_plug-in/md5filewidget.ui
		
RESOURCES += \
    texteditor.qrc
	
# DEFINES += "VERSION=3.0"
	
CONFIG += c++11
