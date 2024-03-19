/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolBox.h"

#include "saktoolfactory.h"

xToolsToolBox::xToolsToolBox(QObject* parent)
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
                &xToolsToolBox::errorOccurred,
                Qt::ConnectionType(flag));
    }

    connect(this, &xToolsToolBox::errorOccurred, this, [=]() { this->close(); });
}

xToolsToolBox::~xToolsToolBox()
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

void xToolsToolBox::initialize(int type)
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
    connect(m_comunicator, &SAKCommunicationTool::bytesWritten, this, &xToolsToolBox::onCommunicatorBytesWritten);
    connect(m_comunicator, &SAKCommunicationTool::bytesRead, this, &xToolsToolBox::onCommunicatorBytesRead);
    // emiiter,responser,prestorer->tx
    connect(m_emitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_responser, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_prestorer, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);

    // transmition -> communicator
    connect(m_serialPortTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_udpTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_tcpTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);
    connect(m_webSocketTransmitter, &SAKBaseTool::outputBytes, m_comunicator, &SAKBaseTool::inputBytes);

    connect(m_comunicator, &SAKCommunicationTool::errorOccurred, this, &xToolsToolBox::errorOccurred);
    // clang-format on

    emit communicatorChanged();
}

void xToolsToolBox::open()
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

void xToolsToolBox::close()
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

void xToolsToolBox::send(const QByteArray& bytes)
{
    m_comunicator->inputBytes(bytes);
}

bool xToolsToolBox::isWorking()
{
    return m_isWorking;
}

SAKCommunicationTool* xToolsToolBox::getCommunicationTool()
{
    return m_comunicator;
}

SAKEmitterTool* xToolsToolBox::getEmitterTool()
{
    return m_emitter;
}

SAKResponserTool* xToolsToolBox::getResponserTool()
{
    return m_responser;
}

SAKStorerTool* xToolsToolBox::getStorerTool()
{
    return m_storer;
}

SAKPrestorerTool* xToolsToolBox::getPrestorerTool()
{
    return m_prestorer;
}

SAKVelometerTool* xToolsToolBox::getRxVelometerTool()
{
    return m_rxVelometer;
}

SAKVelometerTool* xToolsToolBox::getTxVelometerTool()
{
    return m_txVelometer;
}

SAKStatisticianTool* xToolsToolBox::getRxStatisticianTool()
{
    return m_rxCounter;
}

SAKStatisticianTool* xToolsToolBox::getTxStatisticianTool()
{
    return m_txCounter;
}

SAKUdpTransmitterTool* xToolsToolBox::getUdpTransmitterTool()
{
    return m_udpTransmitter;
}

SAKTcpTransmitterTool* xToolsToolBox::getTcpTransmitterTool()
{
    return m_tcpTransmitter;
}

SAKWebSocketTransmitterTool* xToolsToolBox::getWebSocketTransmitterTool()
{
    return m_webSocketTransmitter;
}

SAKSerialPortTransmitterTool* xToolsToolBox::getSerialPortTransmitterTool()
{
    return m_serialPortTransmitter;
}

void xToolsToolBox::uninitializedTips()
{
    static QByteArray tips("You must call the interface name initialize()"
                           " before using the object.");
    Q_ASSERT_X(false, __FUNCTION__, tips.constData());

    emit errorOccurred(QString::fromLatin1(tips));
}

QVariant xToolsToolBox::communicator()
{
    return QVariant::fromValue(m_comunicator);
}

QVariant xToolsToolBox::emitter()
{
    return QVariant::fromValue(m_emitter);
}

QVariant xToolsToolBox::responser()
{
    return QVariant::fromValue(m_responser);
}

QVariant xToolsToolBox::storer()
{
    return QVariant::fromValue(m_storer);
}

QVariant xToolsToolBox::prestorer()
{
    return QVariant::fromValue(m_prestorer);
}

QVariant xToolsToolBox::rxVelometer()
{
    return QVariant::fromValue(m_rxVelometer);
}

QVariant xToolsToolBox::txVelometer()
{
    return QVariant::fromValue(m_txVelometer);
}

QVariant xToolsToolBox::rxStatistician()
{
    return QVariant::fromValue(m_rxCounter);
}

QVariant xToolsToolBox::txStatistician()
{
    return QVariant::fromValue(m_txCounter);
}

QVariant xToolsToolBox::udpTransmitter()
{
    return QVariant::fromValue(m_udpTransmitter);
}

QVariant xToolsToolBox::tcpTransmitter()
{
    return QVariant::fromValue(m_tcpTransmitter);
}

QVariant xToolsToolBox::webSocketTransmitter()
{
    return QVariant::fromValue(m_webSocketTransmitter);
}

QVariant xToolsToolBox::serialPortTransmitter()
{
    return QVariant::fromValue(m_serialPortTransmitter);
}

void xToolsToolBox::onCommunicatorBytesWritten(const QByteArray& bytes, const QString& to)
{
    Q_UNUSED(to)
    m_txVelometer->inputBytes(bytes);
    m_txCounter->inputBytes(bytes);
    m_storer->inputBytes(bytes);
}

void xToolsToolBox::onCommunicatorBytesRead(const QByteArray& bytes, const QString& to)
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
