/***************************************************************************************************
 * Copyright 2022-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMap>
#include <QObject>

class AssistantFactory : QObject
{
    Q_OBJECT
private:
    AssistantFactory(QObject* parent = Q_NULLPTR);

private:
    enum AssistantType {
        AssistantTypeCrc,
        AssistantTypeFileCheck,
        AssistantTypeAscii,
        AssistantTypeNumber,
        AssistantTypeString,
        AssistantTypeBroadcast,
        AssistantTypeBase64,
        AssistantTypeMdns,
        AssistantTypePing,
        AssistantTypeFileMerge,
        AssistantTypeQRCode,
        AssistantTypeBarcode,
        AssistantTypeCANBus = 100,
        AssistantTypeModbus
    };

public:
    static AssistantFactory* instance();

    QList<int> supportedAssistants();
    QString assistantName(int type) const;
    QWidget* newAssistant(int type);

private:
    QMap<int, QString> m_typeNameMap;
    QMap<int, QMetaObject> m_metaObjectMap;

private:
    template<typename T>
    void addAssistant(int type, const QString& assistantName)
    {
        m_typeNameMap.insert(type, assistantName);
        m_metaObjectMap.insert(type, T::staticMetaObject);
    }
};
