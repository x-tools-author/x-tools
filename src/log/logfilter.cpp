/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logfilter.h"

#include "logmodel.h"

namespace xLog {

class LogFilterPrivate : public QObject
{
public:
    LogFilterPrivate(LogFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~LogFilterPrivate() override {}

public:
    int m_filterLogLevel{-1};
    QString m_filterCategory{""};
    QString m_filterText{""};

private:
    LogFilter* q{nullptr};
};

LogFilter::LogFilter(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    d = new LogFilterPrivate(this);
}

LogFilter::~LogFilter() {}

void LogFilter::setFilterLogLevel(int level)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    d->m_filterLogLevel = level;
    endFilterChange();
#else
    d->m_filterLogLevel = level;
    invalidateFilter();
#endif
}

void LogFilter::setFilterCategory(const QString& category)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    d->m_filterCategory = category;
    endFilterChange();
#else
    d->m_filterCategory = category;
    invalidateFilter();
#endif
}

void LogFilter::setFilterText(const QString& text)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    d->m_filterText = text;
    endFilterChange();
#else
    d->m_filterText = text;
    invalidateFilter();
#endif
}

bool LogFilter::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QAbstractItemModel* model = sourceModel();
    if (!model) {
        return false;
    }

    LogModel* logModel = qobject_cast<LogModel*>(model);
    if (!logModel) {
        return false;
    }

    if (sourceRow < 0 || sourceRow >= logModel->rowCount(sourceParent)) {
        return false;
    }

    QModelIndex index = logModel->index(sourceRow, LOG_MODEL_COLUMN_LEVEL, sourceParent);
    int logLevel = logModel->data(index, Qt::EditRole).toInt();
    bool visible = true;
    if (d->m_filterLogLevel == QtMsgType::QtDebugMsg) {
        bool v = (logLevel == QtMsgType::QtDebugMsg);
        v |= (logLevel == QtMsgType::QtInfoMsg);
        v |= (logLevel == QtMsgType::QtWarningMsg);
        v |= (logLevel == QtMsgType::QtCriticalMsg);
        v |= (logLevel == QtMsgType::QtFatalMsg);
        visible &= v;
    } else if (d->m_filterLogLevel == QtMsgType::QtInfoMsg) {
        bool v = (logLevel == QtMsgType::QtInfoMsg);
        v |= (logLevel == QtMsgType::QtWarningMsg);
        v |= (logLevel == QtMsgType::QtCriticalMsg);
        v |= (logLevel == QtMsgType::QtFatalMsg);
        visible &= v;
    } else if (d->m_filterLogLevel == QtMsgType::QtWarningMsg) {
        bool v = (logLevel == QtMsgType::QtWarningMsg);
        v |= (logLevel == QtMsgType::QtCriticalMsg);
        v |= (logLevel == QtMsgType::QtFatalMsg);
        visible &= v;
    } else if (d->m_filterLogLevel == QtMsgType::QtCriticalMsg) {
        bool v = (logLevel == QtMsgType::QtCriticalMsg);
        v |= (logLevel == QtMsgType::QtFatalMsg);
        visible &= v;
    } else if (d->m_filterLogLevel == QtMsgType::QtFatalMsg) {
        bool v = (logLevel == QtMsgType::QtFatalMsg);
        visible &= v;
    }

    index = logModel->index(sourceRow, LOG_MODEL_COLUMN_CATEGORY, sourceParent);
    QString logCategory = logModel->data(index, Qt::EditRole).toString();
    if (!d->m_filterCategory.isEmpty() && d->m_filterCategory != QString("default")) {
        visible &= (logCategory == d->m_filterCategory);
    }

    index = logModel->index(sourceRow, LOG_MODEL_COLUMN_MESSAGE, sourceParent);
    QString logMessage = logModel->data(index, Qt::EditRole).toString();
    if (!d->m_filterText.isEmpty()) {
        visible &= logMessage.contains(d->m_filterText, Qt::CaseInsensitive);
    }

    return visible;
}

} // namespace xLog