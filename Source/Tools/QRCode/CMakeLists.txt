if(X_TOOLS_ENABLE_MODULE_QRCODE)
  set(TOOL_OWN_SOURCE "")
  set(ASSISTANT_OWN_LIBS Qt${QT_VERSION_MAJOR}::Widgets qrencode)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)
  x_tools_add_tool_app("QRCode" "QRCodeAssistant")

  set(WITH_TOOLS NO)
  set(WITHOUT_PNG ON)
endif()
