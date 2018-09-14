!contains(QT, network){
greaterThan(QT_MAJOR_VERSION, 4): QT += network
}

#-------------------------------------------------------------
#域名解释小工具
INCLUDEPATH += src/Tools/nslookup
HEADERS += \
    src/Tools/nslookup/nslookup.h
SOURCES += \
    src/Tools/nslookup/nslookup.cpp
FORMS   += \
    src/Tools/nslookup/nslookup.ui
