/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QMenu>
#include <QTableView>

#include "utilities/serializable.h"

namespace xCoAP {

class CoAPPayloadEditorPrivate;
class CoAPPayloadEditor : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPPayloadEditorPrivate *d{nullptr};

public:
    explicit CoAPPayloadEditor(QWidget *parent = nullptr);
    ~CoAPPayloadEditor() override;

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;

signals:
    void accepted();
};

} // namespace xCoAP
