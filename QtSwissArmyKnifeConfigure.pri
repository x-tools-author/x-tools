#编译目录配置
MOC_DIR             = $$OUT_PWD/moc
OBJECTS_DIR         = $$OUT_PWD/obj
RCC_DIR             = $$OUT_PWD/resource
UI_DIR              = $$OUT_PWD/ui

#release 版本默认舍弃输出信息中的行号等信息，需要加语句保留
DEFINES += QT_MESSAGELOGCONTEXT

win32{
    RC_ICONS = resource/icon/window.ico
}
