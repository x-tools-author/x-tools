/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class CommandLineUi;
}

class CommandLineUi : public BaseNodeUi
{
    Q_OBJECT
public:
    explicit CommandLineUi(BaseNode *node, QWidget *parent = nullptr);
    ~CommandLineUi() override;

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    void onBrowseButtonClicked();

private:
    Ui::CommandLineUi *ui;
};
