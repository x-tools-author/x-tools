#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/src/UpdateDialog.h

SOURCES += \
    $$PWD/src/UpdateDialog.cpp

FORMS   += \
    $$PWD/src/UpdateDialog.ui

INCLUDEPATH += $$PWD/src

include($${PWD}/src/http/http.pri)
