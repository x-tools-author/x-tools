/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakanalyzertoolui.h"

#include "sakanalyzertool.h"
#include "sakdatastructure.h"
#include "sakinterface.h"
#include "sakuiinterface.h"
#include "ui_sakanalyzertoolui.h"

SAKAnalyzerToolUi::SAKAnalyzerToolUi(QWidget *parent)
    : SAKBaseToolUi{parent}
    , ui(new Ui::SAKAnalyzerToolUi)
{
    ui->setupUi(this);
}

void SAKAnalyzerToolUi::setToolName(const QString &name)
{
    ui->groupBox->setTitle(name);
}

void SAKAnalyzerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup)
{
    int format = SAKDataStructure::TextFormatHex;
    SAKUiInterface::setValidator(ui->lineEditSeparationMark, format);

    ui->checkBoxEnable->setGroupKey(settingsGroup, "analyzerEnable");
    ui->checkBoxFixedLength->setGroupKey(settingsGroup, "fxiedLength");
    ui->spinBoxFrameLength->setGroupKey(settingsGroup, "frameLength");
    ui->spinBoxMaxTempBytes->setGroupKey(settingsGroup, "maxTempBytes");
    ui->lineEditSeparationMark->setGroupKey(settingsGroup, "separationMark");

    auto cookedTool = qobject_cast<SAKAnalyzerTool *>(tool);
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

    cookedTool->setEnable(enable);
    cookedTool->setFixed(fixed);
    cookedTool->setFrameBytes(len);
    cookedTool->setMaxTempBytes(maxBytes);
    cookedTool->setSeparationMark(flag);

    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=]() {
        bool enable = ui->checkBoxEnable->isChecked();
        cookedTool->setEnable(enable);
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
