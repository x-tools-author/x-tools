/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

class Device;
class DeviceUi;
class CommunicationUi;
class Communication : public BaseNode
{
    Q_OBJECT
public:
    Communication(int type, QObject *parent = nullptr);
    ~Communication();

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType,
                                   QtNodes::PortIndex portIndex) const override;
    QWidget *embeddedWidget() override;

private:
    void onInputTothread(std::shared_ptr<QtNodes::NodeData> nodeData,
                         QtNodes::PortIndex const portIndex);
    void onBytesRx(const QByteArray &bytes, const QString &from);
    void onBytesTx(const QByteArray &bytes, const QString &to);
    void onDeviceClosed();
    void onDeviceOpened();
    void onDeviceErrorOccurred(const QString &error);
    void onWarningOccurred(const QString &warning);

private:
    int m_type;
    Device *m_device{nullptr};
    DeviceUi *m_deviceUi{nullptr};
    CommunicationUi *m_embeddedWidget{nullptr};
    QTimer *m_rebootDeviceTimer{nullptr};
};
