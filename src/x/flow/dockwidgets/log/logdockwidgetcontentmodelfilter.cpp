/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logdockwidgetcontentmodelfilter.h"

#include <QDateTime>
#include <QtLogging>

LogDockWidgetContentModelFilter::LogDockWidgetContentModelFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

LogDockWidgetContentModelFilter::~LogDockWidgetContentModelFilter() {}

void LogDockWidgetContentModelFilter::setMessagesLevel(QtMsgType level)
{
    m_level = level;
    invalidate();
}

bool LogDockWidgetContentModelFilter::filterAcceptsRow(int sourceRow,
                                                       const QModelIndex &sourceParent) const
{
    // Degbug, info, warning, critical, fatal, 注意info的值为4
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid()) {
        return false;
    }

    QtMsgType level = static_cast<QtMsgType>(index.data(Qt::EditRole).toInt());
    if (m_level == QtDebugMsg) {
        return true;
    } else if (m_level == QtInfoMsg) {
        return level == QtInfoMsg || level == QtWarningMsg || level == QtCriticalMsg
               || level == QtFatalMsg;
    } else if (m_level == QtWarningMsg) {
        return level == QtWarningMsg || level == QtCriticalMsg || level == QtFatalMsg;
    } else if (m_level == QtCriticalMsg) {
        return level == QtCriticalMsg || level == QtFatalMsg;
    } else if (m_level == QtFatalMsg) {
        return level == QtFatalMsg;
    } else {
        return false;
    }
}