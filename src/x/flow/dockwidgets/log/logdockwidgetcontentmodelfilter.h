/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSortFilterProxyModel>

class LogDockWidgetContentModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    LogDockWidgetContentModelFilter(QObject *parent = nullptr);
    ~LogDockWidgetContentModelFilter() override;

    void setMessagesLevel(QtMsgType level);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QtMsgType m_level = QtDebugMsg;
};
