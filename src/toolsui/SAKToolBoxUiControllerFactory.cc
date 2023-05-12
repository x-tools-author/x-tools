/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "tools/SAKToolsFactory.hh"

#include "SAKToolBoxUiControllerFactory.hh"
#include "SAKToolBoxUiSerialPortController.hh"

SAKToolBoxUiControllerFactory::SAKToolBoxUiControllerFactory(QObject *parent)
    : QObject{parent}
{

}

SAKToolBoxUiControllerFactory *SAKToolBoxUiControllerFactory::instance()
{
    static SAKToolBoxUiControllerFactory factory;
    return &factory;
}

QWidget *SAKToolBoxUiControllerFactory::createController(
    int type, SAKCommunicationTool *tool)
{
    if (type == SAKToolsFactory::SerialportTool) {
        return new SAKToolBoxUiSerialPortController(tool);
    }
    return nullptr;
}
