/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsAnalyzerToolUi.h"
#include "ui_xToolsAnalyzerToolUi.h"

#include "xToolsAnalyzerTool.h"
#include "sakdatastructure.h"
#include "sakinterface.h"
#include "sakuiinterface.h"

xToolsAnalyzerToolUi::xToolsAnalyzerToolUi(QWidget *parent)
    : SAKBaseToolUi{parent}
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
    int format = SAKDataStructure::TextFormatHex;
    SAKUiInterface::setValidator(ui->lineEditSeparationMark, format);

    ui->checkBoxEnable->setGroupKey(settingsGroup, "analyzerEnable");
    ui->checkBoxFixedLength->setGroupKey(settingsGroup, "fxiedLength");
    ui->spinBoxFrameLength->setGroupKey(settingsGroup, "frameLength");
    ui->spinBoxMaxTempBytes->setGroupKey(settingsGroup, "maxTempBytes");
    ui->lineEditSeparationMark->setGroupKey(settingsGroup, "separationMark");

    auto cookedTool = qobject_cast<xToolsAnalyzerTool *>(tool);
    static QByteArray tips("invalid SAKAnalyzerTool");
    Q_ASSERT_X(cookedTool, __FUNCTION__, tips.constData());
    if (!cookedTool) {
        qCWarning((*mLoggingCategory)) << QString::fromLatin1(tips);
        return;
    }

    bool enable = ui->checkBoxEnable->isChecked();
    bool fixed = ui->checkBoxFixedLength->isChecked();
    int len = ui->spinBoxFrameLength->value();
    int maxBytes = ui->spinBoxMaxTempBytes->value();
    QString txt = ui->lineEditSeparationMark->text().trimmed();
    QByteArray flag = SAKInterface::string2array(txt, format);
    
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
    connect(ui->spinBoxFrameLength,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QOverload<const QString &>::of(&QSpinBox::valueChanged),
#else
            &QSpinBox::valueChanged,
#endif
            this,
            [=]() {
                int len = ui->spinBoxFrameLength->value();
                cookedTool->setFrameBytes(len);
            });
    connect(ui->spinBoxMaxTempBytes,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QOverload<const QString &>::of(&QSpinBox::valueChanged),
#else
      &QSpinBox::valueChanged,
#endif
            this,
            [=]() {
                int maxBytes = ui->spinBoxMaxTempBytes->value();
                cookedTool->setMaxTempBytes(maxBytes);
            });
    connect(ui->lineEditSeparationMark, &QLineEdit::textChanged, this, [=]() {
        QString txt = ui->lineEditSeparationMark->text().trimmed();
        QByteArray flag = SAKInterface::string2array(txt, format);
        cookedTool->setSeparationMark(flag);
    });
}
