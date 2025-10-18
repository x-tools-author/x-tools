/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>

#include "nodeeditor/nodes/common/basenode.h"

class RandomGenerator : public BaseNode
{
    Q_OBJECT
public:
    enum class DataType { Uint32, Uint64, Double };
    enum class OutputFormat { Binary, Text };

    struct Data
    {
        int dataType;
        int outputFormat;
        int interval;
        bool bigEndian;
    };
    struct DataKeys
    {
        const QString dataType{"dataType"};
        const QString outputFormat{"outputFormat"};
        const QString interval{"interval"};
        const QString bigEndian{"bigEndian"};
    };

public:
    RandomGenerator(QObject *parent = nullptr);
    ~RandomGenerator() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

    static QByteArray generate(int dataType, int outputFormat, bool bigEndian);
    static QJsonObject data2jsonObject(const Data &data);
    static Data jsonObject2Data(const QJsonObject &data);
    static QByteArray data2bytes(const Data &data);

private:
    QWidget *m_embeddedWidget{nullptr};
};
