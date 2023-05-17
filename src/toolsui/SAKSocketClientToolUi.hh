/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTOOLUI_HH
#define SAKSOCKETCLIENTTOOLUI_HH

#include <QLoggingCategory>
#include "SAKCommunicationToolUi.hh"

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
    virtual void setupCommunicationTool(SAKCommunicationTool *tool) final;
    virtual void updateUiState(bool isWorking) final;

private:
    const QLoggingCategory mLoggingCategory{"SAK.SocketClientToolUi"};
    SAKSocketClientTool *mTool{nullptr};

private:
    Ui::SAKSocketClientToolUi *ui{nullptr};

    void onComboBoxClientAddressActived();
    void onSpinBoxClientPortValueChanged(int value);
    void onComboBoxServerAddressActived();
    void onSpinBoxServerPortValueChanged(int value);
    void onCheckBoxSpecifyIpAndPortClicked();
};

#endif // SAKSOCKETCLIENTTOOLUI_H
