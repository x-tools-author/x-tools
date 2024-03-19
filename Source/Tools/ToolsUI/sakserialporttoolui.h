/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "sakcommunicationtoolui.h"
#include <QLoggingCategory>

namespace Ui {
class SAKSerialPortToolUi;
}

class xToolsSerialPortTool;
class SAKSerialPortToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKSerialPortToolUi(QWidget *parent = nullptr);
    ~SAKSerialPortToolUi();
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup) final;

private:
    xToolsSerialPortTool *mTool{nullptr};
    const QLoggingCategory mLoggingCategory{"sak.serialporttoolui"};

private:
    void checkInitializingStatus();

private:
    Ui::SAKSerialPortToolUi *ui;

    void onComboBoxPortNamesCurrentTextChanged();
    void onComboBoxBaudRateCurrentIndexChanged();
    void onComboBoxDataBitsCurrentIndexChanged();
    void onComboBoxStopBitsCurrentIndexChanged();
    void onComboBoxParityCurrentIndexChanged();
    void onComboBoxFlowControlCurrentIndexChanged();
};
