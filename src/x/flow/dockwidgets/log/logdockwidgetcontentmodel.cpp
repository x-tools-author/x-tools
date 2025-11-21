/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logdockwidgetcontentmodel.h"

#include <QDateTime>
#include <QtLogging>

LogDockWidgetContentModel::LogDockWidgetContentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
#if 0
    addLog(QtDebugMsg, tr("The is a debug message."));
    addLog(QtInfoMsg, tr("The is a information message."));
    addLog(QtWarningMsg, tr("The is a warning message."));
    addLog(QtCriticalMsg, tr("The is a critical message."));
    addLog(QtFatalMsg, tr("The is a fatal message."));
#endif
}

LogDockWidgetContentModel::~LogDockWidgetContentModel() {}

LogDockWidgetContentModel &LogDockWidgetContentModel::singleton()
{
    static LogDockWidgetContentModel instance;
    return instance;
}

void LogDockWidgetContentModel::addLog(int type, const QString &msg)
{
    LogItem item;
    item.type = type;
    item.msg = msg;
    item.time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    beginInsertRows(QModelIndex(), 0, 0);
    m_logItems.prepend(item);
    endInsertRows();

    if (m_logItems.size() > 10000) {
        beginRemoveRows(QModelIndex(), m_logItems.size() - 1, m_logItems.size() - 1);
        m_logItems.removeFirst();
        endRemoveRows();
    }
}

void LogDockWidgetContentModel::clear()
{
    beginResetModel();
    m_logItems.clear();
    endResetModel();
}

int LogDockWidgetContentModel::rowCount(const QModelIndex &parent) const
{
    return m_logItems.size();
}

int LogDockWidgetContentModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant LogDockWidgetContentModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (row < 0 || row >= m_logItems.size()) {
        return QString("Unknown item row");
    }

    if (role == Qt::DisplayRole) {
        LogItem item = m_logItems.at(row);
        if (column == LOG_COLUMN_TYPE) {
            if (item.type == QtDebugMsg) {
                return QString("[D]");
            } else if (item.type == QtInfoMsg) {
                return QString("[I]");
            } else if (item.type == QtWarningMsg) {
                return QString("[W]");
            } else if (item.type == QtCriticalMsg) {
                return QString("[C]");
            } else if (item.type == QtFatalMsg) {
                return QString("[F]");
            } else {
                return QString("[-]");
            }
        } else if (column == LOG_COLUMN_TIME) {
            return item.time;
        } else if (column == LOG_COLUMN_MSG) {
            return item.msg;
        }
    }

    if (role == Qt::EditRole) {
        LogItem item = m_logItems.at(row);
        if (column == LOG_COLUMN_TYPE) {
            return item.type;
        } else if (column == LOG_COLUMN_TIME) {
            return item.time;
        } else if (column == LOG_COLUMN_MSG) {
            return item.msg;
        }
    }

#if 0
    if (role == Qt::TextAlignmentRole) {
        if (column == LOG_COLUMN_TYPE) {
            return Qt::AlignCenter;
        } else if (column == LOG_COLUMN_TIME) {
            return Qt::AlignCenter;
        } else if (column == LOG_COLUMN_MSG) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
#endif

    return QVariant();
}

QVariant LogDockWidgetContentModel::headerData(int section,
                                               Qt::Orientation orientation,
                                               int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == LOG_COLUMN_TYPE) {
            return tr("Type");
        } else if (section == LOG_COLUMN_TIME) {
            return tr("Time");
        } else if (section == LOG_COLUMN_MSG) {
            return tr("Message");
        }
    }

#if 0
    if (role == Qt::TextAlignmentRole) {
        if (section == LOG_COLUMN_TYPE) {
            return Qt::AlignCenter;
        } else if (section == LOG_COLUMN_TIME) {
            return Qt::AlignCenter;
        } else if (section == LOG_COLUMN_MSG) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
#endif

    return QVariant();
}
