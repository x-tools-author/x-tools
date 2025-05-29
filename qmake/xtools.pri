# Usage: windeployqt [options] [files]
# Qt Deploy Tool 6.8.3

# The simplest way to use windeployqt is to add the bin directory of your Qt
# installation (e.g. <QT_DIR\bin>) to the PATH variable and then run:
#   windeployqt <path-to-app-binary>

# If your application uses Qt Quick, run:
#   windeployqt --qmldir <path-to-app-qml-files> <path-to-app-binary>

# Options:
#   -?, -h, --help                            Displays help on commandline
#                                             options.
#   --help-all                                Displays help, including generic Qt
#                                             options.
#   -v, --version                             Displays version information.
#   --dir <directory>                         Use directory instead of binary
#                                             directory.
#   --qmake <path>                            Use specified qmake instead of
#                                             qmake from PATH. Deprecated, use
#                                             qtpaths instead.
#   --qtpaths <path>                          Use specified qtpaths.exe instead
#                                             of qtpaths.exe from PATH.
#   --libdir <path>                           Copy libraries to path.
#   --plugindir <path>                        Copy plugins to path.
#   --translationdir <path>                   Copy translations to path.
#   --qml-deploy-dir <path>                   Copy qml files to path.
#   --debug                                   Assume debug binaries.
#   --release                                 Assume release binaries.
#   --pdb                                     Deploy .pdb files (MSVC).
#   --force                                   Force updating files.
#   --dry-run                                 Simulation mode. Behave normally,
#                                             but do not copy/update any files.
#   --no-patchqt                              Do not patch the Qt6Core library.
#   --ignore-library-errors                   Ignore errors when libraries cannot
#                                             be found.
#   --no-plugins                              Skip plugin deployment.
#   --include-soft-plugins                    Include in the deployment all
#                                             relevant plugins by taking into
#                                             account all soft dependencies.
#   --skip-plugin-types <plugin types>        A comma-separated list of plugin
#                                             types that are not deployed
#                                             (qmltooling,generic).
#   --add-plugin-types <plugin types>         A comma-separated list of plugin
#                                             types that will be added to
#                                             deployment
#                                             (imageformats,iconengines)
#   --include-plugins <plugins>               A comma-separated list of
#                                             individual plugins that will be
#                                             added to deployment (scene2d,qjpeg)
#   --exclude-plugins <plugins>               A comma-separated list of
#                                             individual plugins that will not be
#                                             deployed (qsvg,qpdf)
#   --no-libraries                            Skip library deployment.
#   --qmldir <directory>                      Scan for QML-imports starting from
#                                             directory.
#   --qmlimport <directory>                   Add the given path to the QML
#                                             module search locations.
#   --no-quick-import                         Skip deployment of Qt Quick
#                                             imports.
#   --translations <languages>                A comma-separated list of languages
#                                             to deploy (de,fi).
#   --no-translations                         Skip deployment of translations.
#   --no-system-d3d-compiler                  Skip deployment of the system D3D
#                                             compiler.
#   --no-system-dxc-compiler                  Skip deployment of the system DXC
#                                             (dxcompiler.dll, dxil.dll).
#   --compiler-runtime                        Deploy compiler runtime (Desktop
#                                             only).
#   --no-compiler-runtime                     Do not deploy compiler runtime
#                                             (Desktop only).
#   --json                                    Print to stdout in JSON format.
#   --no-opengl-sw                            Do not deploy the software
#                                             rasterizer library.
#   --no-ffmpeg                               Do not deploy the FFmpeg libraries.
#   --force-openssl                           Deploy openssl plugin but ignore
#                                             openssl library dependency
#   --openssl-root <directory>                Directory containing openSSL
#                                             libraries.
#   --list <option>                           Print only the names of the files
#                                             copied.
#                                             Available options:
#                                              source:   absolute path of the
#                                             source files
#                                              target:   absolute path of the
#                                             target files
#                                              relative: paths of the target
#                                             files, relative
#                                                        to the target directory
#                                              mapping:  outputs the source and
#                                             the relative
#                                                        target, suitable for use
#                                             within an
#                                                        Appx mapping file
#   --verbose <level>                         Verbose level (0-2).

contains(CONFIG, static) {
    # Nothing to do...
} else {
    win32 {
        DEPLOY_TOOL = $${dirname(QMAKE_QMAKE)}/windeployqt.exe
        DEPLOY_TOOL = $$replace(DEPLOY_TOOL, /, \\)
        contains(CONFIG, debug, debug|release) {
            DEPLOY_TARGET = $${OUT_PWD}/debug/$${TARGET}.exe
        } else {
            DEPLOY_TARGET = $${OUT_PWD}/release/$${TARGET}.exe
        }
    }

    DEPLOY_TARGET=$$replace(DEPLOY_TARGET, /, \\)
    msvc {
        QMAKE_POST_LINK+=$${DEPLOY_TOOL} $${DEPLOY_TARGET} --no-compiler-runtime $$escape_expand(\\n)
    } else {
        QMAKE_POST_LINK+='$$escape_expand("\\n\\t") $${DEPLOY_TOOL} $${DEPLOY_TARGET} --no-compiler-runtime $$escape_expand("\\n\\t")'
    }
}
