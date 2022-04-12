exists($$PWD/user/src/SAKUser.pri) {
    #DEFINES+=SAK_IMPORT_MODULE_USER
    #INCLUDEPATH += $$PWD/user/src
    #include($$PWD/user/src/SAKUser.pri)
}
