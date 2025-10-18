/***************************************************************************************************
 * 版权声明 © 2024 声菲特. 保留所有版权。
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class LuaScriptUi;
}

class LuaScriptUi : public BaseNodeUi
{
    Q_OBJECT
public:
    explicit LuaScriptUi(BaseNode *node, QWidget *parent = nullptr);
    ~LuaScriptUi() override;

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    Ui::LuaScriptUi *ui;
};
