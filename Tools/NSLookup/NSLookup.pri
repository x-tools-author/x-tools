#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/Src/nslookup.h

SOURCES += \
    $$PWD/Src/nslookup.cpp

FORMS   += \
    $$PWD/Src/nslookup.ui

INCLUDEPATH += \
    $$PWD/Src/
