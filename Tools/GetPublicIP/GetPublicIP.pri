#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
# 这个工具的原理是读取别人家的网页数据，如果网页失效，该工具将会失效，可能会失效
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/Src/GetPublicIPThread.h \
    $$PWD/Src/GetPublicIPWidget.h

SOURCES += \
    $$PWD/Src/GetPublicIPThread.cpp \
    $$PWD/Src/GetPublicIPWidget.cpp

FORMS   += \
    $$PWD/Src/GetPublicIPWidget.ui

INCLUDEPATH += \
    $$PWD/Src/
