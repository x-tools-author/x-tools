#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T14:00:28
#
#-------------------------------------------------

!contains(QT, network){
    QT  += network
}

HEADERS += \
    $$PWD/Src/CheckForUpdateDialog.h \
    $$PWD/Src/ConfirmUpdateDialog.h \
    $$PWD/Src/DownloadDialog.h \
    $$PWD/Src/HttpAnalyzer.h \
    $$PWD/Src/HttpDownloader.h \
    $$PWD/Src/UpdateManager.h

SOURCES += \
    $$PWD/Src/CheckForUpdateDialog.cpp \
    $$PWD/Src/ConfirmUpdateDialog.cpp \
    $$PWD/Src/DownloadDialog.cpp \
    $$PWD/Src/HttpAnalyzer.cpp \
    $$PWD/Src/HttpDownloader.cpp \
    $$PWD/Src/UpdateManager.cpp

FORMS   += \
    $$PWD/Src/CheckForUpdateDialog.ui \
    $$PWD/Src/ConfirmUpdateDialog.ui \
    $$PWD/Src/DownloadDialog.ui

INCLUDEPATH += $$PWD/Src
