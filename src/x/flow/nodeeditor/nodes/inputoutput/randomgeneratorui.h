/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class RandomGeneratorUi;
}

class RandomGeneratorUi : public BaseNodeUi
{
    Q_OBJECT
public:
    explicit RandomGeneratorUi(BaseNode *node, QWidget *parent = nullptr);
    ~RandomGeneratorUi() override;

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    void output();
    void updateCycleTimer();

private:
    Ui::RandomGeneratorUi *ui;
    QTimer *m_cycleTimer{nullptr};
};
