QMAKE_POST_LINK += @echo "." && @echo ".." && @echo "..." && @echo "...." && @echo "....." && @echo "......" && @echo "Copying files...."
win32{
    CONFIG(debug, debug|release) {
        QMAKE_POST_LINK += && $$[QT_INSTALL_BINS]/windeployqt.exe --no-compiler-runtime $$OUT_PWD/debug/$$TARGET.exe
    } else {
        QMAKE_POST_LINK += && $$[QT_INSTALL_BINS]/windeployqt.exe --no-compiler-runtime $$OUT_PWD/release/$$TARGET.exe
    }
}

