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
DEFINES+=MAJOR_VERSION=9
DEFINES+=MINOR_VERSION=9
DEFINES+=MICRO_VERSION=9
DEFINES+=VERSION=\"\\\"9.9.9\\\"\"
DEFINES+=HAVE_LIBPTHREAD=0

INCLUDEPATH += $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip
HEADERS += \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrencode.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrinput.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/bitstream.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrspec.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/rsecc.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/split.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mask.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mqrspec.h \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mmask.h
SOURCES += \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrencode.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrinput.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/bitstream.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/qrspec.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/rsecc.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/split.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mask.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mqrspec.c \
  $${_PRO_FILE_PWD_}/3rd/$$libqrencode_zip/mmask.c

