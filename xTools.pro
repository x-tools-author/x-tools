QT       += core gui widgets svg network

CONFIG += c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# --------------------------------------------------------------------------------------------------
# Output directory
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

# --------------------------------------------------------------------------------------------------
# Configuration of Windows
win32: msvc: lessThan(QT_MAJOR_VERSION, 6) {
  QMAKE_CXXFLAGS += -execution-charset:utf-8
}

# --------------------------------------------------------------------------------------------------
# Get all source files in src directory
X_H_FILES = $$files(src/*.h, true)
X_CPP_FILES = $$files(src/*.cpp, true)
X_UI_FILES = $$files(src/*.ui, true)

# --------------------------------------------------------------------------------------------------
# pri file
include(qmake/git.pri)
include(qmake/qxlsx.pri)
include(qmake/xtools.pri)

# 3rd module
DEFINES += X_DISABLE_GLOG # QMake is not supported by glog, so disable it
# DEFINES += X_DISABLE_MDNS
# DEFINES += X_DISABLE_QR_CODE

# Remove QR code files
contains(DEFINES, X_DISABLE_QR_CODE) {
  TEMP_FILES = $$files(src/tools/qrcode/*.*)
  for(f, TEMP_FILES): {
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
  }
} else {
  include(qmake/libqrencode.pri)
}

# Remove mdns files
contains(DEFINES, X_DISABLE_MDNS) {
  TEMP_FILES = $$files(src/tools/mdns/*.*)
  for(f, TEMP_FILES): {
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
  }
} else {
  include(qmake/qmdnsengine.pri)
}

# --------------------------------------------------------------------------------------------------
# Git env
tmp = $$x_git_get_latest_tag()
version_tmp = $$tmp
DEFINES += X_LATEST_GIT_TAG=\\\"$$tmp\\\"
message("[xTools] X_LATEST_GIT_TAG: $$version_tmp")
tmp = $$x_git_get_latest_commit()
DEFINES += X_GIT_COMMIT=\\\"$$tmp\\\"
message("[xTools] X_GIT_COMMIT: $$tmp")
tmp = $$x_git_get_latest_commit_time()
DEFINES += X_GIT_COMMIT_TIME=\\\"$$tmp\\\"
message("[xTools] X_GIT_COMMIT_TIME: $$tmp")

# --------------------------------------------------------------------------------------------------
# Application settings(RC file)
win32 {
  QMAKE_TARGET_COMPANY            = "xTools"
  QMAKE_TARGET_DESCRIPTION        = "xTools(https://github.com/x-tools-author/x-tools)"
  QMAKE_TARGET_COPYRIGHT          = "Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved."
  QMAKE_TARGET_PRODUCT            = "xTools"
  QMAKE_TARGET_ORIGINAL_FILENAME  = "xTools"
  QMAKE_TARGET_INTERNALNAME       = "xTools"
  QMAKE_TARGET_COMMENTS           = "xTools"
  QMAKE_TARGET_TRADEMARKS         = "xTools"
  QMAKE_MANIFEST                  =
  RC_CODEPAGE                     =
  RC_ICONS                        = xTools.ico
  RC_LANG                         =
  VERSION                         = $$version_tmp
}

# --------------------------------------------------------------------------------------------------
# I18N
TRANSLATIONS  += \
    res/translations/xTools_en.ts \
    res/translations/xTools_zh_CN.ts

# --------------------------------------------------------------------------------------------------
# SerialPort module
qtHaveModule(serialport): {
  QT += serialport
  DEFINES += X_ENABLE_SERIAL_PORT
} else: {
  TEMP_FILES = $$files(src/*serialport*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
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
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
  }
}

# --------------------------------------------------------------------------------------------------
# Bluetooth module(qt6.8.0 or later)
greaterThan(QT_MAJOR_VERSION, 5): {
  greaterThan(QT_MINOR_VERSION, 7): {
    qtHaveModule(bluetooth): {
      QT += bluetooth
      DEFINES += X_ENABLE_BLUETOOTH
    }
  }
}

!contains(QT, bluetooth): {
  TEMP_FILES = $$files(src/ble*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
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
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
  }

  TEMP_FILES = $$files(src/tools/canbus/*.*, true)
  for(f, TEMP_FILES): {
    X_H_FILES -= $$f
    X_CPP_FILES -= $$f
    X_UI_FILES -= $$f
  }
}

# --------------------------------------------------------------------------------------------------
# Charts module(Qt 6.5.0 or later))
exists(3rd/$$qxlsx_zip/QXlsx/QXlsx.pri): {
  greaterThan(QT_MAJOR_VERSION, 5): {
    greaterThan(QT_MINOR_VERSION, 4): {
      qtHaveModule(charts): {
        QT += charts
        DEFINES += X_ENABLE_CHARTS
        include(3rd/$$qxlsx_zip/QXlsx/QXlsx.pri)
      }
    }
  }

  !contains(QT, charts): {
    TEMP_FILES = $$files(src/page/charts/*.*, true)
    for(f, TEMP_FILES): {
      X_H_FILES -= $$f
      X_CPP_FILES -= $$f
      X_UI_FILES -= $$f
    }
  }
}

# --------------------------------------------------------------------------------------------------
# The used files of the project
INCLUDEPATH += src
RESOURCES += xTools.qrc
HEADERS += $${X_H_FILES}
SOURCES += $${X_CPP_FILES}
FORMS += $${X_UI_FILES}
