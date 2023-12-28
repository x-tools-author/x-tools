/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakmdnsassistant.h"
#include "sakui.h"

int main(int argc, char* argv[])
{
    const QString appName = QObject::tr("MDNS Assistant");
    SAK_APP(SAKMdnsAssistant, argc, argv, appName);
    return QApplication::exec();
}