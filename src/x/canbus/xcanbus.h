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
#include <QToolButton>
#include <QWidget>

#include "utilities/keepopenedmenu.h"
#include "x/canbus/canbuspanel.h"
#include "x/common/xpage.h"

namespace Ui {
class xCanBus;
}

namespace xCanBus {

class CanBusDevice;
class xCanBus : public xPage
{
    Q_OBJECT
public:
    explicit xCanBus(QWidget* parent = nullptr);
    ~xCanBus() override;

    QJsonObject save() const override;
    void load(const QJsonObject& obj) override;

protected:
    bool event(QEvent* event) override;

private:
    Ui::xCanBus* ui;
    int m_rightWidth{400};
    CanBusDevice* m_device{nullptr};
    xTools::KeepOpenedMenu* m_menu{nullptr};
    QTimer* m_timedSender{nullptr};

private:
    struct PanelItem
    {
        QString key;
        CanBusPanel* panel{nullptr};
        QToolButton* btn{nullptr};
    };
    QList<PanelItem> m_panels;

    void addPanel(const QString& key, const QString& title, CanBusPanel* panel);

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
    void setupTimedSending(QComboBox* cb);

    QCanBusFrame::FrameErrors errorFilter() const;
    bool hasErrorFilter() const;
    void updateErrorFilterBtn();
    void updateInputValidator();
    void sendFrame();
    void updateUiState(bool isConnected);
};

} // namespace xCanBus