/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSOCKETCLIENTTOOLUI_H
#define SAKSOCKETCLIENTTOOLUI_H

#include "sakcommunicationtoolui.h"
#include <QLoggingCategory>

namespace Ui {
class SAKSocketClientToolUi;
}

class SAKSocketClientTool;
class SAKSocketClientToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKSocketClientToolUi(QWidget *parent = nullptr);
    ~SAKSocketClientToolUi();
    virtual void onIsWorkingChanged(bool isWorking) final;
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup) final;

private:
    const QLoggingCategory mLoggingCategory{"sak.socketclienttoolui"};
    SAKSocketClientTool *mTool{nullptr};

private:
    Ui::SAKSocketClientToolUi *ui{nullptr};

    void onComboBoxClientAddressActived();
    void onSpinBoxClientPortValueChanged(int value);
    void onComboBoxServerAddressCurrentTextChanged();
    void onSpinBoxServerPortValueChanged(int value);
    void onCheckBoxSpecifyIpAndPortClicked();
};

#endif // SAKSOCKETCLIENTTOOLUI_H
