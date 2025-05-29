qxlsx_zip = QXlsx-1.4.7
pri_file = $${_PRO_FILE_PWD_}/3rd/$$qxlsx_zip/QXlsx/QXlsx.pri
exists($$pri_file): {
  message("QXlsx already exists, skipping extracting.")
} else: {
  zip_file = $${_PRO_FILE_PWD_}/3rd/$${qxlsx_zip}.zip
  win32: {
    system(powershell -Command "Expand-Archive -Path '$$zip_file' -DestinationPath '$${_PRO_FILE_PWD_}/3rd/'")
  } else: {
    system(unzip -o $$zip_file -d $${_PRO_FILE_PWD_}/3rd/)
  }

  message("Extracting QXlsx from $$zip_file to $${_PRO_FILE_PWD_}/3rd/$${qxlsx_zip}"))
}
