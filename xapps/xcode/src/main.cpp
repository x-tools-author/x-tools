/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>
#include <QDateTime>

#include "common/xapp.h"
#include "xcode.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("xTools");
    QApplication::setApplicationName("xCode");
    QApplication::setApplicationVersion("1.0.0");

    xApp app(argc, argv);

    auto settings = app.settings();
    settings->setValue("Application/boost", QDateTime::currentDateTime().toString(Qt::ISODate));

    app.setupHdpi();
    app.setupColorScheme();
    app.setupLanguage();

    xCode w;
    w.show();
    w.generateBarcode();
    return app.exec();
}