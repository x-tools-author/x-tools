/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

#include "x/xpage.h"

namespace Ui {
class xMqtt;
}

namespace xMqtt {

class xMqtt : public xPage
{
    Q_OBJECT
public:
    explicit xMqtt(QWidget* parent = nullptr);
    ~xMqtt() override;

    QJsonObject save() const override;
    void load(const QJsonObject& obj) override;

private:
    Ui::xMqtt* ui;

private:
    void onClientButtonClicked(bool checked);
    void onServerButtonClicked(bool checked);
};

} // namespace xMqtt