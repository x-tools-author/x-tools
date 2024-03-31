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
class xToolsSocketClientToolUi;
}

class xToolsSocketClientTool;
class xToolsSocketClientToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    explicit xToolsSocketClientToolUi(QWidget *parent = nullptr);
    ~xToolsSocketClientToolUi();
    virtual void onIsWorkingChanged(bool isWorking) override;
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool,
                                         const QString &settingsGroup) override;

private:
    Ui::xToolsSocketClientToolUi *ui{nullptr};
    xToolsSocketClientTool *m_tool{nullptr};

public slots:
    void onComboBoxClientAddressActivated();
    void onSpinBoxClientPortValueChanged(int value);
    void onComboBoxServerAddressCurrentTextChanged();
    void onSpinBoxServerPortValueChanged(int value);
    void onAuthenticationCheckBoxClicked();
    void onUserNameLineEditTextChanged(const QString &text);
    void onPasswordLineEditTextChanged(const QString &text);
};
