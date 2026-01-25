/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#define COAP_MODEL_COLUMN_FLAG 0
#define COAP_MODEL_COLUMN_HOST 1
#define COAP_MODEL_COLUMN_VERSION 2
#define COAP_MODEL_COLUMN_TYPE 3
#define COAP_MODEL_COLUMN_TOKEN_LENGTH 4
#define COAP_MODEL_COLUMN_CODE 5
#define COAP_MODEL_COLUMN_MESSAGE_ID 6
#define COAP_MODEL_COLUMN_TOKEN 7
#define COAP_MODEL_COLUMN_OPTIONS 8
#define COAP_MODEL_COLUMN_PAYLOAD 9

namespace xCoAP {

class CoAPPayloadModelPrivate;
class CoAPPayloadModel : public QAbstractTableModel
{
    Q_OBJECT
    CoAPPayloadModelPrivate *d;

public:
    explicit CoAPPayloadModel(QObject *parent = nullptr);
    ~CoAPPayloadModel() override;

public:
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCoAP