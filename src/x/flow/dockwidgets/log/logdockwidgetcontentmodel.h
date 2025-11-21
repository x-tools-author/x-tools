/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#define LOG_COLUMN_TYPE 0
#define LOG_COLUMN_TIME 1
#define LOG_COLUMN_MSG 2

class LogDockWidgetContentModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    LogDockWidgetContentModel(QObject *parent = nullptr);
    LogDockWidgetContentModel(const LogDockWidgetContentModel &) = delete;
    LogDockWidgetContentModel &operator=(const LogDockWidgetContentModel &) = delete;

public:
    ~LogDockWidgetContentModel() override;

    static LogDockWidgetContentModel &singleton();
    void addLog(int type, const QString &msg);
    void clear();

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    struct LogItem
    {
        int type;
        QString msg;
        QString time;
    };

    QList<LogItem> m_logItems;
};
