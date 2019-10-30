/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include "SAKApplication.hh"

#include <QMessageLogContext>
#include <QDateTime>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <QSettings>

#include "SAKConsole.hh"
#include "SAKGlobal.hh"

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(SAKConsole::messageRedirection);
#endif
    SAKApplication a(argc, argv);

    qInfo() << "The applincation has started, The log will be save in file:" << SAKGlobal::logFile();

    return a.exec();
}
