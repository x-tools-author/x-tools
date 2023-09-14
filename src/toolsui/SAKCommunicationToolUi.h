/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMMUNICATIONTOOLUI_HH
#define SAKCOMMUNICATIONTOOLUI_HH

#include "SAKBaseToolUi.h"

class SAKCommunicationTool;
class SAKCommunicationToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    explicit SAKCommunicationToolUi(QWidget *parent = nullptr);
    virtual void onIsWorkingChanged(bool isWorking) override;
};

#endif // SAKCOMMUNICATIONTOOLUI_HH
