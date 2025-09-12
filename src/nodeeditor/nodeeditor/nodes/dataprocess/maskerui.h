/***************************************************************************************************
 * 版权声明 © 2024 声菲特. 保留所有版权。
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class MaskerUi;
}

class MaskerUi : public BaseNodeUi
{
    Q_OBJECT
public:
    explicit MaskerUi(BaseNode *node, QWidget *parent = nullptr);
    ~MaskerUi() override;

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    Ui::MaskerUi *ui;
};
