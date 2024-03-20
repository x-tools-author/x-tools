/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseToolUi.h"

class xToolsCommunicationTool;
class xToolsCommunicationToolUi : public xToolsBaseToolUi
{
    Q_OBJECT
public:
    explicit xToolsCommunicationToolUi(QWidget *parent = nullptr);
    virtual void onIsWorkingChanged(bool isWorking) override;
};
