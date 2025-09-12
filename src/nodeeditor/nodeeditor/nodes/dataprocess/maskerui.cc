/***************************************************************************************************
 * 版权声明 © 2024 声菲特. 保留所有版权。
 **************************************************************************************************/
#include "maskerui.h"
#include "ui_maskerui.h"

#include "masker.h"

MaskerUi::MaskerUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::MaskerUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
    ui->spinBoxMask->setValue(255);
}

MaskerUi::~MaskerUi()
{
    delete ui;
}

QJsonObject MaskerUi::save() const
{
    MaskerParametersKeys keys;
    QVariantMap parameters;
    parameters.insert(keys.mask, ui->spinBoxMask->value());
    return QJsonObject::fromVariantMap(parameters);
}

void MaskerUi::load(const QJsonObject &parameters)
{
    MaskerParametersKeys keys;
    ui->spinBoxMask->setValue(parameters.value(keys.mask).toInt());
}
