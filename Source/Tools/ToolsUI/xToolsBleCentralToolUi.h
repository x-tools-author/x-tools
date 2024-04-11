/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QLowEnergyDescriptor>

#include "xToolsBleCentralTool.h"
#include "xToolsCommunicationToolUi.h"

namespace Ui {
class xToolsBleCentralToolUi;
}
class xToolsBleCentralToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    explicit xToolsBleCentralToolUi(QWidget *parent = nullptr);
    ~xToolsBleCentralToolUi();

protected:
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool,
                                         const QString &settingsGroup) override;

    virtual void onIsWorkingChanged(bool isWorking) final;

private:
    Ui::xToolsBleCentralToolUi *ui{nullptr};
    xToolsBleCentralTool *m_bleTool;

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
