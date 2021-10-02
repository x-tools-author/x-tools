include($$PWD/../../libs/windows_openssl/SAKWindowsOpenSSL.pri)

# The file(SAKDefaultConfigure.pri) is use to control which module will be compiled.
# It is not suggested to modify the file directly.
# You should copy the file and rename it to SAKCustomConfigure.pri, then modify it.
exists($$PWD/SAKCustomConfigure.pri){
    include($$PWD/SAKCustomConfigure.pri)
}else{
    include($$PWD/SAKDefaultConfigure.pri)
}
