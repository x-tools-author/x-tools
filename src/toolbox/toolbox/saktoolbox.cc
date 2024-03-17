/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolbox.h"

#include "saktoolfactory.h"

SAKToolBox::SAKToolBox(QObject* parent)
    : QObject{parent}
{
    auto createTool = [](int type) -> SAKBaseTool* {
        auto toolFactory = SAKToolFactory::instance();
        return toolFactory->createTool(type);
    };

    // clang-format off
    m_emitter = qobject_cast<SAKEmitterTool*>(createTool(SAKToolFactory::EmitterTool));
    m_responser = qobject_cast<SAKResponserTool*>(createTool(SAKToolFactory::ResponserTool));
    m_storer = qobject_cast<SAKStorerTool*>(createTool(SAKToolFactory::StorerTool));
    m_prestorer = qobject_cast<SAKPrestorerTool*>(createTool(SAKToolFactory::PrestoreTool));
    m_rxVelometer = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    m_txVelometer = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    m_rxCounter = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));
    m_txCounter = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));

    m_udpTransmitter = qobject_cast<SAKUdpTransmitterTool*>(createTool(SAKToolFactory::UdpTransmitterTool));
    m_tcpTransmitter = qobject_cast<SAKTcpTransmitterTool*>(createTool(SAKToolFactory::TcpTransmitterTool));
    m_webSocketTransmitter = qobject_cast<SAKWebSocketTransmitterTool*>( createTool(SAKToolFactory::WebSocketTransmitterTool));
    m_serialPortTransmitter = qobject_cast<SAKSerialPortTransmitterTool*>( createTool(SAKToolFactory::SerialPortTransmitterTool));
    // clang-format on

    m_tools << m_emitter << m_responser << m_storer << m_prestorer << m_rxVelometer << m_txVelometer
            << m_rxCounter << m_txCounter << m_udpTransmitter << m_tcpTransmitter
            << m_webSocketTransmitter << m_serialPortTransmitter;

    int flag = Qt::AutoConnection | Qt::UniqueConnection;
    for (auto tool : m_tools) {
        connect(tool,
                &SAKBaseTool::errorOccurred,
                this,
                &SAKToolBox::errorOccurred,
                Qt::ConnectionType(flag));
    }

    connect(this, &SAKToolBox::errorOccurred, this, [=]() { this->close(); });
}

SAKToolBox::~SAKToolBox()
{
    if (m_comunicator) {
        m_comunicator->exit();
        m_comunicator->wait();
        m_comunicator->deleteLater();
    }

    for (auto tool : m_tools) {
        tool->exit();
        tool->wait();
        tool->deleteLater();
    }
}

void SAKToolBox::initialize(int type)
{
    if (m_comunicator) {
        m_comunicator->exit();
        m_comunicator->wait();
        m_comunicator->deleteLater();
        m_comunicator = nullptr;
    }

    auto toolFactory = SAKToolFactory::instance();
    auto tool = toolFactory->createTool(type);
    m_comunicator = qobject_cast<SAKCommunicationTool*>(tool);
    if (!m_comunicator) {
        qWarning() << "mComunicationTool is nullptr, type:" << type;
        return;
    }

    // clang-format off
    connect(m_comunicator, &SAKCommunicationTool::bytesWritten, this, &SAKToolBox::onCommunicatorBytesWritten);
    connect(m_comunicator, &SAKCommunicationTool::bytesRead, this, &SAKToolBox::onCommunicatorBytesRead);
    // emiiter,responser,prestorer->tx
    connect(m_emitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_responser, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_prestorer, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);

    // transmition -> communicator
    connect(m_serialPortTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_udpTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_tcpTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_webSocketTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);

    connect(m_comunicator, &SAKCommunicationTool::errorOccurred, this, &SAKToolBox::errorOccurred);
    // clang-format on

    emit communicatorChanged();
}

