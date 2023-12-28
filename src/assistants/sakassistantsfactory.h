/***************************************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKASSISTANTSFACTORY_H
#define SAKASSISTANTSFACTORY_H

#include <QMap>
#include <QObject>

class SAKAssistantsFactory : QObject
{
    Q_OBJECT
private:
    SAKAssistantsFactory(QObject* parent = Q_NULLPTR);

private:
    enum AssistantTypes {
        AssistantTypesCrc,
        AssistantTypesFileCheck,
        AssistantTypesAscii,
        AssistantTypesNumber,
        AssistantTypesString,
        AssistantTypesBroadcast,
        AssistantTypesBase64,
        AssistantTypesMdns
    };

public:
    static SAKAssistantsFactory* instance();

    QList<int> supportedAssistants();
    QString assistantName(int type) const;
    QWidget* newAssistant(int type);

private:
    QMap<int, QString> m_typeNameMap;
    QMap<int, QMetaObject> m_metaObjectMap;

private:
    template<typename T>
    void registerAssistant(int type, const QString& assistantName)
    {
        m_typeNameMap.insert(type, assistantName);
        m_metaObjectMap.insert(type, T::staticMetaObject);
    }
};

#endif // SAKASSISTANTSFACTORY_H
