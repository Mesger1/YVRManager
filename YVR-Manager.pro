QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_FILE = yvrmanager.rc
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    memmap.cpp \
    tcppoller.cpp \
    udppoller.cpp \
    workerthread.cpp

HEADERS += \
    defines.h \
    mainwindow.h \
    memmap.h \
    tcppoller.h \
    udppoller.h \
    workerthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

DISTFILES += \
    stylesheet.qss

RESOURCES += \
    iconsandstylesheets.qrc
