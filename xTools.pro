QT       += core gui widgets svg

CONFIG += c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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

#------------------------------------------------------------------------------
# pri file
include(qmake/git.pri)


#------------------------------------------------------------------------------
# Git env
# tmp = $$system(git log -1 --format=%cd)
# DEFINES += X_GIT_COMMIT_TIME=\\\"$$tmp\\\"
# tmp = $$system(git log -1 --format=%cd)
# DEFINES += X_GIT_COMMIT_TIME=\"\\\"$${tmp}\\\"\"
# DEFINES += X_GIT_COMMIT_TIME=\\\"$$tmp\\\"
# message("[xTools.git]X_GIT_COMMIT_TIME:$$tmp")

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
  X_DISABLE_GLOG \
  X_DISABLE_MDNS \
  X_DISABLE_QR_CODE

# --------------------------------------------------------------------------------------------------
# Remove mdns files
TEMP_FILES = $$files(src/tools/mdns/*.*)
for(f, TEMP_FILES): {
  X_H_FILES -= $${f}
  X_CPP_FILES -= $${f}
  X_UI_FILES -= $${f}
}

# --------------------------------------------------------------------------------------------------
# Remove QR code files
TEMP_FILES = $$files(src/tools/qrcode/*.*)
for(f, TEMP_FILES): {
  X_H_FILES -= $${f}
  X_CPP_FILES -= $${f}
  X_UI_FILES -= $${f}
}

# --------------------------------------------------------------------------------------------------
# SerialPort module
qtHaveModule(serialport): {
  QT += serialport
  DEFINES += X_ENABLE_SERIAL_PORT
} else: {
  TEMP_FILES = $$files(src/*serialport*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $${f}
    X_CPP_FILES -= $${f}
    X_UI_FILES -= $${f}
  }
}

# --------------------------------------------------------------------------------------------------
# SerialPort module
qtHaveModule(websockets): {
  QT += websockets
  DEFINES += X_ENABLE_WEB_SOCKET
} else: {
  TEMP_FILES = $$files(src/*websocket*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $${f}
    X_CPP_FILES -= $${f}
    X_UI_FILES -= $${f}
  }
}

# --------------------------------------------------------------------------------------------------
# Bluetooth module(qt6.8.0 or later)
isEqual(QT_MAJOR_VERSION, 6): {
  greaterThan(QT_MAJOR_VERSION, 7): {
    qtHaveModule(bluetooth): {
      QT += bluetooth
      DEFINES += X_ENABLE_BLUETOOTH
    }
  }
}
!contains(bluetooth, QT): {
  TEMP_FILES = $$files(src/*ble*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $${f}
    X_CPP_FILES -= $${f}
    X_UI_FILES -= $${f}
  }
}

# --------------------------------------------------------------------------------------------------
# Serialbus module
qtHaveModule(serialbus): {
  QT += serialbus
  DEFINES += X_ENABLE_SERIALBUS
} else: {
  TEMP_FILES = $$files(src/tools/modbus/*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $${f}
    X_CPP_FILES -= $${f}
    X_UI_FILES -= $${f}
  }

  TEMP_FILES = $$files(src/tools/canbus/*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $${f}
    X_CPP_FILES -= $${f}
    X_UI_FILES -= $${f}
  }
}

# --------------------------------------------------------------------------------------------------
# Charts module
# qtHaveModule(charts): {
#   QT += charts
#   DEFINES += X_ENABLE_CHARTS
# } else: {
#   TEMP_FILES = $$files(src/page/charts/*.*, true)
#   for(f, TEMP_FILES): {
#     X_H_FILES -= $${f}
#     X_CPP_FILES -= $${f}
#     X_UI_FILES -= $${f}
#   }
# }
TEMP_FILES = $$files(src/page/charts/*.*, true)
for(f, TEMP_FILES): {
  X_H_FILES -= $${f}
  X_CPP_FILES -= $${f}
  X_UI_FILES -= $${f}
}

# --------------------------------------------------------------------------------------------------
# The used files of the project
INCLUDEPATH += src
RESOURCES += xTools.qrc
HEADERS += $${X_H_FILES}
SOURCES += $${X_CPP_FILES}
FORMS += $${X_UI_FILES}
