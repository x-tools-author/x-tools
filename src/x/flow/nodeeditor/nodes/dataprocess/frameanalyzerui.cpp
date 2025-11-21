/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "frameanalyzerui.h"
#include "ui_frameanalyzerui.h"

#include <QJsonArray>

#include "frameanalyzer.h"

FrameAnalyzerUi::FrameAnalyzerUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::FrameAnalyzerUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    FrameAnalyzer *frameAnalyzer = qobject_cast<FrameAnalyzer *>(node);
    if (!frameAnalyzer) {
        return;
    }

    connect(ui->pushButtonClearCache, &QPushButton::clicked, this, [frameAnalyzer]() {
        frameAnalyzer->clearCacheBytes();
    });
}

FrameAnalyzerUi::~FrameAnalyzerUi()
{
    delete ui;
}

QJsonObject FrameAnalyzerUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    FrameAnalyzerParametersKeys keys;
    parameters.insert(keys.fixed, ui->checkBoxFixedLength->isChecked());
    parameters.insert(keys.length, ui->spinBoxLength->value());
    parameters.insert(keys.splitFlag, ui->lineEditSplitFlag->text().trimmed());
    parameters.insert(keys.maxCacheBytes, ui->spinBoxMaxCacheBytes->value());
    return parameters;
}

void FrameAnalyzerUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    FrameAnalyzerParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();
    bool fixed = parametersMap.value(keys.fixed, false).toBool();
    QString splitFlag = parametersMap.value(keys.splitFlag, QString("0A")).toString();
    int length = parametersMap.value(keys.length, 8).toInt();
    int maxCacheBytes = parametersMap.value(keys.maxCacheBytes, 1024).toInt();

    ui->checkBoxFixedLength->setChecked(fixed);
    ui->spinBoxLength->setValue(length);
    ui->lineEditSplitFlag->setText(splitFlag);
    ui->spinBoxMaxCacheBytes->setValue(maxCacheBytes);
}
