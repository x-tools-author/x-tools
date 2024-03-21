/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBLECENTRALTOOLUI_H
#define SAKBLECENTRALTOOLUI_H

#include <QLowEnergyDescriptor>

#include "sakblecentraltool.h"
#include "xToolsCommunicationToolUi.h"

namespace Ui {
class SAKBleCentralToolUi;
}
class SAKBleCentralToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKBleCentralToolUi(QWidget *parent = nullptr);
    ~SAKBleCentralToolUi();

protected:
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool,
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
    void onDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &newValue);

    void onPushButtonScanClicked();
    void onComboBoxDevicesActived();
    void onComboBoxServicesCurrentIndexChanged();
    void onComboBoxCharacteristicsActived();
    void onComboBoxWriteWayCurrentIndexChanged();
    void onPushButtonNotifyClicked();
    void onPushButtonReadClicked();
};

#endif // SAKBLECENTRALTOOLUI_H
