/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

#include "utilities/serializable.h"

namespace xCoAP {

class CoAPClientUiPrivate;
class CoAPClientUi : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPClientUiPrivate* d{nullptr};

public:
    explicit CoAPClientUi(QWidget* parent = nullptr);
    ~CoAPClientUi();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;
    QWidget* resourceView() const;
    QWidget* payloadView() const;
    QWidget* optionView() const;
};

} // namespace xCoAP
