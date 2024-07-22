/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStorerToolUi.h"
#include "ui_xToolsStorerToolUi.h"

#include <QDebug>
#include <QFileDialog>
#include <QLineEdit>

#include "xToolsStorerTool.h"

xToolsStorerToolUi::xToolsStorerToolUi(QWidget *parent)
    : xToolsBaseToolUi{parent}
    , ui(new Ui::xToolsStorerToolUi)
{
    ui->setupUi(this);
    connect(ui->pushButtonSelectFile,
            &QPushButton::clicked,
            this,
            &xToolsStorerToolUi::onPushButtonSelectFileClicked);
}

xToolsStorerToolUi::~xToolsStorerToolUi()
{
    delete ui;
}

void xToolsStorerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    if (!tool) {
        qWarning() << "The tool value is nullptr!";
        return;
    }

    xToolsStorerTool *cookedTool = qobject_cast<xToolsStorerTool *>(tool);
    if (!cookedTool) {
        qWarning() << "The cookedTool value is nullptr!";
        return;
    }

    connect(ui->checkBoxEnable, &QCheckBox::clicked, this, [=]() {
        cookedTool->setIsEnable(ui->checkBoxEnable->isChecked());
    });
    connect(ui->checkBoxRx, &QCheckBox::clicked, this, [=]() {
        cookedTool->setSaveRx(ui->checkBoxRx->isChecked());
    });
    connect(ui->checkBoxTx, &QCheckBox::clicked, this, [=]() {
        cookedTool->setSaveTx(ui->checkBoxTx->isChecked());
    });
    connect(ui->lineEditStorerPath, &xToolsLineEdit::textChanged, this, [=]() {
        cookedTool->setFileName(ui->lineEditStorerPath->text());
    });

    ui->checkBoxEnable->setGroupKey(settingsGroup, "enable");
    ui->checkBoxRx->setGroupKey(settingsGroup, "rx");
    ui->checkBoxTx->setGroupKey(settingsGroup, "tx");
    ui->lineEditStorerPath->setGroupKey(settingsGroup, "path");
    
    cookedTool->setIsEnable(ui->checkBoxEnable->isChecked());
    cookedTool->setSaveTx(ui->checkBoxTx->isChecked());
    cookedTool->setSaveRx(ui->checkBoxRx->isChecked());
    cookedTool->setFileName(ui->lineEditStorerPath->text());
}

void xToolsStorerToolUi::onPushButtonSelectFileClicked()
{
    auto str = QFileDialog::getSaveFileName(Q_NULLPTR,
                                            tr("Save file"),
                                            ".",
                                            tr("txt (*.txt);;All (*)"));
    if (!str.isEmpty()) {
        ui->lineEditStorerPath->setText(str);
    }
}
