/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QModbusDataUnit>
#include <QMutex>
#include <QObject>

namespace xModbus {

struct RegisterItemKeys
{
    const QString type{"type"};
    const QString name{"name"};
    const QString unit{"unit"};
    const QString description{"description"};
    const QString address{"address"};
    const QString min{"min"};
    const QString max{"max"};
    const QString decimals{"decimals"};
    const QString value{"value"};
    const QString serverAddress{"serverAddress"};
};

class ModbusRegister : public QObject
{
    Q_OBJECT
public:
    ModbusRegister(QObject *parent = nullptr);
    ModbusRegister(const ModbusRegister &other);
    ModbusRegister &operator=(const ModbusRegister &other) = delete;
    ~ModbusRegister() override;

public:
    void load(const QJsonObject &json);
    QJsonObject save();

public:
    QModbusDataUnit::RegisterType type() const;
    void setType(QModbusDataUnit::RegisterType newType);
    QString name() const;
    void setName(const QString &newName);
    QString unit() const;
    void setUnit(const QString &newUnit);
    QString description() const;
    void setDescription(const QString &newDescription);
    quint16 address() const;
    void setAddress(quint16 newAddress);
    double min() const;
    void setMin(double newMin);
    double max() const;
    void setMax(double newMax);
    int decimals() const;
    void setDecimals(int newDecimals);
    quint16 value() const;
    void setValue(quint16 newValue);
    int serverAddress() const;
    void setServerAddress(int newServerAddress);

signals:
    void typeChanged();
    void nameChanged();
    void unitChanged();
    void descriptionChanged();
    void addressChanged();
    void minChanged();
    void maxChanged();
    void decimalsChanged();
    void valueChanged();
    void serverAddressChanged();

private:
    QModbusDataUnit::RegisterType m_type; // 寄存器类型
    QString m_name;                       // 名称
    QString m_unit;                       // 单位
    QString m_description;                // 描述（说明）
    quint16 m_address;                    // 地址
    double m_min;                         // 最小值
    double m_max;                         // 最大值
    int m_decimals;                       // 小数位
    quint16 m_value;                      // 寄存器值
    int m_serverAddress{1};               // 服务器地址(仅对客户端设备有效)

private:
    mutable QMutex m_valueMutex;
};

} // namespace xModbus