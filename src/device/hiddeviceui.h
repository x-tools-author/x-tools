/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "deviceui.h"
#include "utilities/hidscanner.h"

namespace Ui {
class HidDeviceUi;
}

class HidScanner;
class HidDeviceUi : public DeviceUi
{
    Q_OBJECT
public:
    HidDeviceUi(QWidget *parent = nullptr);
    ~HidDeviceUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

protected:
    Device *newDevice() override;

private:
    void refresh();
    void onPortNameChanged(const QStringList &portName);
    void onDevicesChanged(const QList<HidDeviceInfo> &infos);
    void onDeviceIndexChanged(int index);

private:
    Ui::HidDeviceUi *ui;
    HidScanner *m_hidScanner{nullptr};
};
