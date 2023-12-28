/***************************************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakassistantsfactory.h"
#include "qwidget.h"

#include <QCoreApplication>
#include <QWidget>

#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
#include "sakfilecheckassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
#include "sakcrcassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_NUMBERASSISTANT
#include "saknumberassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
#include "sakstringassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
#include "sakasciiassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
#include "sakbroadcastassistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_BASE64ASSISTANT
#include "sakbase64assistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_MDNSASSISTANT
#include "sakmdnsassistant.h"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject* parent)
    : QObject(parent)
{
#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
    registerAssistant<SAKCRCAssistant>(AssistantTypesCrc, tr("CRC Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
    registerAssistant<SAKFileCheckAssistant>(AssistantTypesFileCheck, tr("File Check Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
    registerAssistant<SAKAsciiAssistant>(AssistantTypesAscii, tr("ASCII Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_NUMBERASSISTANT
    registerAssistant<SAKNumberAssistant>(AssistantTypesNumber, tr("Number Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
    registerAssistant<SAKStringAssistant>(AssistantTypesString, tr("String Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
    registerAssistant<SAKBroadcastAssistant>(AssistantTypesBroadcast, tr("Broadcast Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_BASE64ASSISTANT
    registerAssistant<SAKBase64Assisatnt>(AssistantTypesBase64, tr("Base64 Assistant"));
#endif
#ifdef SAK_IMPORT_MODULE_MDNSASSISTANT
    registerAssistant<SAKMdnsAssistant>(AssistantTypesMdns, tr("MDNS Assistant"));
#endif
}

QList<int> SAKAssistantsFactory::supportedAssistants()
{
    return m_typeNameMap.keys();
}

QString SAKAssistantsFactory::assistantName(int type) const
{
    if (m_typeNameMap.contains(type)) {
        return m_typeNameMap.value(type);
    }

    QString name = QString("UnknowType(%1)").arg(type);
    return name;
}

SAKAssistantsFactory* SAKAssistantsFactory::instance()
{
    static SAKAssistantsFactory* factory = nullptr;
    if (!factory) {
        factory = new SAKAssistantsFactory(qApp);
    }

    return factory;
}

QWidget* SAKAssistantsFactory::newAssistant(int type)
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
