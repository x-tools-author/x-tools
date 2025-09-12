/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "randomgeneratorui.h"
#include "ui_randomgeneratorui.h"

#include <QTimer>

#include "randomgenerator.h"

RandomGeneratorUi::RandomGeneratorUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::RandomGeneratorUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    ui->comboBoxType->addItem("uint32_t", static_cast<int>(RandomGenerator::DataType::Uint32));
    ui->comboBoxType->addItem("uint64_t", static_cast<int>(RandomGenerator::DataType::Uint64));
    ui->comboBoxType->addItem("double", static_cast<int>(RandomGenerator::DataType::Double));

    // clang-format off
    ui->comboBoxFormat->addItem(tr("Binary"), static_cast<int>(RandomGenerator::OutputFormat::Binary));
    ui->comboBoxFormat->addItem(tr("Text"), static_cast<int>(RandomGenerator::OutputFormat::Text));
    // clang-format on

    ui->comboBoxInterval->addItem(tr("Disable"), -1);
    ui->comboBoxInterval->addItem("100", 100);
    ui->comboBoxInterval->addItem("200", 200);
    ui->comboBoxInterval->addItem("500", 500);
    ui->comboBoxInterval->addItem("1000", 1000);
    ui->comboBoxInterval->addItem("2000", 2000);
    ui->comboBoxInterval->addItem("5000", 5000);

    ui->checkBoxBigEndian->setChecked(true);
    connect(ui->pushButtonManual, &QPushButton::clicked, this, &RandomGeneratorUi::output);

    m_cycleTimer = new QTimer(this);
    connect(m_cycleTimer, &QTimer::timeout, this, &RandomGeneratorUi::output);
    connect(ui->comboBoxInterval, &QComboBox::activated, this, &RandomGeneratorUi::updateCycleTimer);
}

RandomGeneratorUi::~RandomGeneratorUi()
{
    delete ui;
}

QJsonObject RandomGeneratorUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    RandomGenerator::DataKeys keys;
    parameters.insert(keys.dataType, ui->comboBoxType->currentData().toInt());
    parameters.insert(keys.outputFormat, ui->comboBoxFormat->currentData().toInt());
    parameters.insert(keys.interval, ui->comboBoxInterval->currentData().toInt());
    parameters.insert(keys.bigEndian, ui->checkBoxBigEndian->isChecked());
    return parameters;
}

void RandomGeneratorUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    RandomGenerator::DataKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();
    RandomGenerator::Data defaultData = RandomGenerator::jsonObject2Data(QJsonObject());
    int type = parametersMap.value(keys.dataType, defaultData.dataType).toInt();
    int format = parametersMap.value(keys.outputFormat, defaultData.outputFormat).toInt();
    int interval = parametersMap.value(keys.interval, defaultData.interval).toInt();
    bool bigEndian = parametersMap.value(keys.bigEndian, defaultData.bigEndian).toBool();

    ui->comboBoxType->setCurrentIndex(ui->comboBoxType->findData(type));
    ui->comboBoxFormat->setCurrentIndex(ui->comboBoxFormat->findData(format));
    ui->comboBoxInterval->setCurrentIndex(ui->comboBoxInterval->findData(interval));
    ui->checkBoxBigEndian->setChecked(bigEndian);
    updateCycleTimer();
}

void RandomGeneratorUi::output()
{
    int dataType = ui->comboBoxType->currentData().toInt();
    int outputFormat = ui->comboBoxFormat->currentData().toInt();
    int interval = ui->comboBoxInterval->currentData().toInt();
    QByteArray bytes = RandomGenerator::generate(dataType, outputFormat, interval);
    m_node->setInData(std::make_shared<BaseNodeData>(bytes), 0);
}

void RandomGeneratorUi::updateCycleTimer()
{
    int currentInterval = this->ui->comboBoxInterval->currentData().toInt();
    if (currentInterval == -1) {
        this->m_cycleTimer->stop();
    } else {
        this->m_cycleTimer->setInterval(currentInterval);
        this->m_cycleTimer->start();
    }
}
