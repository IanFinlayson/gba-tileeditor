# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set options
CONFIG += qt debug

# set the QT modules we need
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# build an application
TARGET = gba-tileeditor
TEMPLATE = app

# specify all the files we need
SOURCES = main.cpp editorwindow.cpp newdialog.cpp paletteview.cpp mapview.cpp \
          map.cpp
HEADERS = editorwindow.h newdialog.h paletteview.h mapview.h map.h
FORMS = mainwindow.ui newmap.ui
RESOURCES = icons.qrc

