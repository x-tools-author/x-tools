/***************************************************************************************************
 * 版权声明 © 2024 声菲特. 保留所有版权。
 **************************************************************************************************/
#include "luascriptui.h"
#include "ui_luascriptui.h"

#include "luascript.h"

LuaScriptUi::LuaScriptUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::LuaScriptUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
}

LuaScriptUi::~LuaScriptUi()
{
    delete ui;
}

QJsonObject LuaScriptUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    LuaScriptParametersKeys keys;
    parameters[keys.script] = ui->textEdit->toPlainText();
    return parameters;
}

void LuaScriptUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    LuaScriptParametersKeys keys;
    QString script = parameters.value(keys.script).toString();
    if (!script.isEmpty()) {
        ui->textEdit->setText(script);
    }
}
