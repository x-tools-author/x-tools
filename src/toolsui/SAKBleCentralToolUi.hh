/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALTOOLUI_HH
#define SAKBLECENTRALTOOLUI_HH

#include "SAKBleCentralTool.hh"
#include "SAKCommunicationToolUi.hh"

namespace Ui {
class SAKBleCentralToolUi;
}
class SAKBleCentralToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKBleCentralToolUi(QWidget *parent = nullptr);
    ~SAKBleCentralToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingsGroup) override;

    virtual void updateUiState(bool isWorking) final;

private:
    Ui::SAKBleCentralToolUi *ui{nullptr};
    SAKBleCentralTool *mBleTool;

private:
    void initSettingsMenu(const QString &settingsGroup);

private slots:
    void onPushButtonScanClicked();
    void onComboBoxDevicesActived();
};

#endif // SAKBLECENTRALTOOLUI_HH
