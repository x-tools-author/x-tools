/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sak.h"
#include "saklog.h"
#include "sakguiapplication.h"

int main(int argc, char* argv[])
{
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
    QString appName = QString("EasyDebug");
    sakDoSomethingBeforeAppCreated(argv, appName);

    SAKGuiApplication app(argc, argv);
    Q_UNUSED(app)
    SAKLog::instance()->start();
    return SAKGuiApplication::exec();
}
