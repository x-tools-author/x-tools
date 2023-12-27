/********************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
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
    enum Assistants {
        kCrcAssistant,
        kFileCheckAssistant,
        kAsciiAssistant,
        kNumberAssistant,
        kStringAssistant,
        kBroadcastAssistant,
        kBase64Assistant,
        kMdnsAssistant
    };

public:
    static SAKAssistantsFactory* Instance();

    QList<int> SupportedAssistants();
    QString GetAssistantName(int type) const;
    QWidget* NewAssistant(int type);

private:
    QMap<int, QString> type_name_map_;
    QMap<int, QMetaObject> meta_object_map_;

private:
    template<typename T>
    void RegisterAssistantMetaType(int type, const QString& assistant_name)
    {
        type_name_map_.insert(type, assistant_name);
        meta_object_map_.insert(type, T::staticMetaObject);
    }
};

#endif // SAKASSISTANTSFACTORY_H
