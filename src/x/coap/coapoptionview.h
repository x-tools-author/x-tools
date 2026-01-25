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
#include <QTableView>

#include "utilities/serializable.h"

namespace xCoAP {

class CoAPOptionViewPrivate;
class CoAPOptionView : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPOptionViewPrivate *d{nullptr};

public:
    explicit CoAPOptionView(QWidget *parent = nullptr);
    ~CoAPOptionView() override;

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;
};

} // namespace xCoAP