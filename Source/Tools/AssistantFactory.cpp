/***************************************************************************************************
 * Copyright 2022-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AssistantFactory.h"

#include <QCoreApplication>
#include <QWidget>

#include "AsciiAssistant.h"
#include "Base64Assisatnt.h"
#include "BroadcastAssistant.h"
#include "CrcAssistant.h"
#include "FileCheckAssistant.h"
#include "FileMergeAssistant.h"
#include "NumberAssistant.h"
#ifdef X_TOOLS_ENABLE_MODULE_PING
#include "PingAssistant.h"
#endif
#include "StringAssistant.h"

#ifdef X_TOOLS_ENABLE_MODULE_MDNS
#include "MdnsAssistant.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_QRCODE
#include "QRCodeAssistant.h"
#endif

#ifdef X_TOOLS_ENABLE_MODULE_SERIALBUS
#include "CanBusAssistant.h"
#include "ModbusAssistant.h"
#endif

AssistantFactory::AssistantFactory(QObject* parent)
    : QObject(parent)
{
    addAssistant<CrcAssistant>(AssistantTypeCrc, tr("CRC Assistant"));
#ifdef X_TOOLS_ENABLE_MODULE_MDNS
    addAssistant<MdnsAssistant>(AssistantTypeMdns, tr("mDNS Assistant"));
#endif
#ifdef X_TOOLS_ENABLE_MODULE_PING
    addAssistant<PingAssistant>(AssistantTypePing, tr("Ping Assistant"));
#endif
    addAssistant<AsciiAssistant>(AssistantTypeAscii, tr("ASCII Assistant"));
    addAssistant<Base64Assisatnt>(AssistantTypeBase64, tr("Base64 Assistant"));
    addAssistant<NumberAssistant>(AssistantTypeNumber, tr("Number Assistant"));
    addAssistant<StringAssistant>(AssistantTypeString, tr("String Assistant"));
    addAssistant<BroadcastAssistant>(AssistantTypeBroadcast, tr("Broadcast Assistant"));
    addAssistant<FileCheckAssistant>(AssistantTypeFileCheck, tr("File Check Assistant"));
    addAssistant<FileMergeAssistant>(AssistantTypeFileMerge, tr("File Merge Assistant"));
#ifdef X_TOOLS_ENABLE_MODULE_QRCODE
    addAssistant<QRCodeAssistant>(AssistantTypeQRCode, tr("QR Code Assistant"));
#endif
#ifdef X_TOOLS_ENABLE_MODULE_SERIALBUS
    addAssistant<CanBusAssistant>(AssistantTypeCANBus, tr("CAN Bus Assistant"));
    addAssistant<ModbusAssistant>(AssistantTypeModbus, tr("Modbus Assistant"));
#endif
}

QList<int> AssistantFactory::supportedAssistants()
{
    return m_typeNameMap.keys();
}

QString AssistantFactory::assistantName(int type) const
{
    if (m_typeNameMap.contains(type)) {
        return m_typeNameMap.value(type);
    }

    QString name = QString("UnknownType(%1)").arg(type);
    return name;
}

AssistantFactory* AssistantFactory::instance()
{
    static AssistantFactory* factory = nullptr;
    if (!factory) {
        factory = new AssistantFactory(qApp);
    }

    return factory;
}

QWidget* AssistantFactory::newAssistant(int type)
{
    if (m_metaObjectMap.contains(type)) {
        const QMetaObject meta_obj = m_metaObjectMap.value(type);
        QObject* obj = meta_obj.newInstance();
        QWidget* w = qobject_cast<QWidget*>(obj);
        w->setWindowTitle(assistantName(type));
        return w;
    }

    return Q_NULLPTR;
}
