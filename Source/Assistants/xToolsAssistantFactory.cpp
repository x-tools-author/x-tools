﻿/***************************************************************************************************
 * Copyright 2022-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsAssistantFactory.h"
#include "qwidget.h"

#include <QCoreApplication>
#include <QWidget>

#ifdef X_TOOLS_IMPORT_MODULE_FILECHECKASSISTANT
#include "xToolsFileCheckerAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_CRCASSISTANT
#include "xToolsCRCCalculatorAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_NUMBERASSISTANT
#include "xToolsNumberAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_STRINGASSISTANT
#include "xToolsStringAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_ASCIIASSISTANT
#include "xToolsAsciiAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_BROADCASTASSISTANT
#include "xToolsBroadcastAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_BASE64ASSISTANT
#include "xToolsBase64Assisatnt.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_MDNSASSISTANT
#include "xToolsMdnsAssistant.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_PINGASSISTANT
#include "xToolsPingAssistant.h"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject* parent)
    : QObject(parent)
{
#ifdef X_TOOLS_IMPORT_MODULE_FILECHECKASSISTANT
    registerAssistant<xToolsCRCCalculatorAssistant>(AssistantTypesCrc, tr("CRC Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_CRCASSISTANT
    registerAssistant<xToolsFileCheckerAssistant>(AssistantTypesFileCheck, tr("File Check Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_ASCIIASSISTANT
    registerAssistant<xToolsAsciiAssistant>(AssistantTypesAscii, tr("ASCII Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_NUMBERASSISTANT
    registerAssistant<xToolsNumberAssistant>(AssistantTypesNumber, tr("Number Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_STRINGASSISTANT
    registerAssistant<xToolsStringAssistant>(AssistantTypesString, tr("String Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_BROADCASTASSISTANT
    registerAssistant<xToolsBroadcastAssistant>(AssistantTypesBroadcast, tr("Broadcast Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_BASE64ASSISTANT
    registerAssistant<xToolsBase64Assisatnt>(AssistantTypesBase64, tr("Base64 Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_MDNSASSISTANT
    registerAssistant<xToolsMdnsAssistant>(AssistantTypesMdns, tr("MDNS Assistant"));
#endif
#ifdef X_TOOLS_IMPORT_MODULE_PINGASSISTANT
    registerAssistant<xToolsPingAssistant>(AssistantTypesPing, tr("Ping Assistant"));
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