!contains(QT, serialport){
greaterThan(QT_MAJOR_VERSION, 4): QT += serialport
}

SOURCES += \
    src/SerialPort/SerialportSAKIODeviceWidget.cpp \
    src/SerialPort/SerialportSAKIODevice.cpp \
    src/SerialPort/SerialportSAKIODeviceControler.cpp
HEADERS += \
    src/SerialPort/SerialportSAKIODeviceWidget.h \
    src/SerialPort/SerialportSAKIODevice.h \
    src/SerialPort/SerialportSAKIODeviceControler.h
FORMS   += \
    src/SerialPort/SerialportSAKIODeviceControler.ui
INCLUDEPATH += \
    src/SerialPort
