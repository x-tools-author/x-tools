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
#include <QWidget>

#include "utilities/serializable.h"

namespace xCoAP {

class CoAPServerUiPrivate;
class CoAPServerUi : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPServerUiPrivate* d;

public:
    explicit CoAPServerUi(QWidget* parent = nullptr);
    ~CoAPServerUi();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;
};

} // namespace xCoAP
