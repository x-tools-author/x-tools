#The simplest way to use windeployqt is to add the bin directory of your Qt
#installation (e.g. <QT_DIR\bin>) to the PATH variable and then run:
#  windeployqt <path-to-app-binary>
#If ICU, ANGLE, etc. are not in the bin directory, they need to be in the PATH
#variable. If your application uses Qt Quick, run:
#  windeployqt --qmldir <path-to-app-qml-files> <path-to-app-binary>
#
#Options:
#  -?, -h, --help            Displays this help.
#  -v, --version             Displays version information.
#  --dir <directory>         Use directory instead of binary directory.
#  --libdir <path>           Copy libraries to path.
#  --plugindir <path>        Copy plugins to path.
#  --debug                   Assume debug binaries.
#  --release                 Assume release binaries.
#  --pdb                     Deploy .pdb files (MSVC).
#  --force                   Force updating files.
#  --dry-run                 Simulation mode. Behave normally, but do not
#                            copy/update any files.
#  --no-patchqt              Do not patch the Qt5Core library.
#  --no-plugins              Skip plugin deployment.
#  --no-libraries            Skip library deployment.
#  --qmldir <directory>      Scan for QML-imports starting from directory.
#  --no-quick-import         Skip deployment of Qt Quick imports.
#  --no-translations         Skip deployment of translations.
#  --no-system-d3d-compiler  Skip deployment of the system D3D compiler.
#  --compiler-runtime        Deploy compiler runtime (Desktop only).
#  --no-compiler-runtime     Do not deploy compiler runtime (Desktop only).
#  --webkit2                 Deployment of WebKit2 (web process).
#  --no-webkit2              Skip deployment of WebKit2.
#  --json                    Print to stdout in JSON format.
#  --angle                   Force deployment of ANGLE.
#  --no-angle                Disable deployment of ANGLE.
#  --no-opengl-sw            Do not deploy the software rasterizer library.
#  --list <option>           Print only the names of the files copied.
#                            Available options:
#                             source:   absolute path of the source files
#                             target:   absolute path of the target files
#                             relative: paths of the target files, relative
#                                       to the target directory
#                             mapping:  outputs the source and the relative
#                                       target, suitable for use within an
#                                       Appx mapping file
#  --verbose <level>         Verbose level (0-2).



# The file was create by Qter(qsaker@qq.com) on September 30, 2019.

# History:
# 20190930  Creating the file.


contains(CONFIG, static){
    # Static compilation does not require deployment
}else{
    # Windows platform(only for x86 architecture)
    win32 {
        DEPLOY_TOOL = $${dirname(QMAKE_QMAKE)}/windeployqt.exe
        DEPLOY_TOOL = $$replace(DEPLOY_TOOL, /, \\)

        contains(CONFIG, debug, debug|release){
            DEPLOY_TARGET = $${OUT_PWD}/debug/$${TARGET}.exe
        }else{
            DEPLOY_TARGET = $${OUT_PWD}/release/$${TARGET}.exe
        }

        DEPLOY_TARGET=$$replace(DEPLOY_TARGET, /, \\)
        msvc {
            QMAKE_POST_LINK+=$${DEPLOY_TOOL} $${DEPLOY_TARGET} --force --no-translations $$escape_expand(\\n)
        }else{
            QMAKE_POST_LINK+='$$escape_expand("\\n\\t") $${DEPLOY_TOOL} $${DEPLOY_TARGET} --force --no-translations $$escape_expand("\\n\\t")'
        }
    }
}
