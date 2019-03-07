/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#include "SAKApplication.h"

#include <QMessageLogContext>
#include <QDateTime>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <QSettings>

int main(int argc, char *argv[])
{
#if 0
    qInstallMessageHandler(SAKAppMsgOutput);
#endif
    SAKApplication a(argc, argv);

    QSettings settings;
    QString style = settings.value(QString(MainWindow::appStyle())).toString();
    if (style.isEmpty()){
        style = QString("Fusion");
        settings.setValue(QString(MainWindow::appStyle()), style);
    }
    QApplication::setStyle(QStyleFactory::create(style));

    return a.exec();
}
