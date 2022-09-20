/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QApplication>
#include "SAKAtAssistantMainWindow.hh"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    SAKAtAssistantMainWindow mainWindow;
    mainWindow.show();
    mainWindow.resize(mainWindow.height()*1.618, mainWindow.height());

    return app.exec();
}