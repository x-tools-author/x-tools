/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSOCKETSERVERTOOLUI_H
#define SAKSOCKETSERVERTOOLUI_H

#include "sakcommunicationtoolui.h"
#include <QLoggingCategory>

namespace Ui {
class SAKSocketServerToolUi;
}

class SAKSocketServerTool;
class SAKSocketServerToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKSocketServerToolUi(QWidget *parent = nullptr);
    ~SAKSocketServerToolUi();
    virtual void onIsWorkingChanged(bool isWorking) final;
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup) final;

private:
    SAKSocketServerTool *mTool{nullptr};
    const QLoggingCategory mLoggingCategory{"sak.socketservertoolui"};

private:
    Ui::SAKSocketServerToolUi *ui{nullptr};

    void onComboBoxServerIpActived();
    void onSpinBoxServerPortValueChanged(int value);
    void onComboBoxClientsIndexChanged();
    void onComboBoxMessageTypeIndexChanged();
    void onCheckBoxSpecifyIpAndPortClicked();
};

#endif // SAKSOCKETSERVERTOOLUI_H
