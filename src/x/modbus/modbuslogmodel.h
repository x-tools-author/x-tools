/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QJsonObject>

#define MODBUS_LOG_MODEL_COLUMN_DATE_TIME 0
#define MODBUS_LOG_MODEL_COLUMN_TYPE 1
#define MODBUS_LOG_MODEL_COLUMN_MESSAGE 2

#define xModbusLog xModbus::ModbusLogModel::singleton()

namespace xModbus {

enum LogType {
    LogTypeMsg,
    LogTypeWarning,
    LogTypeError,
    LogTypeRequest,
    LogTypeResponse,
    LogTypeAll = -1
};

QString logTypeToString(LogType type);

class ModbusLogModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    struct LogItem
    {
        QDateTime dateTime;
        LogType type;
        QString message;
    };

private:
    explicit ModbusLogModel(QObject *parent = nullptr);
    ModbusLogModel(const ModbusLogModel &) = delete;
    ModbusLogModel &operator=(const ModbusLogModel &) = delete;

public:
    ~ModbusLogModel() override;

    static ModbusLogModel &singleton();

    void clearLog();
    void addLogThreadSafely(LogType type, const QString &message);
    QList<LogItem> logItems() const;
    void setLogItems(const QList<LogItem> &items);

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    Q_SIGNAL void invokeAddLog(const QDateTime &time, LogType type, const QString &message);

private:
    QList<LogItem> m_logItems;

private:
    void onInvokeAddLog(const QDateTime &time, LogType type, const QString &message);
};

} // namespace xModbus