void SAKToolBox::open()
{
    if (m_comunicator) {
        for (auto tool : m_tools) {
            tool->start();
        }

        m_isWorking = true;
        emit isWorkingChanged();
        m_comunicator->start();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::close()
{
    if (m_comunicator) {
        m_comunicator->exit();
        m_comunicator->wait();

        for (auto tool : m_tools) {
            tool->exit();
            tool->wait();
        }

        m_isWorking = false;
        emit isWorkingChanged();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::send(const QByteArray& bytes)
{
    m_comunicator->inputBytes(bytes);
}

bool SAKToolBox::isWorking()
{
    return m_isWorking;
}

SAKCommunicationTool* SAKToolBox::getCommunicationTool()
{
    return m_comunicator;
}

SAKEmitterTool* SAKToolBox::getEmitterTool()
{
    return m_emitter;
}

SAKResponserTool* SAKToolBox::getResponserTool()
{
    return m_responser;
}

SAKStorerTool* SAKToolBox::getStorerTool()
{
    return m_storer;
}

SAKPrestorerTool* SAKToolBox::getPrestorerTool()
{
    return m_prestorer;
}

SAKVelometerTool* SAKToolBox::getRxVelometerTool()
{
    return m_rxVelometer;
}

SAKVelometerTool* SAKToolBox::getTxVelometerTool()
{
    return m_txVelometer;
}

SAKStatisticianTool* SAKToolBox::getRxStatisticianTool()
{
    return m_rxCounter;
}

SAKStatisticianTool* SAKToolBox::getTxStatisticianTool()
{
    return m_txCounter;
}

SAKUdpTransmitterTool* SAKToolBox::getUdpTransmitterTool()
{
    return m_udpTransmitter;
}

SAKTcpTransmitterTool* SAKToolBox::getTcpTransmitterTool()
{
    return m_tcpTransmitter;
}

SAKWebSocketTransmitterTool* SAKToolBox::getWebSocketTransmitterTool()
{
    return m_webSocketTransmitter;
}

SAKSerialPortTransmitterTool* SAKToolBox::getSerialPortTransmitterTool()
{
    return m_serialPortTransmitter;
}

void SAKToolBox::uninitializedTips()
{
    static QByteArray tips("You must call the interface name initialize()"
                           " before using the object.");
    Q_ASSERT_X(false, __FUNCTION__, tips.constData());

    emit errorOccurred(QString::fromLatin1(tips));
}

QVariant SAKToolBox::communicator()
{
    return QVariant::fromValue(m_comunicator);
}

QVariant SAKToolBox::emitter()
{
    return QVariant::fromValue(m_emitter);
}

QVariant SAKToolBox::responser()
{
    return QVariant::fromValue(m_responser);
}

QVariant SAKToolBox::storer()
{
    return QVariant::fromValue(m_storer);
}

QVariant SAKToolBox::prestorer()
{
    return QVariant::fromValue(m_prestorer);
}

QVariant SAKToolBox::rxVelometer()
{
    return QVariant::fromValue(m_rxVelometer);
}

QVariant SAKToolBox::txVelometer()
{
    return QVariant::fromValue(m_txVelometer);
}

QVariant SAKToolBox::rxStatistician()
{
    return QVariant::fromValue(m_rxCounter);
}

QVariant SAKToolBox::txStatistician()
{
    return QVariant::fromValue(m_txCounter);
}

QVariant SAKToolBox::udpTransmitter()
{
    return QVariant::fromValue(m_udpTransmitter);
}

QVariant SAKToolBox::tcpTransmitter()
{
    return QVariant::fromValue(m_tcpTransmitter);
}

QVariant SAKToolBox::webSocketTransmitter()
{
    return QVariant::fromValue(m_webSocketTransmitter);
}

QVariant SAKToolBox::serialPortTransmitter()
{
    return QVariant::fromValue(m_serialPortTransmitter);
}

void SAKToolBox::onCommunicatorBytesWritten(const QByteArray& bytes, const QString& to)
{
    Q_UNUSED(to)
    m_txVelometer->inputBytes(bytes);
    m_txCounter->inputBytes(bytes);
    m_storer->inputBytes(bytes);
}

void SAKToolBox::onCommunicatorBytesRead(const QByteArray& bytes, const QString& to)
{
    Q_UNUSED(to)
    m_rxVelometer->inputBytes(bytes);
    m_rxCounter->inputBytes(bytes);
    m_storer->inputBytes(bytes);

    m_responser->inputBytes(bytes);

    m_serialPortTransmitter->inputBytes(bytes);
    m_udpTransmitter->inputBytes(bytes);
    m_tcpTransmitter->inputBytes(bytes);
    m_webSocketTransmitter->inputBytes(bytes);
}
