/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "communicationsettings.h"
#include "ui_communicationsettings.h"

#include <QFileDialog>
#include <QStandardPaths>

#include "common/xtools.h"
#include "devicepage/utilities/savethread.h"

const struct
{
    const QString saveToFile = "saveToFile";
    const QString fileName = "fileName";
    const QString saveRx = "saveRx";
    const QString saveTx = "saveTx";
    const QString saveTime = "saveTime";
    const QString saveDate = "saveDate";
    const QString saveMs = "saveMs";
    const QString format = "format";
    const QString maxKBytes = "maxKBytes";
} gKeys;

CommunicationSettings::CommunicationSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CommunicationSettings)
{
    ui->setupUi(this);
    setupTextFormat(ui->comboBoxSaveTextFormat);
    ui->comboBoxMaxBytes->addItem("16K", 16);
    ui->comboBoxMaxBytes->addItem("32K", 32);
    ui->comboBoxMaxBytes->addItem("64K", 64);
    ui->comboBoxMaxBytes->addItem("128K", 128);
    ui->comboBoxMaxBytes->addItem("256K", 256);
    ui->comboBoxMaxBytes->addItem("512K", 512);
    ui->comboBoxMaxBytes->addItem("1M", 1024);
    ui->comboBoxMaxBytes->addItem("2M", 2048);
    ui->comboBoxMaxBytes->addItem("4M", 4096);
    ui->comboBoxMaxBytes->addItem("8M", 8192);
    ui->comboBoxMaxBytes->addItem("16M", 16384);
    ui->comboBoxMaxBytes->addItem("32M", 32768);

    m_saveThread = new SaveThread(this);
    m_saveThread->start();

    connect(ui->pushButtonSavePathBrowser,
            &QPushButton::clicked,
            this,
            &CommunicationSettings::onBrowserButtonClicked);
}

CommunicationSettings::~CommunicationSettings()
{
    delete ui;
}

void CommunicationSettings::saveData(const QByteArray &data, bool isRx)
{
    SaveThread::SaveParameters params;
    params.saveToFile = ui->checkBoxSaveToFile->isChecked();
    params.fileName = m_fileName;
    params.saveRx = ui->checkBoxSaveRx->isChecked();
    params.saveTx = ui->checkBoxSaveTx->isChecked();
    params.saveTime = ui->checkBoxSaveTime->isChecked();
    params.saveDate = ui->checkBoxSaveDate->isChecked();
    params.saveMs = ui->checkBoxSaveMs->isChecked();
    params.format = ui->comboBoxSaveTextFormat->currentData().toInt();
    params.maxKBytes = ui->comboBoxMaxBytes->currentData().toInt();

    m_saveThread->saveData(params, data, isRx);
}

QVariantMap CommunicationSettings::save()
{
    QVariantMap map;
    map[gKeys.saveToFile] = ui->checkBoxSaveToFile->isChecked();
    map[gKeys.fileName] = m_fileName;
    map[gKeys.saveRx] = ui->checkBoxSaveRx->isChecked();
    map[gKeys.saveTx] = ui->checkBoxSaveTx->isChecked();
    map[gKeys.saveTime] = ui->checkBoxSaveTime->isChecked();
    map[gKeys.saveDate] = ui->checkBoxSaveDate->isChecked();
    map[gKeys.saveMs] = ui->checkBoxSaveMs->isChecked();
    map[gKeys.format] = ui->comboBoxSaveTextFormat->currentData().toInt();
    map[gKeys.maxKBytes] = ui->comboBoxMaxBytes->currentData().toInt();
    return map;
}

void CommunicationSettings::load(const QVariantMap &data)
{
    bool saveToFile = data.value(gKeys.saveToFile).toBool();
    m_fileName = data.value(gKeys.fileName).toString();
    bool saveRx = data.value(gKeys.saveRx).toBool();
    bool saveTx = data.value(gKeys.saveTx).toBool();
    bool saveTime = data.value(gKeys.saveTime).toBool();
    bool saveDate = data.value(gKeys.saveDate).toBool();
    bool saveMs = data.value(gKeys.saveMs).toBool();
    int format = data.value(gKeys.format).toInt();
    int maxKBytes = data.value(gKeys.maxKBytes).toInt();

    ui->checkBoxSaveToFile->setChecked(saveToFile);
    ui->checkBoxSaveRx->setChecked(saveRx);
    ui->checkBoxSaveTx->setChecked(saveTx);
    ui->checkBoxSaveTime->setChecked(saveTime);
    ui->checkBoxSaveDate->setChecked(saveDate);
    ui->checkBoxSaveMs->setChecked(saveMs);

    int index = ui->comboBoxSaveTextFormat->findData(format);
    ui->comboBoxSaveTextFormat->setCurrentIndex(index);

    index = ui->comboBoxMaxBytes->findData(maxKBytes);
    ui->comboBoxMaxBytes->setCurrentIndex(index);
}

void CommunicationSettings::addWidgets(QList<QWidget *> widgets)
{
    auto *layout = this->layout();
    if (!layout) {
        qWarning() << "No layout found";
        return;
    }

    for (auto *widget : widgets) {
        layout->addWidget(widget);
    }
}

static const QString settingsFileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/temp.ini").arg(path, QCoreApplication::applicationName());
    return ret;
}

void CommunicationSettings::onBrowserButtonClicked()
{
    QStandardPaths::StandardLocation location = QStandardPaths::DesktopLocation;
    QString defaultPath = QStandardPaths::writableLocation(location);

    auto fileName = QString("%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    QString ret = QFileDialog::getSaveFileName(nullptr,
                                               tr("Save to file"),
                                               defaultPath + "/" + fileName,
                                               tr("Text File(*.txt)"));
    m_fileName = ret;
}
