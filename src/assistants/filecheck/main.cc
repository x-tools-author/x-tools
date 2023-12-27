/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>

#include "sakcommonmainwindow.h"
#include "sakfilecheckassistant.h"

int main(int argc, char* argv[])
{
    const QString title = QObject::tr("File Check Assistant");
    QApplication* app = sakNewApp<SAKFileCheckAssistant>(argc, argv, title);
    int ret = app->exec();
    app->deleteLater();

    return ret;
}
