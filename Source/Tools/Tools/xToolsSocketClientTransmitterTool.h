/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsTransmitterTool.h"

class xToolsSocketClientTransmitterTool : public xToolsTransmitterTool
{
    Q_OBJECT
public:
    explicit xToolsSocketClientTransmitterTool(QObject *parent = nullptr);
    virtual QString cookHeaderString(const QString &str) override;
    Q_INVOKABLE virtual QVariant itemContext(int index) override;

public:
    struct ItemContextKeys
    {
        const QString enable{"Enable"};
        const QString clientIp{"ClientIp"};
        const QString clientPort{"ClientPort"};
        const QString specifiedClientIpPort{"SpecifiedClientIpAndPort"};
        const QString serverIp{"ServerIp"};
        const QString serverPort{"ServerPort"};

        const QString messageType{"MessageType"};
    };

protected:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
};
