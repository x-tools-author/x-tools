/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsCommunicationToolUi.h"

namespace Ui {
class xToolsSerialPortToolUi;
}

class xToolsSerialPortTool;
class xToolsSerialPortToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    explicit xToolsSerialPortToolUi(QWidget *parent = nullptr);
    ~xToolsSerialPortToolUi();
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup) final;

private:
    xToolsSerialPortTool *m_tool{nullptr};

private:
    void checkInitializingStatus();

private:
    Ui::xToolsSerialPortToolUi *ui;

    void onComboBoxPortNamesCurrentTextChanged();
    void onComboBoxBaudRateCurrentIndexChanged();
    void onComboBoxDataBitsCurrentIndexChanged();
    void onComboBoxStopBitsCurrentIndexChanged();
    void onComboBoxParityCurrentIndexChanged();
    void onComboBoxFlowControlCurrentIndexChanged();
};
