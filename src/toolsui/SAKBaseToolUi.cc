/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKBaseTool.hh"
#include "SAKBaseToolUi.hh"

SAKBaseToolUi::SAKBaseToolUi(QWidget *parent)
    : QWidget{parent}
{
//    Q_ASSERT_X(tool, __FUNCTION__, "The value of tool can not be nullptr");
//    if (mTool) {
//        connect(mTool, &SAKBaseTool::isWorkingChanged, this, [=](){
//            bool isWorking = mTool->isWorking();
//            this->onIsWorkingChanged(isWorking);
//        });
//    }
}

void SAKBaseToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
}
