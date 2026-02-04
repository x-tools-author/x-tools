/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

#include "utilities/serializable.h"

namespace xCoAP {

class CoAPResourceEditorPrivate;
class CoAPResourceEditor : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPResourceEditorPrivate *d{nullptr};

public:
    explicit CoAPResourceEditor(QWidget *parent = nullptr);
    ~CoAPResourceEditor() override;

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;

    int editingRow() const;
    void setEditingRow(int row);

signals:
    void accepted();
    void rejected();
};

} // namespace xCoAP