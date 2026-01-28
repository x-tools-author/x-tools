/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

#include "utilities/serializable.h"

namespace Ui {
class xOpcUa;
}

namespace xOpcUa {

class xOpcUa : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit xOpcUa(QWidget* parent = nullptr);
    ~xOpcUa();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;

private:
    Ui::xOpcUa* ui;
};

} // namespace xOpcUa