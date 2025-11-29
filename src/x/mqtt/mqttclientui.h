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
#include <QWidget>

namespace Ui {
class MqttClientUi;
}

namespace xMQTT {

class MqttClient;
class MqttClientUi : public QWidget
{
    Q_OBJECT
public:
    explicit MqttClientUi(QWidget *parent = nullptr);
    ~MqttClientUi() override;

    QJsonObject save() const;
    void load(const QJsonObject &obj) const;

protected:
    bool event(QEvent *event) override;

private:
    Ui::MqttClientUi *ui;
    int m_leftWidth{168};
    MqttClient *m_client{nullptr};

private:
    void onOpenButtonClicked() const;
    void onCloseButtonClicked() const;
    void onPublishButtonClicked();
    void onSubscribeButtonClicked();
    void onLogMessageReceived(const QString &msg, bool isError) const;

    void showNotOpenedWarning();
    void showEmptyTopicWarning();
};

} // namespace xMQTT