/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "randomgenerator.h"
#include "randomgeneratorui.h"

#include <QRandomGenerator>

RandomGenerator::RandomGenerator(QObject *parent)
    : BaseNode(parent)
{}

RandomGenerator::~RandomGenerator() {}

QString RandomGenerator::caption() const
{
    return tr("Random Generator");
}

QString RandomGenerator::name() const
{
    return QString("RandomGenerator");
}

unsigned int RandomGenerator::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 0;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *RandomGenerator::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new RandomGeneratorUi(this);
    }

    return m_embeddedWidget;
}

template<typename T>
QByteArray generateRandom(T value, int dataType, int outputFormat, bool bigEndian)
{
    QByteArray bytes;
    if (outputFormat == static_cast<int>(RandomGenerator::OutputFormat::Text)) {
        bytes = QString::number(value).toLatin1();
    } else {
        bytes = QByteArray::number(value);
    }

    if (bigEndian) {
        std::reverse(bytes.begin(), bytes.end());
    }

    return bytes;
}

QByteArray RandomGenerator::generate(int dataType, int outputFormat, bool bigEndian)
{
    switch (dataType) {
    case static_cast<int>(DataType::Uint32): {
        quint32 value = QRandomGenerator::global()->generate();
        QByteArray bytes = generateRandom<quint32>(value, dataType, outputFormat, bigEndian);
        return bytes;
    }
    case static_cast<int>(DataType::Uint64): {
        quint64 value = QRandomGenerator::global()->generate64();
        QByteArray bytes = generateRandom<quint64>(value, dataType, outputFormat, bigEndian);
        return bytes;
    }
    case static_cast<int>(DataType::Double): {
        double value = QRandomGenerator::global()->generateDouble();
        QByteArray bytes = generateRandom<double>(value, dataType, outputFormat, bigEndian);
        return bytes;
    }
    default:
        return QByteArray();
        break;
    }
}

QJsonObject RandomGenerator::data2jsonObject(const RandomGenerator::Data &data)
{
    DataKeys keys;
    QJsonObject obj;
    obj[keys.dataType] = data.dataType;
    obj[keys.outputFormat] = data.outputFormat;
    obj[keys.bigEndian] = data.bigEndian;
    obj[keys.interval] = data.interval;
    return obj;
}

RandomGenerator::Data RandomGenerator::jsonObject2Data(const QJsonObject &data)
{
    static Data defaultData{
        .dataType = static_cast<int>(DataType::Uint32),
        .outputFormat = static_cast<int>(OutputFormat::Binary),
        .interval = 1000,
        .bigEndian = true,
    };

    QVariantMap tmp = data.toVariantMap();
    DataKeys keys;
    Data savedData;
    savedData.dataType = tmp.value(keys.dataType, defaultData.dataType).toInt();
    savedData.outputFormat = tmp.value(keys.outputFormat, defaultData.outputFormat).toInt();
    savedData.bigEndian = tmp.value(keys.bigEndian, defaultData.bigEndian).toBool();
    savedData.interval = tmp.value(keys.interval, defaultData.interval).toInt();
    return savedData;
}

QByteArray RandomGenerator::data2bytes(const RandomGenerator::Data &data)
{
    return generate(data.dataType, data.outputFormat, data.bigEndian);
}
