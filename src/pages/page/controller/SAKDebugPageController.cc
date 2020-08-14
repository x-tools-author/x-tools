/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QEventLoop>
#include "SAKDebugPageController.hh"

SAKDebugPageController::SAKDebugPageController(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
{
    // nothing to do
}

SAKDebugPageController::~SAKDebugPageController()
{

}

void SAKDebugPageController::setUiEnable(bool opened)
{
    setEnabled(!opened);
}
