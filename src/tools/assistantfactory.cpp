/***************************************************************************************************
 * Copyright 2022-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "assistantfactory.h"

#include <QCoreApplication>
#include <QWidget>

#include "ascii/asciiassistant.h"
#include "base64/base64assistant.h"
#include "broadcast/broadcastassistant.h"
#include "crc/crcassistant.h"
#include "filecheck/filecheckassistant.h"
#include "filemerge/filemergeassistant.h"
#include "number/numberassistant.h"
#include "ping/pingassistant.h"
#include "string/stringassistant.h"

#if X_ENABLE_ZINT
#include "barcode/barcodeassistant.h"
#endif

#if !defined(X_DISABLE_MDNS)
#include "mdns/mdnsassistant.h"
#endif

#if !defined(X_DISABLE_QR_CODE)
#include "qrcode/qrcodeassistant.h"
#endif

#ifdef X_ENABLE_SERIALBUS
#include "canbus/canbusassistant.h"
#include "modbus/modbusassistant.h"
#endif

AssistantFactory::AssistantFactory(QObject* parent)
    : QObject(parent)
{
    addAssistant<CrcAssistant>(AssistantTypeCrc, tr("CRC Assistant"));
#if !defined(X_DISABLE_MDNS)
    addAssistant<MdnsAssistant>(AssistantTypeMdns, tr("mDNS Assistant"));
#endif
    addAssistant<PingAssistant>(AssistantTypePing, tr("Ping Assistant"));
    addAssistant<AsciiAssistant>(AssistantTypeAscii, tr("ASCII Assistant"));
    addAssistant<Base64Assistant>(AssistantTypeBase64, tr("Base64 Assistant"));
    addAssistant<NumberAssistant>(AssistantTypeNumber, tr("Number Assistant"));
    addAssistant<StringAssistant>(AssistantTypeString, tr("String Assistant"));
    addAssistant<BroadcastAssistant>(AssistantTypeBroadcast, tr("Broadcast Assistant"));
    addAssistant<FileCheckAssistant>(AssistantTypeFileCheck, tr("File Check Assistant"));
    addAssistant<FileMergeAssistant>(AssistantTypeFileMerge, tr("File Merge Assistant"));
#if !defined(X_DISABLE_QR_CODE)
    addAssistant<QRCodeAssistant>(AssistantTypeQRCode, tr("QR Code Assistant"));
#endif
#if X_ENABLE_ZINT
    addAssistant<BarCodeAssistant>(AssistantTypeBarcode, tr("Barcode Assistant"));
#endif
#ifdef X_ENABLE_SERIALBUS
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
