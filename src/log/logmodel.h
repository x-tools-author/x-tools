/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QDateTime>

#define LOG_MODEL_COLUMN_TIMESTAMP 0
#define LOG_MODEL_COLUMN_LEVEL 1
#define LOG_MODEL_COLUMN_CATEGORY 2
#define LOG_MODEL_COLUMN_MESSAGE 3

namespace xLog {

class LogModelPrivate;
class LogModel : public QAbstractTableModel
{
    Q_OBJECT
    LogModelPrivate *d;

public:
    explicit LogModel(QObject *parent = nullptr);
    ~LogModel() override;

public:
    struct LogMessageItem
    {
        QDateTime timestamp;
        QtMsgType type;
        QString module;
        QString message;
    };
    void addLogMessage(const LogMessageItem &item);
    void clearLogMessages();

public:
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

}; // namespace xLog