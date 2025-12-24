/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbuslogview.h"
#include "ui_modbuslogview.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMessageBox>

#include "modbuslogmodel.h"
#include "modbuslogmodelfilter.h"
#include "utilities/compatibility.h"

namespace xModbus {

struct LogItemKeys
{
    const QString dateTime{"dateTime"};
    const QString type{"type"};
    const QString message{"message"};
};

struct ModbusLogViewKeys
{
    const QString logType{"logType"};
    const QString filterText{"filterText"};
    const QString ignoreDataLog{"ignoreDataLog"};
    const QString usingColor{"usingColor"};
    const QString autoScrolling{"autoScrolling"};
};

ModbusLogView::ModbusLogView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusLogView)
{
    ui->setupUi(this);
    m_logModel = &ModbusLogModel::singleton();
    m_logModelFilter = new ModbusLogModelFilter(this);
    m_logModelFilter->setSourceModel(m_logModel);
    ui->tableView->setModel(m_logModelFilter);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);

    // Scroll to bottom on new log entry
    connect(m_logModel, &QAbstractItemModel::rowsInserted, this, [this]() {
        if (ui->checkBoxAutoScrolling->isChecked()) {
            ui->tableView->scrollToBottom();
        }
    });

    ui->comboBoxLogType->addItem(QObject::tr("All"), LogTypeAll);
    ui->comboBoxLogType->addItem(logTypeToString(LogTypeMsg), LogTypeMsg);
    ui->comboBoxLogType->addItem(logTypeToString(LogTypeWarning), LogTypeWarning);
    ui->comboBoxLogType->addItem(logTypeToString(LogTypeError), LogTypeError);
    ui->comboBoxLogType->addItem(logTypeToString(LogTypeRequest), LogTypeRequest);
    ui->comboBoxLogType->addItem(logTypeToString(LogTypeResponse), LogTypeResponse);

    connect(ui->comboBoxLogType, xComboBoxActivated, this, &ModbusLogView::onLogTypeChanged);
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &ModbusLogView::onFilterTextChanged);
    connect(ui->toolButtonClear, &QToolButton::clicked, this, &ModbusLogView::onClearLogClicked);
    connect(ui->toolButtonSave, &QToolButton::clicked, this, &ModbusLogView::onSaveLogClicked);
    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ModbusLogView::onOpenLogClicked);

    onIgnoreDataLogClicked();
    connect(ui->checkBoxIgnoredDataLog,
            &QCheckBox::clicked,
            this,
            &ModbusLogView::onIgnoreDataLogClicked);
    onUsingColorClicked();
    connect(ui->checkBoxUsingColor, &QCheckBox::clicked, this, &ModbusLogView::onUsingColorClicked);
}

ModbusLogView::~ModbusLogView()
{
    delete ui;
}

QJsonObject ModbusLogView::save()
{
    QJsonObject obj;
    ModbusLogViewKeys keys;
    obj.insert(keys.logType, ui->comboBoxLogType->currentData().toInt());
    obj.insert(keys.filterText, ui->lineEditFilter->text());
    obj.insert(keys.ignoreDataLog, ui->checkBoxIgnoredDataLog->isChecked());
    obj.insert(keys.usingColor, ui->checkBoxUsingColor->isChecked());
    obj.insert(keys.autoScrolling, ui->checkBoxAutoScrolling->isChecked());
    return obj;
}

void ModbusLogView::load(const QJsonObject &obj)
{
    ModbusLogViewKeys keys;
    int type = obj.value(keys.logType).toInt(LogTypeAll);
    int typeIndex = ui->comboBoxLogType->findData(type);
    ui->comboBoxLogType->setCurrentIndex(typeIndex == -1 ? 0 : typeIndex);
    ui->lineEditFilter->setText(obj.value(keys.filterText).toString());
    ui->checkBoxIgnoredDataLog->setChecked(obj.value(keys.ignoreDataLog).toBool(true));
    ui->checkBoxUsingColor->setChecked(obj.value(keys.usingColor).toBool(false));
    ui->checkBoxAutoScrolling->setChecked(obj.value(keys.autoScrolling).toBool(true));

    onIgnoreDataLogClicked();
    onLogTypeChanged();
    onUsingColorClicked();
    onFilterTextChanged(ui->lineEditFilter->text());
}

void ModbusLogView::onLogTypeChanged()
{
    m_logModelFilter->setLogType(ui->comboBoxLogType->currentData().toInt());
}

void ModbusLogView::onFilterTextChanged(const QString &text)
{
    m_logModelFilter->setFilterFixedString(text);
}

void ModbusLogView::onClearLogClicked()
{
    int ret = QMessageBox::question(nullptr,
                                    tr("Clear Log"),
                                    tr("Are you sure you want to clear the log?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    m_logModel->clearLog();
}

void ModbusLogView::onSaveLogClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Log"),
                                                    QString(),
                                                    tr("Log Files (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Save Log"),
                             tr("Failed to open file for writing: %1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    LogItemKeys keys;
    const auto logItems = m_logModel->logItems();
    QJsonArray jsonArray;
    for (const auto &item : logItems) {
        QJsonObject jsonObj;
        jsonObj.insert(keys.dateTime, item.dateTime.toString(Qt::ISODate));
        jsonObj.insert(keys.type, item.type);
        jsonObj.insert(keys.message, item.message);
        jsonArray.append(jsonObj);
    }
    QJsonDocument jsonDoc(jsonArray);
    out << jsonDoc.toJson(QJsonDocument::Indented);
    file.close();
}

void ModbusLogView::onOpenLogClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Log"),
                                                    QString(),
                                                    tr("Log Files (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Open Log"),
                             tr("Failed to open file for reading: %1").arg(file.errorString()));
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this,
                             tr("Open Log"),
                             tr("Failed to parse log file: %1 at offset %2")
                                 .arg(parseError.errorString())
                                 .arg(parseError.offset));
        return;
    }

    if (!jsonDoc.isArray()) {
        QMessageBox::warning(this,
                             tr("Open Log"),
                             tr("Invalid log file format: root element is not an array."));
        return;
    }

    LogItemKeys keys;
    QJsonArray jsonArray = jsonDoc.array();
    QList<ModbusLogModel::LogItem> items;
    for (const QJsonValue &value : std::as_const(jsonArray)) {
        if (!value.isObject()) {
            continue;
        }
        QJsonObject jsonObj = value.toObject();
        QString dateTimeStr = jsonObj.value(keys.dateTime).toString();
        int typeInt = jsonObj.value(keys.type).toInt();
        QString message = jsonObj.value(keys.message).toString();

        QDateTime dateTime = QDateTime::fromString(dateTimeStr, Qt::ISODate);
        LogType type = static_cast<LogType>(typeInt);
        items.append({dateTime, type, message});
    }
    m_logModel->setLogItems(items);
}

void ModbusLogView::onIgnoreDataLogClicked()
{
    m_logModel->setIgnoreDataLog(ui->checkBoxIgnoredDataLog->isChecked());
}

void ModbusLogView::onUsingColorClicked()
{
    m_logModel->setUsingColor(ui->checkBoxUsingColor->isChecked());
}

} // namespace xModbus
