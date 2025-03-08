/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "transferview.h"

#include "transfermodel.h"

TransferView::TransferView(QWidget *parent)
    : TableView{parent}
{}

TransferView::~TransferView() {}

void TransferView::inputBytes(const QByteArray &bytes)
{
    if (isDisableAll()) {
        return;
    }

    TransferModel *model = qobject_cast<TransferModel *>(tableModel());
    if (model) {
        model->inputBytes(bytes);
    }
}

void TransferView::startAll()
{
    TransferModel *model = qobject_cast<TransferModel *>(tableModel());
    if (model) {
        model->startAll();
    }
}

void TransferView::stopAll()
{
    TransferModel *model = qobject_cast<TransferModel *>(tableModel());
    if (model) {
        model->stopAll();
    }
}