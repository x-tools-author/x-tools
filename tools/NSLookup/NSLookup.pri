#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/src/nslookup.h

SOURCES += \
    $$PWD/src/nslookup.cpp

FORMS   += \
    $$PWD/src/nslookup.ui

INCLUDEPATH += \
    $$PWD/src/
