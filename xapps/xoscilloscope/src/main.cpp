/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>
#include <QLocale>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *translator = new QTranslator();
    if (QLocale::system().name() == "zh_CN") {
        if (translator->load(":/res/translations/xOscilloscope_zh_CN.qm")) {
            a.installTranslator(translator);
        }
    } else {
        if (translator->load(":/res/translations/xOscilloscope_en.qm")) {
            a.installTranslator(translator);
        }
    }

    /* Apply style sheet */
    QFile file(":/serial_port_plotter/res/styles/style.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    /* Get the icon for the window corner */
    MainWindow w;
    QIcon appIcon(":/res/icons/logo.ico");
    w.setWindowIcon(appIcon);
    w.setWindowTitle(QObject::tr("xOscilloscope") + " v1.1.0");
    w.resize(1366, 768);
#if defined(X_MAGIC)
    w.setWindowOpacity(0.3);
#endif
    w.show();

    return a.exec();
}
