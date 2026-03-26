/***************************************************************************************************
 * Copyright 2023-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 * 
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QVariantMap>

#include "page/common/tableview.h"

class PresetViewPrivate;
class PresetView : public TableView
{
    Q_OBJECT
    PresetViewPrivate *d{nullptr};

public:
    explicit PresetView(QWidget *parent = nullptr);
    ~PresetView();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

signals:
    void outputBytes(const QByteArray &bytes);

protected:
    QList<int> textItemColumns() const override;
    void onCellDoubleClicked(const QModelIndex &index) override;
};
