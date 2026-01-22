/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapserverui.h"
#include "ui_coapserverui.h"

#include <QObject>

#include "coapcommon.h"
#include "coapserver.h"

namespace Ui {
class CoAPServerUi;
}

namespace xCoAP {

class CoAPServerUiPrivate : QObject
{
public:
    CoAPServerUiPrivate(CoAPServerUi* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPServerUi();
        ui->setupUi(q_ptr);
        ui->pushButtonClose->setEnabled(false);
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, [=]() { onOpenBtnClicked(); });
        connect(ui->pushButtonClose, &QPushButton::clicked, this, [=]() { onCloseBtnClicked(); });

        m_server = new CoAPServer(q);
        connect(m_server, &QThread::started, this, [=]() { onStarted(); });
        connect(m_server, &QThread::finished, this, [=]() { onFinished(); });
    }
    ~CoAPServerUiPrivate()
    {
        delete ui;
        if (m_server && m_server->isRunning()) {
            m_server->requestInterruption();
            m_server->wait();
        }
    }

public:
    Ui::CoAPServerUi* ui{nullptr};
    CoAPServer* m_server{nullptr};

public:
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
    void onOpenBtnClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        if (m_server->isRunning()) {
            m_server->requestInterruption();
            m_server->exit();
            m_server->wait();
        }

        m_server->start();
    }
    void onCloseBtnClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        if (m_server->isRunning()) {
            m_server->requestInterruption();
            m_server->wait();
        }
    }

private:
    CoAPServerUi* q{nullptr};

private:
    QJsonObject save()
    {
        QJsonObject obj;
        return obj;
    }
    void load(const QJsonObject& obj) { Q_UNUSED(obj); }
};

CoAPServerUi::CoAPServerUi(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPServerUiPrivate(this);

    CoAPCommon::setupSocketAddress(d->ui->comboBoxServerIp);
    CoAPCommon::setupSocketPort(d->ui->spinBoxServerPort);
    CoAPCommon::setupSocketProtocol(d->ui->comboBoxProtocol);
}

CoAPServerUi::~CoAPServerUi() {}

QJsonObject CoAPServerUi::save()
{
    QJsonObject obj;
    return obj;
}

void CoAPServerUi::load(const QJsonObject& obj) {}

} // namespace xCoAP
