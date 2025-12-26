if(QT_VERSION VERSION_LESS "5.9.0")
  add_compile_definitions(X_DISABLE_LOG)
  return()
endif()

# https://github.com/google/glog
set(glog_package_name "glog-0.7.1")
x_auto_import_package(glog-0.7.1 glog glog_internal)
list(APPEND X_LIBS glog::glog)
