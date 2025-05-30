mdns_zip = qmdnsengine-master
!exists($${_PRO_FILE_PWD_}/3rd/$$mdns_zip/CMakeLists.txt): {
  zip_file = $${_PRO_FILE_PWD_}/3rd/$${mdns_zip}.zip
  win32: {
    system(powershell -Command "Expand-Archive -Path '$$zip_file' -DestinationPath '$${_PRO_FILE_PWD_}/3rd/'")
  } else: {
    system(unzip -o $$zip_file -d $${_PRO_FILE_PWD_}/3rd/)
  }
}

TEMP_H_FILES = $$files($${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/*.h, true)
TEMP_CPP_FILES = $$files($${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/*.cpp, true)
TEMP_UI_FILES = $$files($${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/*.ui, true)

# Create an empty file: $${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/qmdnsengine_export.h
!exists($${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/qmdnsengine_export.h): {
  src_file = $${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/qmdnsengine_export.h.in
  dst_file = $${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/qmdnsengine_export.h
  old_text = 'cmakedefine BUILD_SHARED_LIBS'
  new_text = 'define X_TOOLS_GENERATE_WITH_QMAKE'
  win32: {
    system(copy $$replace(src_file, /, \\) $$replace(dst_file, /, \\))
    system(powershell -Command "\"(Get-Content \'$$dst_file\') -replace \'$$old_text\', \'$$new_text\' | Set-Content \'$$dst_file\'\"")
  } else: {
    system(copy $$src_file $$dst_file)
    system(sed -i "s|$$old_text|$$new_text|g" $$dst_file)
  }
}

INCLUDEPATH += \
  $${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src/include \
  $${_PRO_FILE_PWD_}/3rd/$$mdns_zip/src

HEADERS += $$TEMP_H_FILES
SOURCES += $$TEMP_CPP_FILES
FORMS += $$TEMP_UI_FILES
