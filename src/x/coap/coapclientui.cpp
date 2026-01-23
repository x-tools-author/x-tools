/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapclientui.h"
#include "ui_coapclientui.h"

#include <QHBoxLayout>
#include <QObject>

#include "coapclient.h"
#include "coapcommon.h"
#include "coapmsgview.h"

namespace Ui {
class CoAPClientUi;
}

namespace xCoAP {

class CoAPClientUiPrivate : QObject
{
public:
    CoAPClientUiPrivate(CoAPClientUi* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPClientUi();
        ui->setupUi(q_ptr);
        ui->pushButtonClose->setEnabled(false);
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, [=]() { onOpenButtonClicked(); });
        connect(ui->pushButtonClose, &QPushButton::clicked, this, [=]() { onCloseButtonClicked(); });

        m_client = new CoAPClient(q);
        connect(m_client, &CoAPClient::started, this, [=]() { onStarted(); });
        connect(m_client, &CoAPClient::finished, this, [=]() { onFinished(); });

        m_msgView = new CoAPMsgView(q);
        QHBoxLayout* layout = new QHBoxLayout(ui->widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_msgView);
    }
    ~CoAPClientUiPrivate() { delete ui; }

public:
    Ui::CoAPClientUi* ui{nullptr};

private:
    CoAPClientUi* q{nullptr};
    CoAPClient* m_client{nullptr};
    CoAPMsgView* m_msgView{nullptr};

private:
    void onStarted()
    {
        ui->pushButtonClose->setEnabled(true);
        ui->pushButtonOpen->setEnabled(false);
    }
    void onFinished()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(true);
    }
    void onOpenButtonClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        onCloseButtonClicked();
        m_client->startClient(q->save());
    }
    void onCloseButtonClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        m_client->stopClient();
    }
};

CoAPClientUi::CoAPClientUi(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPClientUiPrivate(this);

    CoAPCommon::setupSocketAddress(d->ui->comboBoxServerIp);
    CoAPCommon::setupSocketPort(d->ui->spinBoxServerPort);
    CoAPCommon::setupSocketProtocol(d->ui->comboBoxProtocol);
}

CoAPClientUi::~CoAPClientUi() {}

QJsonObject CoAPClientUi::save()
{
    CoAPCommon::ClientParameterKeys keys;
    QJsonObject obj;
    obj[keys.serverAddress] = d->ui->comboBoxServerIp->currentText();
    obj[keys.serverPort] = d->ui->spinBoxServerPort->value();
    obj[keys.protocol] = d->ui->comboBoxProtocol->currentData().toInt();
    return obj;
}

void CoAPClientUi::load(const QJsonObject& obj)
{
    CoAPCommon::ClientParameterKeys keys;
    CoAPCommon::ClientParameters params = CoAPCommon::jsonObject2ClientParameters(obj);
    int index = d->ui->comboBoxServerIp->findText(params.serverAddress);
    if (index != -1) {
        d->ui->comboBoxServerIp->setCurrentIndex(index);
    }
    d->ui->spinBoxServerPort->setValue(params.serverPort);
    index = d->ui->comboBoxProtocol->findData(params.protocol);
    if (index != -1) {
        d->ui->comboBoxProtocol->setCurrentIndex(index);
    }
}

} // namespace xCoAP
