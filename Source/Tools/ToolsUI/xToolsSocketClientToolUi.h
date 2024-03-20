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
#include <QLoggingCategory>

namespace Ui {
class SAKSocketClientToolUi;
}

class xToolsSocketClientTool;
class xToolsSocketClientToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    explicit xToolsSocketClientToolUi(QWidget *parent = nullptr);
    ~xToolsSocketClientToolUi();
    virtual void onIsWorkingChanged(bool isWorking) final;
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup) final;

private:
    const QLoggingCategory mLoggingCategory{"sak.socketclienttoolui"};
    xToolsSocketClientTool *mTool{nullptr};

private:
    Ui::SAKSocketClientToolUi *ui{nullptr};

    void onComboBoxClientAddressActived();
    void onSpinBoxClientPortValueChanged(int value);
    void onComboBoxServerAddressCurrentTextChanged();
    void onSpinBoxServerPortValueChanged(int value);
    void onCheckBoxSpecifyIpAndPortClicked();
};
