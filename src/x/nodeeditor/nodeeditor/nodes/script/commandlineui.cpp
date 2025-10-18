/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "commandlineui.h"
#include "ui_commandlineui.h"

#include <QFileDialog>

#include "commandline.h"

CommandLineUi::CommandLineUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::CommandLineUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    connect(ui->pushButtonBrowse,
            &QPushButton::clicked,
            this,
            &CommandLineUi::onBrowseButtonClicked);

    CommandLine *cmdLine = qobject_cast<CommandLine *>(node);
    if (cmdLine != nullptr) {
        connect(ui->pushButtonExecute, &QPushButton::clicked, cmdLine, &CommandLine::execute);
    }
}

CommandLineUi::~CommandLineUi()
{
    delete ui;
}

QJsonObject CommandLineUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    CommandLineParametersKeys keys;
    parameters[keys.program] = ui->lineEditProgram->text();
    parameters[keys.arguments] = ui->lineEditArguments->text();
    parameters[keys.isHexParameters] = ui->radioButtonHex->isChecked();
    parameters[keys.isUtf8Parameters] = ui->radioButtonUtf8->isChecked();
    return parameters;
}

void CommandLineUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    CommandLineParametersKeys keys;
    QString program = parameters.value(keys.program).toString();
    QString arguments = parameters.value(keys.arguments).toString();
    bool isHexParameters = parameters.value(keys.isHexParameters).toBool(false);
    bool isUtf8Parameters = parameters.value(keys.isUtf8Parameters).toBool(true);

    ui->lineEditProgram->setText(program);
    ui->lineEditArguments->setText(arguments);
    ui->radioButtonHex->setChecked(isHexParameters);
    ui->radioButtonUtf8->setChecked(isUtf8Parameters);
}

void CommandLineUi::onBrowseButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                    tr("Open File"),
                                                    QString(),
                                                    tr("Executable Files (*.exe)"));
    if (!fileName.isEmpty()) {
        ui->lineEditProgram->setText(fileName);
    }
}
