QT       += core gui widgets svg

CONFIG += c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

warning("CMake is recommended for building this project, but you are using qmake.")
#------------------------------------------------------------------------------
# Android settings
X_APP_NAME        = "xTools"
X_ORG_NAME        = "xTools"
X_ORG_DOMAIN      = "IT"
X_APP_DESCRIPTION = "xTools"
X_APP_COPYRIGHT   = "Copyright 2018-2022 x-tools-author(x-tools@outlook.com). All rights reserved."
X_VERSION         = "1.0.0"
win32 {
    QMAKE_TARGET_COMPANY        = "$${X_ORG_NAME}"
    QMAKE_TARGET_DESCRIPTION    = "$${X_APP_DESCRIPTION}"
    QMAKE_TARGET_COPYRIGHT      = "$${X_APP_COPYRIGHT}"
    QMAKE_TARGET_PRODUCT        = "$${X_APP_NAME}"
    QMAKE_TARGET_VERSION        = "$${X_VERSION}"
}

#--------------------------------------------------------------------------------------------
#Output directory
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

#--------------------------------------------------------------------------------------------
#Configuration of Windows
win32 {
    RC_ICONS = xTools.ico
    msvc:{
        lessThan(QT_MAJOR_VERSION, 6){
            QMAKE_CXXFLAGS += -execution-charset:utf-8
        }
    }
}

#--------------------------------------------------------------------------------------------
#I18N
TRANSLATIONS  += \
    res/translations/xTools_en.ts \
    res/translations/xTools_zh_CN.ts

# --------------------------------------------------------------------------------------------------
# Get all source files in src directory
X_H_FILES = $$files(src/*.h, true)
X_CPP_FILES = $$files(src/*.cpp, true)
X_UI_FILES = $$files(src/*.ui, true)

# --------------------------------------------------------------------------------------------------
# 3rd module
DEFINES += \
  X_DISABLE_GOOGLE_LOG \
  X_DISABLE_MDNS \
  X_DISABLE_QR_CODE

# --------------------------------------------------------------------------------------------------
# SerialPort module
qtHaveModule(serialport): {
  QT += serialport
  DEFINES += X_ENABLE_SERIAL_PORT
} else: {

}

# --------------------------------------------------------------------------------------------------
# SerialPort module
qtHaveModule(websockets): {
  QT += websockets
  DEFINES += X_ENABLE_WEB_SOCKET
} else: {

}

# --------------------------------------------------------------------------------------------------
# Bluetooth module
qtHaveModule(bluetooth): {
  QT += bluetooth
  DEFINES += X_ENABLE_BLUETOOTH
} else: {

}

# --------------------------------------------------------------------------------------------------
# Serialbus module
qtHaveModule(serialbus): {
  QT += serialbus
  DEFINES += X_ENABLE_SERIALBUS
} else: {

}

# --------------------------------------------------------------------------------------------------
# Charts module
qtHaveModule(charts): {
  QT += charts
  DEFINES += X_ENABLE_CHARTS
} else: {

}

# --------------------------------------------------------------------------------------------------
# The used files of the project
INCLUDEPATH += src
RESOURCES += xTools.qrc
HEADERS += $${X_H_FILES}
SOURCES += $${X_CPP_FILES}
FORMS += $${X_UI_FILES}
