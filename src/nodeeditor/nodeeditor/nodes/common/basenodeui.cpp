/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "basenodeui.h"
#include "ui_basenodeui.h"

#include <QLayout>
#include <QPushButton>

#include "basenode.h"

BaseNodeUi::BaseNodeUi(BaseNode *node, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BaseNodeUi)
    , m_node(node)
    , m_innerWidget(nullptr)
{
    ui->setupUi(this);
    ui->toolButtonExpand->setIcon(QIcon(":/res/icons/web_asset.svg"));
    ui->toolButtonFast->setIcon(QIcon(":/res/icons/fast_forward.svg"));
    ui->toolButtonPause->setIcon(QIcon(":/res/icons/pause.svg"));
    ui->toolButtonWarning->setIcon(QIcon(":/res/icons/warning.svg"));

    ui->toolButtonExpand->setCheckable(true);
    ui->toolButtonExpand->setChecked(true);
    ui->toolButtonFast->setCheckable(true);
    ui->toolButtonPause->setCheckable(true);

    ui->toolButtonExpand->setToolTip(tr("Show/hide settings panel..."));
    ui->toolButtonFast->setToolTip(tr("Output the data immediately..."));
    ui->toolButtonPause->setToolTip(tr("Ignore the input data and no data output..."));
    ui->toolButtonWarning->setToolTip(tr("No message..."));

    connect(ui->toolButtonExpand, &QToolButton::clicked, this, &BaseNodeUi::onExpandButtonClicked);
}

BaseNodeUi::~BaseNodeUi()
{
    delete ui;
}

QJsonObject BaseNodeUi::save() const
{
    BaseNode::BaseNodeParametersKeys keys;
    QJsonObject obj;
    obj[keys.expanded] = ui->toolButtonExpand->isChecked();
    obj[keys.fastForward] = ui->toolButtonFast->isChecked();
    obj[keys.paused] = ui->toolButtonPause->isChecked();

    return obj;
}

void BaseNodeUi::load(const QJsonObject &parameters)
{
    BaseNode::BaseNodeParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();

    bool expanded = parametersMap.value(keys.expanded, true).toBool();
    bool fastForward = parametersMap.value(keys.fastForward, false).toBool();
    bool paused = parametersMap.value(keys.paused, false).toBool();

    ui->toolButtonExpand->setChecked(expanded);
    ui->toolButtonFast->setChecked(fastForward);
    ui->toolButtonPause->setChecked(paused);

    if (m_innerWidget) {
        m_innerWidget->setVisible(ui->toolButtonExpand->isChecked());
        adjustSize();
        emit m_node->embeddedWidgetSizeUpdated();
    }
}

void BaseNodeUi::setEmbeddedWidget(QWidget *widget)
{
    m_innerWidget = widget;
    layout()->addWidget(widget);
}

void BaseNodeUi::setMessage(const QString &message)
{
    if (ui) {
        ui->toolButtonWarning->setToolTip(message);
    }
}

void BaseNodeUi::onExpandButtonClicked()
{
    if (!m_innerWidget) {
        return;
    }

    if (ui->toolButtonExpand->isChecked()) {
        m_innerWidget->show();
    } else {
        m_innerWidget->close();
    }

    adjustSize();
    emit m_node->embeddedWidgetSizeUpdated();
}
