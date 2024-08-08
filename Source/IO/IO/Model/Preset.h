/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QMutex>
#include <QTimer>
#include <QVariant>

#include "../../xIO.h"
#include "AbstractModel.h"

namespace xTools {

class Preset : public AbstractModel
{
    Q_OBJECT
public:
    struct Item
    {
        QString itemDescription{"Demo"};
        xIO::TextItemContext textContext;
    };

    struct ItemKeys
    {
        const QString itemDescription{"description"};
        const QString itemTextFormat{"format"};
        const QString itemEscapeCharacter{"escapeCharacter"};
        const QString itemPrefix{"prefix"};
        const QString itemSuffix{"suffix"};
        const QString itemCrcEnable{"crcEnable"};
        const QString itemCrcBigEndian{"crcBigEndian"};
        const QString itemCrcAlgorithm{"crcAlgorithm"};
        const QString itemCrcStartIndex{"crcStartIndex"};
        const QString itemCrcEndIndex{"crcEndIndex"};
        const QString itemText{"text"};
    };

public:
    explicit Preset(QObject *parent = nullptr);

    // clang-format off
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // clang-format on

protected:
    void inputBytes(const QByteArray &bytes) override;
    void run() override;

private:
    QList<Item> m_items;
};

} // namespace xTools
