/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class MqttServerUi;
}

namespace xMQTT {

struct MqttMessage;
class MqttServer;
class MqttServerUi : public QWidget
{
    Q_OBJECT
public:
    explicit MqttServerUi(QWidget *parent = nullptr);
    ~MqttServerUi() override;

    QJsonObject save();
    void load(const QJsonObject &obj);

protected:
    bool event(QEvent *event) override;

private:
    Ui::MqttServerUi *ui;
    MqttServer *m_server{nullptr};
    int m_rightWidth{168};
    QStandardItemModel *m_clientsModel{nullptr};

private:
    QString clientFlag(const QString &ip, quint16 port);
    QStandardItem *findClientItem(const QString &ip, quint16 port);
    QStandardItem *addClientItem(const QString &ip, quint16 port);
    void removeClientItem(const QString &ip, quint16 port);
    QStandardItem *findSubscriptionItem(QStandardItem *clientItem, const QString &topic);
    QStandardItem *addSubscriptionItem(QStandardItem *clientItem, const QString &topic);
    void removeSubscriptionItem(QStandardItem *clientItem, const QString &topic);
    void disableOpenBtnAndCloseBtn();

    void onOpenBtnClicked();
    void onCloseBtnClicked();
    void onLogMessageReceived(const QString &msg, bool isError);
    void onMqttMessageRx(std::shared_ptr<MqttMessage> message);
    void onServerStarted();
    void onServerStopped();
    void onClientSubscribed(const QString &ip, quint16 port, const QString &topic);
    void onClientUnsubscribed(const QString &ip, quint16 port, const QString &topic);
};

} // namespace xMQTT