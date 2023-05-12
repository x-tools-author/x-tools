/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBoxUiFactory.hh"
#include "tools/SAKToolsFactory.hh"

SAKToolBoxUiFactory::SAKToolBoxUiFactory(QObject *parent)
    : QObject{parent}
{

}

SAKToolBoxUiFactory *SAKToolBoxUiFactory::factory()
{
    static SAKToolBoxUiFactory factory;
    return &factory;
}

QList<int> SAKToolBoxUiFactory::supportedTools()
{
    QList<int> list;
    list << SAKToolsFactory::SerialportTool;
    return list;
}

SAKToolBoxUi *SAKToolBoxUiFactory::createToolBoxUi(int type)
{

}
