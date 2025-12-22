# https://github.com/google/glog
set(glog_package_name "glog-0.7.1")
x_auto_import_package(glog-0.7.1 glog glog_internal)
list(APPEND X_LIBS glog::glog)
