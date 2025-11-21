/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "debugoutputui.h"
#include "ui_debugoutputui.h"

#include <QAction>
#include <QApplication>
#include <QDateTime>
#include <QList>
#include <QMenu>
#include <QToolButton>

#include "common/common.h"
#include "debugoutput.h"

DebugOutputUi::DebugOutputUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::DebugOutputUi)
    , m_channel(-1)
    , m_format(static_cast<int>(TextFormat::Ascii))
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    setupTextFormat(ui->comboBoxFormat);
    for (int i = 0; i < 8; ++i) {
        ui->comboBoxChannel->addItem(QString("O[%1]").arg(QString::number(i + 1)), i);
    }

    connect(node, &DebugOutput::dataUpdated, this, &DebugOutputUi::onDataUpdated);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &DebugOutputUi::onClearButtonClicked);
}

DebugOutputUi::~DebugOutputUi() {}

QJsonObject DebugOutputUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    DebugOutput::DataKeys keys;

    parameters[keys.channel] = ui->comboBoxChannel->currentData().toInt();
    parameters[keys.format] = ui->comboBoxFormat->currentData().toInt();
    parameters[keys.showTime] = ui->checkBoxShowTime->isChecked();
    parameters[keys.flag] = ui->lineEditFlag->text();

    return parameters;
}

void DebugOutputUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    DebugOutput::DataKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();
    int channel = parametersMap.value(keys.channel, 0).toInt();
    int format = parametersMap.value(keys.format, static_cast<int>(TextFormat::Hex)).toInt();
    bool showTime = parametersMap.value(keys.showTime, true).toBool();
    QString flag = parametersMap.value(keys.flag, QString("")).toString();

    ui->comboBoxChannel->setCurrentIndex(ui->comboBoxChannel->findData(channel));
    ui->comboBoxFormat->setCurrentIndex(ui->comboBoxFormat->findData(format));
    ui->checkBoxShowTime->setChecked(showTime);
    ui->lineEditFlag->setText(flag);
}

void DebugOutputUi::onDataUpdated(QtNodes::PortIndex const portIndex)
{
    QList<QWidget *> widgets = qApp->topLevelWidgets();
    for (QWidget *&widget : widgets) {
        if (widget->objectName() != QString("MainWindow")) {
            continue;
        }

        auto data = m_node->outData(portIndex);
        if (!data) {
            return;
        }

        auto cookedData = std::dynamic_pointer_cast<BaseNodeData>(data);
        if (!cookedData) {
            return;
        }

        QByteArray bytes = cookedData->bytes();
        if (bytes.isEmpty()) {
            return;
        }

        int format = ui->comboBoxFormat->currentData().toInt();
        int channel = ui->comboBoxChannel->currentData().toInt();
        QString flag = ui->lineEditFlag->text().trimmed();
        QString dateTime = QString("");
        if (ui->checkBoxShowTime->isChecked()) {
            dateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        }

        QString header = QString("");
        if (dateTime.isEmpty()) {
            if (!flag.isEmpty()) {
                header = QString("[%1] ").arg(flag);
            }
        } else {
            if (flag.isEmpty()) {
                header = QString("[%1] ").arg(dateTime);
            } else {
                header = QString("[%1 %2] ").arg(dateTime, flag);
            }
        }

        QString txt = bytes2string(bytes, format);
        if (!header.isEmpty()) {
            txt = QString("%1%2").arg(header, txt);
        }

        widget->metaObject()->invokeMethod(widget, "outputBytes", txt, channel);
        break;
    }
}

void DebugOutputUi::onClearButtonClicked()
{
    QList<QWidget *> widgets = qApp->topLevelWidgets();
    for (QWidget *&widget : widgets) {
        if (widget->objectName() == "MainWindow") {
            int channel = ui->comboBoxChannel->currentData().toInt();
            widget->metaObject()->invokeMethod(widget, "clearOutput", channel);
        }
    }
}
