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

namespace xCoAP {

class CoAPResourceModelPrivate;
class CoAPResourceModel : public QAbstractTableModel
{
    Q_OBJECT
    CoAPResourceModelPrivate *d{nullptr};

public:
    explicit CoAPResourceModel(QObject *parent = nullptr);
    ~CoAPResourceModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

} // namespace xCoAP