/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusFrame>
#include <QComboBox>
#include <QJsonObject>
#include <QTimer>
#include <QWidget>

#include "utilities/keepopenedmenu.h"
#include "utilities/serializable.h"

namespace Ui {
class xCanBus;
}

namespace xCanBus {

class CanBusDevice;
class xCanBus : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit xCanBus(QWidget* parent = nullptr);
    ~xCanBus();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;

protected:
    bool event(QEvent* event) override;

private:
    Ui::xCanBus* ui;
    int m_leftWidth{168};
    CanBusDevice* m_device{nullptr};
    xTools::KeepOpenedMenu* m_menu{nullptr};
    QTimer* m_timedSender{nullptr};

private:
    void onDisconnectBtnClicked();
    void onConnectBtnClicked();
    void onDeviceOpened();
    void onDeviceClosed();
    void onPluginChanged(const QString& pluginName);
    void onSendBtnClicked();
    void onTimedSendingChanged();
    void onTimedSenderTimeout();
    void onFrameRx(const QCanBusFrame& frame);
    void onFrameTx(const QCanBusFrame& frame);

    void setupOptions(QComboBox* cb, bool usingUnspecified);
    void setupBitRates(QComboBox* cb, bool isFlexibleDataRateEnable);
    void setupFrameTypes(QComboBox* cb);
    void setupTimedSending(QComboBox* cb);

    QCanBusFrame::FrameErrors errorFilter() const;
    bool hasErrorFilter() const;
    void updateErrorFilterBtn();
    void updateInputValidator();
    void sendFrame();
    void updateUiState(bool isConnected);
};

} // namespace xCanBus