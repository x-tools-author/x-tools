android {
    include($$PWD/android/SAKAndroid.pri)
}

win32 {
    include($$PWD/windows/SAKWindows.pri)
}

unix {
    include($$PWD/unix/SAKUnix.pri)
}
