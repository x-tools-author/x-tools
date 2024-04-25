/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolBox.h"

#include <QDebug>

#include "xToolsToolFactory.h"

xToolsToolBox::xToolsToolBox(QObject* parent)
    : QObject{parent}
{
    auto createTool = [](int type) -> xToolsBaseTool* {
        auto toolFactory = xToolsToolFactory::instance();
        return toolFactory->createTool(type);
    };

    // clang-format off
    m_emitter = qobject_cast<xToolsEmitterTool*>(createTool(xToolsToolFactory::EmitterTool));
    m_responser = qobject_cast<xToolsResponserTool*>(createTool(xToolsToolFactory::ResponserTool));
    m_storer = qobject_cast<xToolsStorerTool*>(createTool(xToolsToolFactory::DataPersistenceTool));
    m_prestorer = qobject_cast<xToolsPrestorerTool*>(createTool(xToolsToolFactory::DataListTool));
    m_rxVelometer = qobject_cast<xToolsVelometerTool*>(createTool(xToolsToolFactory::SpeedometerTool));
    m_txVelometer = qobject_cast<xToolsVelometerTool*>(createTool(xToolsToolFactory::SpeedometerTool));
    m_rxCounter = qobject_cast<xToolsStatisticianTool*>(createTool(xToolsToolFactory::CounterTool));
    m_txCounter = qobject_cast<xToolsStatisticianTool*>(createTool(xToolsToolFactory::CounterTool));
    m_udpTransmitter = qobject_cast<xToolsUdpTransmitterTool*>(createTool(xToolsToolFactory::UdpTransmitterTool));
    m_tcpTransmitter = qobject_cast<xToolsTcpTransmitterTool*>(createTool(xToolsToolFactory::TcpTransmitterTool));
    m_webSocketTransmitter = qobject_cast<xToolsWebSocketTransmitterTool*>( createTool(xToolsToolFactory::WebSocketTransmitterTool));
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    m_serialPortTransmitter = qobject_cast<xToolsSerialPortTransmitterTool*>( createTool(xToolsToolFactory::SerialPortTransmitterTool));
#endif
    // clang-format on

    m_tools << m_emitter << m_responser << m_storer << m_prestorer << m_rxVelometer << m_txVelometer
            << m_rxCounter << m_txCounter << m_udpTransmitter << m_tcpTransmitter
            << m_webSocketTransmitter;
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    m_tools << m_serialPortTransmitter;
#endif

    int flag = Qt::AutoConnection | Qt::UniqueConnection;
    for (auto tool : m_tools) {
        connect(tool,
                &xToolsBaseTool::errorOccurred,
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

    auto toolFactory = xToolsToolFactory::instance();
    auto tool = toolFactory->createTool(type);
    m_comunicator = qobject_cast<xToolsCommunicationTool*>(tool);
    if (!m_comunicator) {
        qWarning() << "mComunicationTool is nullptr, type:" << type;
        return;
    }

    // clang-format off
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_rxCounter, &xToolsStatisticianTool::inputBytes);
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_rxVelometer, &xToolsVelometerTool::inputBytes);
    connect(m_comunicator, &xToolsCommunicationTool::bytesWritten, m_txCounter, &xToolsStatisticianTool::inputBytes);
    connect(m_comunicator, &xToolsCommunicationTool::bytesWritten, m_txVelometer, &xToolsVelometerTool::inputBytes);
    // communicator->responser,txCounter,txVelometer,storer,serialPortTransmitter,udpTransmitter,tcpTransmitter
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_responser, &xToolsResponserTool::inputBytes);
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_serialPortTransmitter, &xToolsSerialPortTransmitterTool::inputBytes);
#endif
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_udpTransmitter, &xToolsUdpTransmitterTool::inputBytes);
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_tcpTransmitter, &xToolsTcpTransmitterTool::inputBytes);
    connect(m_comunicator, &xToolsCommunicationTool::bytesRead, m_webSocketTransmitter, &xToolsWebSocketTransmitterTool::inputBytes);
    // emiiter,responser,prestorer->communicator
    connect(m_emitter, &xToolsBaseTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
    connect(m_responser, &xToolsBaseTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
    connect(m_prestorer, &xToolsBaseTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
    // serialPortTransmitter,udpTransmitter,tcpTransmitter,webSocketTransmitter->communicator
#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
    connect(m_serialPortTransmitter, &xToolsSerialPortTransmitterTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
#endif
    connect(m_udpTransmitter, &xToolsUdpTransmitterTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
    connect(m_tcpTransmitter, &xToolsTcpTransmitterTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);
    connect(m_webSocketTransmitter, &xToolsWebSocketTransmitterTool::outputBytes, m_comunicator, &xToolsCommunicationTool::inputBytes);

    connect(m_comunicator, &xToolsCommunicationTool::errorOccurred, this, &xToolsToolBox::errorOccurred);
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

xToolsCommunicationTool* xToolsToolBox::getCommunicationTool()
{
    return m_comunicator;
}

xToolsEmitterTool* xToolsToolBox::getEmitterTool()
{
    return m_emitter;
}

xToolsResponserTool* xToolsToolBox::getResponserTool()
{
    return m_responser;
}

xToolsStorerTool* xToolsToolBox::getStorerTool()
{
    return m_storer;
}

xToolsPrestorerTool* xToolsToolBox::getPrestorerTool()
{
    return m_prestorer;
}

xToolsVelometerTool* xToolsToolBox::getRxVelometerTool()
{
    return m_rxVelometer;
}

xToolsVelometerTool* xToolsToolBox::getTxVelometerTool()
{
    return m_txVelometer;
}

xToolsStatisticianTool* xToolsToolBox::getRxStatisticianTool()
{
    return m_rxCounter;
}

xToolsStatisticianTool* xToolsToolBox::getTxStatisticianTool()
{
    return m_txCounter;
}

xToolsUdpTransmitterTool* xToolsToolBox::getUdpTransmitterTool()
{
    return m_udpTransmitter;
}

xToolsTcpTransmitterTool* xToolsToolBox::getTcpTransmitterTool()
{
    return m_tcpTransmitter;
}

xToolsWebSocketTransmitterTool* xToolsToolBox::getWebSocketTransmitterTool()
{
    return m_webSocketTransmitter;
}

#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
xToolsSerialPortTransmitterTool* xToolsToolBox::getSerialPortTransmitterTool()
{
    return m_serialPortTransmitter;
}
#endif

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

#ifdef X_TOOLS_ENABLE_MODULE_SERIALPORT
QVariant xToolsToolBox::serialPortTransmitter()
{
    return QVariant::fromValue(m_serialPortTransmitter);
}
#endif

void xToolsToolBox::onCommunicatorBytesWritten(const QByteArray& bytes, const QString& to)
{
    Q_UNUSED(bytes)
    Q_UNUSED(to)
}

void xToolsToolBox::onCommunicatorBytesRead(const QByteArray& bytes, const QString& from)
{
    Q_UNUSED(bytes)
    Q_UNUSED(from)
}
