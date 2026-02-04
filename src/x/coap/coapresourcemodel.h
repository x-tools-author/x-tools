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
#include <QJsonObject>

#define COAP_RES_MODEL_COLUMN_DESCRIPTION 0
#define COAP_RES_MODEL_COLUMN_URI 1
#define COAP_RES_MODEL_COLUMN_FORMAT 2
#define COAP_RES_MODEL_COLUMN_PAYLOAD 3

namespace xCoAP {

class CoAPResourceModelPrivate;
class CoAPResourceModel : public QAbstractTableModel
{
    Q_OBJECT
    CoAPResourceModelPrivate *d{nullptr};

public:
    explicit CoAPResourceModel(QObject *parent = nullptr);
    ~CoAPResourceModel() override;

    void addResource(const QJsonObject &obj);
    void updateResource(int row, const QJsonObject &obj);
    QJsonObject saveRow(int row) const;
    void deleteRow(int row);
    void clearAllRows();
    QStringList uriPaths() const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCoAP