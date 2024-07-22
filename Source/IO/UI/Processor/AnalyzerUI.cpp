/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsAnalyzerToolUi.h"
#include "ui_xToolsAnalyzerToolUi.h"

#include <QDebug>

#include "xToolsAnalyzerTool.h"
#include "xToolsDataStructure.h"
#include "xToolsApplication.h"

xToolsAnalyzerToolUi::xToolsAnalyzerToolUi(QWidget *parent)
    : xToolsBaseToolUi{parent}
    , ui(new Ui::xToolsAnalyzerToolUi)
{
    ui->setupUi(this);
}

void xToolsAnalyzerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}

void xToolsAnalyzerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    int format = xToolsDataStructure::TextFormatHex;
    xToolsApplication::setValidator(ui->lineEditSeparationMark, format);

    ui->checkBoxEnable->setGroupKey(settingsGroup, "analyzerEnable");
    ui->checkBoxFixedLength->setGroupKey(settingsGroup, "fxiedLength");
    ui->spinBoxFrameLength->setGroupKey(settingsGroup, "frameLength");
    ui->spinBoxMaxTempBytes->setGroupKey(settingsGroup, "maxTempBytes");
    ui->lineEditSeparationMark->setGroupKey(settingsGroup, "separationMark");

    auto cookedTool = qobject_cast<xToolsAnalyzerTool *>(tool);
    static QByteArray tips("invalid SAKAnalyzerTool");
    Q_ASSERT_X(cookedTool, __FUNCTION__, tips.constData());
    if (!cookedTool) {
        qWarning() << QString::fromLatin1(tips);
        return;
    }

    bool enable = ui->checkBoxEnable->isChecked();
    bool fixed = ui->checkBoxFixedLength->isChecked();
    int len = ui->spinBoxFrameLength->value();
    int maxBytes = ui->spinBoxMaxTempBytes->value();
    QString txt = ui->lineEditSeparationMark->text().trimmed();
    QByteArray flag = xToolsDataStructure::stringToByteArray(txt, format);
    
    cookedTool->setIsEnable(enable);
    cookedTool->setFixed(fixed);
    cookedTool->setFrameBytes(len);
    cookedTool->setMaxTempBytes(maxBytes);
    cookedTool->setSeparationMark(flag);

    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=]() {
        bool enable = ui->checkBoxEnable->isChecked();
        cookedTool->setIsEnable(enable);
    });
    connect(ui->checkBoxFixedLength, &QCheckBox::clicked, this, [=]() {
        bool fixed = ui->checkBoxFixedLength->isChecked();
        cookedTool->setFixed(fixed);
    });
    connect(ui->spinBoxFrameLength, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]() {
        int len = ui->spinBoxFrameLength->value();
        cookedTool->setFrameBytes(len);
    });
    connect(ui->spinBoxMaxTempBytes, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]() {
        int maxBytes = ui->spinBoxMaxTempBytes->value();
        cookedTool->setMaxTempBytes(maxBytes);
    });
    connect(ui->lineEditSeparationMark, &QLineEdit::textChanged, this, [=]() {
        QString txt = ui->lineEditSeparationMark->text().trimmed();
        QByteArray flag = xToolsDataStructure::stringToByteArray(txt, format);
        cookedTool->setSeparationMark(flag);
    });
}
