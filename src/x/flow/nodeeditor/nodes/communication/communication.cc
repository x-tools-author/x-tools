/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "communication.h"
#include "communicationui.h"

#include <QCoreApplication>
#include <QTimer>

#include "device/device.h"
#include "device/devicefactory.h"
#include "device/deviceui.h"

Communication::Communication(int type, QObject *parent)
    : BaseNode(parent)
    , m_type(type)
{
    m_handleInDataInThread = true;
    connect(this, &Communication::input2thread, this, &Communication::onInputTothread);

    m_rebootDeviceTimer = new QTimer(this);
    m_rebootDeviceTimer->setInterval(1000);
    m_rebootDeviceTimer->setSingleShot(true);
    connect(m_rebootDeviceTimer, &QTimer::timeout, this, [this] {
        this->m_rebootDeviceTimer->stop();
        if (this->m_device) {
            this->m_device->load(this->m_deviceUi->save());
            this->m_device->start();
        }
    });
}

Communication::~Communication() {}

QString Communication::caption() const
{
    return DeviceFactory::singleton().deviceName(m_type);
}

QString Communication::name() const
{
    return DeviceFactory::singleton().deviceRawName(m_type);
}

unsigned int Communication::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 2;
    } else {
        return 0;
    }
}

QtNodes::NodeDataType Communication::dataType(QtNodes::PortType portType,
                                              QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
        return QtNodes::NodeDataType{QString("QByteArray"), QString("[In]")};
    } else if (portIndex == 0 && portType == QtNodes::PortType::Out) {
        return QtNodes::NodeDataType{QString("QByteArray"), QString("[Rx]")};
    } else if (portIndex == 1 && portType == QtNodes::PortType::Out) {
        return QtNodes::NodeDataType{QString("QByteArray"), QString("[Tx]")};
    }

    return QtNodes::NodeDataType{QString("-"), QString("[-]")};
}

QWidget *Communication::embeddedWidget()
{
    if (QThread::currentThread() != qApp->thread()) {
        return m_embeddedWidget;
    }

    if (m_embeddedWidget == nullptr) {
        m_embeddedWidget = new CommunicationUi(m_type, this);
        m_deviceUi = m_embeddedWidget->deviceUi();
        connect(m_deviceUi,
                &DeviceUi::parametersChanged,
                m_embeddedWidget,
                &CommunicationUi::rebootDevice);

        m_device = m_deviceUi->device();
        connect(m_device, &Device::bytesRead, this, &Communication::onBytesRx);
        connect(m_device, &Device::bytesWritten, this, &Communication::onBytesTx);
        connect(m_device, &Device::finished, this, &Communication::onDeviceClosed);
        connect(m_device, &Device::opened, this, &Communication::onDeviceOpened);
        connect(m_device, &Device::errorOccurred, this, &Communication::onDeviceErrorOccurred);
        connect(m_device, &Device::warningOccurred, this, &Communication::onWarningOccurred);
        m_deviceUi->openDevice();
    }

    return m_embeddedWidget;
}

void Communication::onInputTothread(std::shared_ptr<QtNodes::NodeData> nodeData,
                                    QtNodes::PortIndex const portIndex)
{
    auto baseNodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!baseNodeData) {
        return;
    }

    if (baseNodeData->bytes().isEmpty()) {
        return;
    }

    if (m_device) {
        m_device->writeBytes(baseNodeData->bytes());
    }
}

void Communication::onBytesRx(const QByteArray &bytes, const QString &from)
{
    auto data = std::make_shared<BaseNodeData>(bytes);
    this->setOutData(data);
    emit this->dataUpdated(0);
}

void Communication::onBytesTx(const QByteArray &bytes, const QString &to)
{
    auto data = std::make_shared<BaseNodeData>(bytes);
    this->setOutData(data);
    emit this->dataUpdated(1);
}

void Communication::onDeviceClosed()
{
    m_rebootDeviceTimer->start();
}

void Communication::onDeviceOpened()
{
    if (m_embeddedWidget) {
        m_embeddedWidget->setMessage(tr("Device is working normally."));
    }
}

void Communication::onDeviceErrorOccurred(const QString &error)
{
    if (m_embeddedWidget) {
        m_embeddedWidget->setMessage(error);
    }
}

void Communication::onWarningOccurred(const QString &warning)
{
    if (m_embeddedWidget) {
        m_embeddedWidget->setMessage(warning);
    }
}
