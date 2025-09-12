/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

class DataPersistenceThread;
class DataPersistence : public BaseNode
{
    Q_OBJECT
public:
    struct Data
    {
        QString fileName;
        int format{0};
        bool saveDate{false};
        bool saveTime{false};
        bool saveMs{false};
        bool binary{false};
    };

    struct DataKeys
    {
        const QString fileName{"fileName"};
        const QString format{"format"};
        const QString saveDate{"saveDate"};
        const QString saveTime{"saveTime"};
        const QString saveMs{"saveMs"};
        const QString binary{"binary"};
    };

public:
    DataPersistence(QObject *parent = nullptr);
    ~DataPersistence();

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

public:
    static Data jsonObject2Data(const QJsonObject &parameters);
    static QJsonObject data2JsonObject(const Data &data);

private:
    QWidget *m_embeddedWidget{nullptr};
    DataPersistenceThread *m_thread{nullptr};
};
