/***************************************************************************************************
 * Copyright 2022-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsAssistantFactory.h"

#include <QCoreApplication>
#include <QWidget>

#include "xToolsAsciiAssistant.h"
#include "xToolsBase64Assisatnt.h"
#include "xToolsBroadcastAssistant.h"
#include "xToolsCRCAssistant.h"
#include "xToolsFileCheckAssistant.h"
#include "xToolsFileMergeAssistant.h"
#include "xToolsNumberAssistant.h"
#include "xToolsPingAssistant.h"
#include "xToolsStringAssistant.h"

#ifdef X_TOOLS_IMPORT_MODULE_MDNS
#include "xToolsMdnsAssistant.h"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject* parent)
    : QObject(parent)
{
    addAssistant<xToolsCRCAssistant>(AssistantTypeCrc, tr("CRC Assistant"));
#ifdef X_TOOLS_IMPORT_MODULE_MDNS
    addAssistant<xToolsMdnsAssistant>(AssistantTypeMdns, tr("mDNS Assistant"));
#endif
    addAssistant<xToolsPingAssistant>(AssistantTypePing, tr("Ping Assistant"));
    addAssistant<xToolsAsciiAssistant>(AssistantTypeAscii, tr("ASCII Assistant"));
    addAssistant<xToolsBase64Assisatnt>(AssistantTypeBase64, tr("Base64 Assistant"));
    addAssistant<xToolsNumberAssistant>(AssistantTypeNumber, tr("Number Assistant"));
    addAssistant<xToolsStringAssistant>(AssistantTypeString, tr("String Assistant"));
    addAssistant<xToolsBroadcastAssistant>(AssistantTypeBroadcast, tr("Broadcast Assistant"));
    addAssistant<xToolsFileCheckAssistant>(AssistantTypeFileCheck, tr("File Check Assistant"));
    addAssistant<xToolsFileMergeAssistant>(AssistantTypeFileMerge, tr("File Merge Assistant"));
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

    QString name = QString("UnknownType(%1)").arg(type);
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
