#-------------------------------------------------
#
# Project created by QtCreator 2012-08-26T01:18:22
#
#-------------------------------------------------

QT       += core gui webkit network

TARGET = Naveo2
TEMPLATE = app

#CONFIG += console
#LIBS += libeay32
#LIBS += libssleay32

INCLUDEPATH += .

RESOURCES += ressources/ressources.qrc

SOURCES += main.cpp\
        nWindow.cpp \
    nNaveoApplication.cpp \
    nTabBar.cpp \
    nWebView.cpp \
    nTheme.cpp \
    nUrlLineEdit.cpp \
    nSearchEngine.cpp \
    nDebugConsole.cpp \
    nDownload.cpp \
    nDownloadWidget.cpp \
    nDownloadManager.cpp

HEADERS  += nWindow.h \
    nNaveoApplication.h \
    nTabBar.h \
    nWebView.h \
    nTheme.h \
    nUrlLineEdit.h \
    nSearchEngine.h \
    nDebugConsole.h \
    nDownload.h \
    nDownloadWidget.h \
    nDownloadManager.h
