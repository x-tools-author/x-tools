# * argLupdate: the lupdate executable
# * argSrcDir: source directory
# * argOutDir: output directory
# * argTarget: target name

set(X_LANGUAGES
    "en"
    "zh_CN"
    "zh_TW"
    "ar"
    "cs"
    "da"
    "de"
    "es"
    "fa"
    "fi"
    "fr"
    "he"
    "uk"
    "it"
    "ja"
    "ko"
    "lt"
    "nl"
    "pl"
    "pt"
    "ru"
    "sk"
    "sl"
    "sv")

message(STATUS "[xTools.lupdate] argLupdate: ${argLupdate}")
message(STATUS "[xTools.lupdate] argSrcDir: ${argSrcDir}")
message(STATUS "[xTools.lupdate] argOutDir: ${argOutDir}")
message(STATUS "[xTools.lupdate] argTarget: ${argTarget}")

foreach(lan ${X_LANGUAGES})
  message(STATUS "[xTools.lupdate] Processing language: ${lan}")
  set(ts_file ${argOutDir}/${argTarget}_${lan}.ts)
  execute_process(
    COMMAND ${argLupdate} ${argSrcDir} -ts ${ts_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result_var
    OUTPUT_VARIABLE output_var)
  if(NOT result_var EQUAL 0)
    message(WARNING "[xTools.lupdate] lupdate failed for language: ${lan}")
    message(WARNING "[xTools.lupdate] Output: ${output_var}")
  else()
    message(STATUS "[xTools.lupdate] lupdate succeeded for language: ${lan}")
  endif()
endforeach(lan)
