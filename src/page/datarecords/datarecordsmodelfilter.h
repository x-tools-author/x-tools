/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSortFilterProxyModel>

class DataRecordsModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DataRecordsModelFilter(QObject *parent = nullptr);
    ~DataRecordsModelFilter();

    void setType(int type);
    void setFlag(const QString &flag);
    void setSearchText(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    int m_type;
    QString m_flag;
    QString m_searchText;
};