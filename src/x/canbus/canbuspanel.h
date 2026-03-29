/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusFrame>
#include <QJsonObject>
#include <QWidget>

namespace xCanBus {

class CanBusPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CanBusPanel(QWidget* parent = nullptr);
    ~CanBusPanel() override;

    virtual QJsonObject save() const;
    virtual void load(const QJsonObject& obj);

    virtual void onFrameRx(const QCanBusFrame& frame);
    virtual void onFrameTx(const QCanBusFrame& frame);

    virtual void onConnected();
    virtual void onDisconnected();

signals:
    void outputFrame(const QCanBusFrame& frame);
};

} // namespace xCanBus