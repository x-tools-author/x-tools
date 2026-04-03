/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logmodel.h"

#include <QDateTime>
#include <QLocale>
#include <QMutex>

namespace xLog {

class LogModelPrivate : public QObject
{
public:
    LogModelPrivate(LogModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~LogModelPrivate() override {}

public:
    QList<LogModel::LogMessageItem> m_logMessages;
    QMutex m_logMessagesMutex;

private:
    LogModel* q{nullptr};
};

LogModel::LogModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new LogModelPrivate(this);
}

LogModel::~LogModel() {}

void LogModel::addLogMessage(const LogMessageItem& item)
{
    d->m_logMessagesMutex.lock();
    beginInsertRows(QModelIndex(), d->m_logMessages.size(), d->m_logMessages.size());
    d->m_logMessages.append(item);
    endInsertRows();
    d->m_logMessagesMutex.unlock();
}

void LogModel::clearLogMessages()
{
    d->m_logMessagesMutex.lock();
    beginResetModel();
    d->m_logMessages.clear();
    endResetModel();
    d->m_logMessagesMutex.unlock();
}

QVariant LogModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    if (row < 0 || row >= d->m_logMessages.size()) {
        return QVariant();
    }

    int column = index.column();
    if (column < 0 || column >= columnCount(QModelIndex())) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (column == LOG_MODEL_COLUMN_TIMESTAMP) {
            QString format = QLocale::system().dateTimeFormat(QLocale::FormatType::LongFormat);
            return d->m_logMessages.at(row).timestamp.toString(format);
        } else if (column == LOG_MODEL_COLUMN_LEVEL) {
            switch (d->m_logMessages.at(row).type) {
            case QtMsgType::QtDebugMsg:
                return tr("Debug");
            case QtMsgType::QtInfoMsg:
                return tr("Info");
            case QtMsgType::QtWarningMsg:
                return tr("Warning");
            case QtMsgType::QtCriticalMsg:
                return tr("Critical");
            case QtMsgType::QtFatalMsg:
                return tr("Fatal");
            default:
                return tr("Unknown");
            }
        } else if (column == LOG_MODEL_COLUMN_CATEGORY) {
            return d->m_logMessages.at(row).module;
        } else if (column == LOG_MODEL_COLUMN_MESSAGE) {
            return d->m_logMessages.at(row).message;
        }
    }

    if (role == Qt::EditRole) {
        if (column == LOG_MODEL_COLUMN_TIMESTAMP) {
            return d->m_logMessages.at(row).timestamp;
        } else if (column == LOG_MODEL_COLUMN_LEVEL) {
            return static_cast<int>(d->m_logMessages.at(row).type);
        } else if (column == LOG_MODEL_COLUMN_CATEGORY) {
            return d->m_logMessages.at(row).module;
        } else if (column == LOG_MODEL_COLUMN_MESSAGE) {
            return d->m_logMessages.at(row).message;
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (column == columnCount(QModelIndex()) - 1) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    const LogMessageItem& item = d->m_logMessages.at(row);

    return QVariant();
}

int LogModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return d->m_logMessages.size();
}

int LogModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case LOG_MODEL_COLUMN_TIMESTAMP:
                return tr("Timestamp");
            case LOG_MODEL_COLUMN_LEVEL:
                return tr("Level");
            case LOG_MODEL_COLUMN_CATEGORY:
                return tr("Module");
            case LOG_MODEL_COLUMN_MESSAGE:
                return tr("Message");
            }
        }

        if (role == Qt::TextAlignmentRole) {
            if (section == columnCount(QModelIndex()) - 1) {
                return int(Qt::AlignLeft | Qt::AlignVCenter);
            } else {
                return Qt::AlignCenter;
            }
        }
    } else {
        if (role == Qt::DisplayRole) {
            return QString::number(section + 1);
        }
    }
    return QVariant();
}

} // namespace xLog