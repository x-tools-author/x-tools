/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/Definitions>

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class DataViewUi;
}

class DataViewUi : public BaseNodeUi
{
    Q_OBJECT
public:
    struct DataKey
    {
        const QString format{"format"};
    };

public:
    DataViewUi(BaseNode *node, QWidget *parent = nullptr);
    ~DataViewUi();

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    void onDataUpdated(QtNodes::PortIndex const portIndex);

private:
    Ui::DataViewUi *ui;
};
