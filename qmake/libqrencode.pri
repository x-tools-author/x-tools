libqrencode_zip = libqrencode-master
!exists($${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrencode.h): {
  zip_file = $${_PRO_FILE_PWD_}/3rd/$${libqrencode_zip}.zip
  win32: {
    system(powershell -Command "Expand-Archive -Path '$$zip_file' -DestinationPath '$${_PRO_FILE_PWD_}/3rd/'")
  } else: {
    system(unzip -o $$zip_file -d $${_PRO_FILE_PWD_}/3rd/)
  }
}

DEFINES+=STATIC_IN_RELEASE=static
DEFINES+=MAJOR_VERSION=4
DEFINES+=MINOR_VERSION=1
DEFINES+=MICRO_VERSION=1
DEFINES+=VERSION=\"\\\"4.1.1\\\"\"
DEFINES+=HAVE_LIBPTHREAD=0

INCLUDEPATH += $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip
HEADERS += \
  3rd/$$libqrencode_zip/qrencode.c \
  3rd/$$libqrencode_zip/qrinput.c \
  3rd/$$libqrencode_zip/bitstream.c \
  3rd/$$libqrencode_zip/qrspec.c \
  3rd/$$libqrencode_zip/rsecc.c \
  3rd/$$libqrencode_zip/split.c \
  3rd/$$libqrencode_zip/mask.c \
  3rd/$$libqrencode_zip/mqrspec.c \
  3rd/$$libqrencode_zip/mmask.c
SOURCES += \
  3rd/$$libqrencode_zip/qrencode_inner.h \
  3rd/$$libqrencode_zip/qrinput.h \
  3rd/$$libqrencode_zip/bitstream.h \
  3rd/$$libqrencode_zip/qrspec.h \
  3rd/$$libqrencode_zip/rsecc.h \
  3rd/$$libqrencode_zip/split.h \
  3rd/$$libqrencode_zip/mask.h \
  3rd/$$libqrencode_zip/mqrspec.h \
  3rd/$$libqrencode_zip/mmask.h
