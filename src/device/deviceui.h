/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>
#include <QWidget>

class Device;
class DeviceUi : public QWidget
{
    Q_OBJECT
public:
    DeviceUi(QWidget *parent = nullptr);
    ~DeviceUi() override;

    void openDevice();
    void closeDevice();
    Device *device();

    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &parameters);
    virtual QList<QWidget *> deviceControllers();
    virtual void setUiEnabled(bool enabled);

protected:
    virtual Device *newDevice() = 0;

private:
    Device *m_device{nullptr};
};
