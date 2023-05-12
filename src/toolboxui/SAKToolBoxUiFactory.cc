/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolFactory.hh"
#include "SAKToolBoxUiFactory.hh"
#include "SAKSerialPortToolBoxUi.hh"

SAKToolBoxUiFactory::SAKToolBoxUiFactory(QObject *parent)
    : QObject{parent}
{

}

SAKToolBoxUiFactory *SAKToolBoxUiFactory::instance()
{
    static SAKToolBoxUiFactory factory;
    return &factory;
}

QList<int> SAKToolBoxUiFactory::supportedTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool;
    return list;
}

SAKToolBoxUi *SAKToolBoxUiFactory::createToolBoxUi(int type)
{
    if (type == SAKToolFactory::SerialportTool) {
        return new SAKSerialPortToolBoxUi();
    }

    return Q_NULLPTR;
}
