#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
# 这个工具的原理是读取别人家的网页数据，如果网页失效，该工具将会失效，可能会失效
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/GetPublicIPThread.h \
    $$PWD/GetPublicIPWidget.h

SOURCES += \
    $$PWD/GetPublicIPThread.cpp \
    $$PWD/GetPublicIPWidget.cpp

FORMS   += \
    $$PWD/GetPublicIPWidget.ui

INCLUDEPATH += $$PWD
