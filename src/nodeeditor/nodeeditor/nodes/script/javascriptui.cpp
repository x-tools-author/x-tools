/***************************************************************************************************
 * 版权声明 © 2024 声菲特. 保留所有版权。
 **************************************************************************************************/
#include "javascriptui.h"
#include "ui_javascriptui.h"

#include <QTextEdit>

#include "javascript.h"

JavaScriptUi::JavaScriptUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::JavaScriptUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
}

JavaScriptUi::~JavaScriptUi()
{
    delete ui;
}

QJsonObject JavaScriptUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    JavaScriptParametersKeys keys;
    parameters.insert(keys.script, ui->textEdit->toPlainText());
    return parameters;
}

void JavaScriptUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    JavaScriptParametersKeys keys;
    QString script = parameters.value(keys.script).toString();
    if (!script.isEmpty()) {
        ui->textEdit->setText(script);
    }
}
