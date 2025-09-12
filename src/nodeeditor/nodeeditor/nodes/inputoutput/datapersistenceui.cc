/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datapersistenceui.h"
#include "ui_datapersistenceui.h"

#include <QDateTime>
#include <QFileDialog>
#include <QPushButton>

#include "common/common.h"
#include "datapersistence.h"

DataPersistenceUi::DataPersistenceUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::DataPersistenceUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    setupTextFormat(ui->comboBoxFormat);

    connect(ui->pushButtonBrowser, &QPushButton::clicked, this, [&]() {
        QString defaultName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        QString fileName
            = QFileDialog::getSaveFileName(nullptr,
                                           "Save file",
                                           defaultName,
                                           tr("txt(*.txt);;bin(*.bin);;All files(*.*)"));
        if (!fileName.isEmpty()) {
            ui->lineEditFilePath->setText(fileName);
        }
    });
}

DataPersistenceUi::~DataPersistenceUi()
{
    delete ui;
}

QJsonObject DataPersistenceUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    DataPersistence::DataKeys keys;

    parameters.insert(keys.fileName, ui->lineEditFilePath->text());
    parameters.insert(keys.format, ui->comboBoxFormat->currentData().toInt());
    parameters.insert(keys.saveDate, ui->checkBoxSaveDate->isChecked());
    parameters.insert(keys.saveTime, ui->checkBoxSaveTime->isChecked());
    parameters.insert(keys.saveMs, ui->checkBoxSaveMs->isChecked());
    parameters.insert(keys.binary, ui->checkBoxBinary->isChecked());

    return parameters;
}

void DataPersistenceUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    DataPersistence::Data data = DataPersistence::jsonObject2Data(parameters);

    ui->comboBoxFormat->setCurrentIndex(ui->comboBoxFormat->findData(data.format));
    ui->lineEditFilePath->setText(data.fileName);
    ui->checkBoxSaveDate->setChecked(data.saveDate);
    ui->checkBoxSaveTime->setChecked(data.saveTime);
    ui->checkBoxSaveMs->setChecked(data.saveMs);
    ui->checkBoxBinary->setChecked(data.binary);
}
