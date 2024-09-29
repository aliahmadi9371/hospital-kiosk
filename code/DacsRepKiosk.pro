#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T19:10:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DacsRepKiosk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwidget.cpp \
    pdfpreview.cpp \

HEADERS  += mainwidget.h \
    pdfpreview.h

FORMS    += mainwidget.ui \
    pdfpreview.ui

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/release$
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/debu$
else:unix: LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lpoppler-qt5

INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
INCLUDEPATH += /usr/include/poppler/qt5
DEPENDPATH += /usr/lib/arm-linux-gnueabihf

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/release$
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/debu$
else:unix: LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lpoppler-qt5

INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
DEPENDPATH += /usr/lib/arm-linux-gnueabihf

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/arm-linux-gnuea$
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/arm-linux-gn$
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/arm$
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/arm-l$
else:unix: PRE_TARGETDEPS += /usr/lib/arm-linux-gnueabihf/libpoppler-qt5.a

INCLUDEPATH += /usr/local/share/fonts


