# You should download linuxdeployqt from https://github.com/probonopd/linuxdeployqt/releases then rename it to "linuxdeployqt" and add it to path.
# Also, You should download appimagetool from https://github.com/AppImage/AppImageKit/releases then rename it to "appimagetool" and add it to path.
# They (linuxdeployqt and appimagetool) must be know by QtCreator if you use QtCreator to build the project.
# The script will be executed after adding qmake parameter CONFIG+=SAKLinuxDeployQt.
SAKLinuxDeployQt {
    !exists($${OUT_PWD}/qsak/share/applications/QtSwissArmyKnife.desktop) {
        QMAKE_POST_LINK += 'cp -r $$PWD/QtSwissArmyKnife $${OUT_PWD}/qsak $$escape_expand(\\n\\t)'
    }

    !exists($${OUT_PWD}/bin/$$TARGET) {
        QMAKE_POST_LINK += 'mkdir qsak/bin $$escape_expand(\\n\\t)'
        QMAKE_POST_LINK += 'cp $${OUT_PWD}/$$TARGET $${OUT_PWD}/qsak/bin $$escape_expand(\\n\\t)'
    }

    QMAKE_POST_LINK += 'linuxdeployqt $${OUT_PWD}/qsak/share/applications/QtSwissArmyKnife.desktop -verbose=2 -appimage $$escape_expand(\\n\\t)'
}
