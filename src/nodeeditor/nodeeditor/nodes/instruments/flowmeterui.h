/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class FlowmeterUi;
}

class FlowmeterUi : public BaseNodeUi
{
    Q_OBJECT
public:
    explicit FlowmeterUi(BaseNode *node, QWidget *parent = nullptr);
    ~FlowmeterUi() override;
    void addBytes(quint64 bytes);

private:
    void updateSpeedLabel();

private:
    Ui::FlowmeterUi *ui;
    quint64 m_bytes{0};
};
