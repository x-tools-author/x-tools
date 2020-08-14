/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugPage.hh"
#include "SAKDebugPageController.hh"

SAKDebugPageController::SAKDebugPageController(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
{
    connect(this, &SAKDebugPageController::messageChange, mDebugPage, &SAKDebugPage::outputMessage);
}

SAKDebugPageController::~SAKDebugPageController()
{

}

QVariant SAKDebugPageController::parameters()
{
    return QVariant();
}

void SAKDebugPageController::setUiEnable(bool opened)
{
    setEnabled(!opened);
}

void SAKDebugPageController::refreshDevice()
{
    // Nothing to do
}
