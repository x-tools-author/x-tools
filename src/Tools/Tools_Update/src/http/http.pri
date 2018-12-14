!contains(QT, network){
    QT += network
}

INCLUDEPATH += $${PWD}

HEADERS += $${PWD}/httpwindow.h
SOURCES += $${PWD}/httpwindow.cpp
FORMS   += $${PWD}/authenticationdialog.ui
