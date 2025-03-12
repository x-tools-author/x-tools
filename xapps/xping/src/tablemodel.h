/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(quint32 from, quint32 to, QObject *parent = nullptr);
    ~TableModel() override = default;

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    static QString cookedStatus(int rawStatus);
    static QString cookedCompare(int rawCompare);

public:
    enum RawStatus { RawStatusOnline, RawStatusOffline, RawStatusUnknown };
    Q_ENUM(RawStatus)

    enum RawCompare { RawCompareEqual, RawCompareAdd, RawCompareSubtract, RawCompareUnknown };
    Q_ENUM(RawCompare)

private:
    struct Ctx
    {
        bool isPinging;
        int rawStatus;
        int rawCompare;

        QString ip;
        QString status;
        QString compare;
        QString description;
    };

    quint32 m_from;
    quint32 m_to;
    QList<Ctx> m_data;
};
