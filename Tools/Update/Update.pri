#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/src/CheckForUpdateDialog.h \
    $$PWD/src/ConfirmUpdateDialog.h \
    $$PWD/src/DownloadDialog.h \
    $$PWD/src/HttpAnalyzer.h \
    $$PWD/src/HttpDownloader.h \
    $$PWD/src/UpdateManager.h

SOURCES += \
    $$PWD/src/CheckForUpdateDialog.cpp \
    $$PWD/src/ConfirmUpdateDialog.cpp \
    $$PWD/src/DownloadDialog.cpp \
    $$PWD/src/HttpAnalyzer.cpp \
    $$PWD/src/HttpDownloader.cpp \
    $$PWD/src/UpdateManager.cpp

FORMS   += \
    $$PWD/src/CheckForUpdateDialog.ui \
    $$PWD/src/ConfirmUpdateDialog.ui \
    $$PWD/src/DownloadDialog.ui

INCLUDEPATH += $$PWD/src
