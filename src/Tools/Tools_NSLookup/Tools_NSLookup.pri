#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/nslookup.h 

SOURCES += \
    $$PWD/nslookup.cpp

FORMS   += \
    $$PWD/nslookup.ui

INCLUDEPATH += $$PWD
