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

private:
    void onOpenBtnClicked();
    void onCloseBtnClicked();
    void onLogMessageReceived(const QString &msg, bool isError);
    void onMqttMessageRx(std::shared_ptr<MqttMessage> message);
};

} // namespace xMQTT