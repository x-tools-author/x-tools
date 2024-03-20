/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "sakserialporttransmittertooluieditor.h"
#include "saktransmittertoolui.h"

class SAKSerialPortTransmitterToolUi : public SAKTransmitterToolUi
{
    Q_OBJECT
public:
    SAKSerialPortTransmitterToolUi(QWidget *parent = nullptr);

protected:
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup) override;
    virtual QDialog *itemEditor() override;

private:
    SAKSerialPortTransmitterToolUiEditor *mEditor;
};
