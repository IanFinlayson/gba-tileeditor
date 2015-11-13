# set the compiler to clang
QMAKE_CXX = clang++
QMAKE_LINK = clang++

# set options
CONFIG += qt debug

# specify icons
#ICON = tetra.icns

# set the QT modules we need
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# build an application
TARGET = gba-tileeditor
TEMPLATE = app

# specify all the files we need
SOURCES = main.cpp
HEADERS = main.h paletteview.h mapview.h
FORMS = mainwindow.ui
RESOURCES = icons.qrc

