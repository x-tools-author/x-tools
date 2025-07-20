/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "deviceui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class UdpMulticastUi;
}
QT_END_NAMESPACE

class Device;
class UdpMulticastUi : public DeviceUi
{
    Q_OBJECT
public:
    explicit UdpMulticastUi(QWidget *parent = nullptr);
    ~UdpMulticastUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

protected:
    Device *newDevice() override;

private:
    Ui::UdpMulticastUi *ui;
};