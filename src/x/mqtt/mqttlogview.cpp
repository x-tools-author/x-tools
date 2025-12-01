/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttlogview.h"
#include "ui_mqttlogview.h"

#include <QTextDocument>

#include "utilities/iconengine.h"
#include "utilities/x.h"

namespace xMQTT {

MqttLogView::MqttLogView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttLogView)
{
    ui->setupUi(this);
    ui->textBrowser->document()->setMaximumBlockCount(1024);
    ui->toolButtonClear->setIcon(xIcon(":/res/icons/mop.svg"));
    xSetNoneBorder(ui->textBrowser);
}

MqttLogView::~MqttLogView()
{
    delete ui;
}

QJsonObject MqttLogView::save()
{
    QJsonObject obj;
    return obj;
}

void MqttLogView::load(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}

void MqttLogView::appendLogMessage(const QString &msg, bool isError)
{
    ui->textBrowser->append(msg);
}

} // namespace xMQTT