/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALTOOLUI_HH
#define SAKBLECENTRALTOOLUI_HH

#include <QLowEnergyDescriptor>

#include "SAKBleCentralTool.h"
#include "SAKCommunicationToolUi.h"

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

    virtual void onIsWorkingChanged(bool isWorking) final;

private:
    Ui::SAKBleCentralToolUi *ui{nullptr};
    SAKBleCentralTool *mBleTool;
    const QLoggingCategory mLoggingCategory{"sak.blecentraltoolui"};

private:
    void initSettingsMenu(const QString &settingsGroup);

private slots:
    void onServiceDiscoveryStarted();
    void onServiceDiscoveryFinished();
    void onDescriptorWritten(const QLowEnergyDescriptor &descriptor,
                             const QByteArray &newValue);

    void onPushButtonScanClicked();
    void onComboBoxDevicesActived();
    void onComboBoxServicesCurrentIndexChanged();
    void onComboBoxCharacteristicsActived();
    void onComboBoxWriteWayCurrentIndexChanged();
    void onPushButtonNotifyClicked();
    void onPushButtonReadClicked();
};

#endif // SAKBLECENTRALTOOLUI_HH
