/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "outputsettings.h"
#include "ui_outputsettings.h"

#include <QCheckBox>

OutputSettings::OutputSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OutputSettings)
{
    ui->setupUi(this);
    connect(ui->lineEditHighlighter,
            &QLineEdit::textChanged,
            this,
            &OutputSettings::highlighterKeywordsChanged);
    connect(ui->checkBoxHighlighter,
            &QCheckBox::clicked,
            this,
            &OutputSettings::highlighterEnableChanged);
    connect(ui->checkBoxShowStatistician, &QCheckBox::clicked, this, [this]() {
        if (ui->checkBoxShowStatistician->checkState() == Qt::Checked) {
            emit showStatisticianChanged(true);
        } else {
            emit showStatisticianChanged(false);
        }
    });
}

OutputSettings::~OutputSettings()
{
    delete ui;
}

bool OutputSettings::isEnableFilter() const
{
    return ui->checkBoxFilter->isChecked();
}

bool OutputSettings::isEnableHighlighter() const
{
    return ui->checkBoxHighlighter->isChecked();
}

QString OutputSettings::filterText() const
{
    return ui->lineEditFilter->text();
}

QStringList OutputSettings::highlighterKeywords() const
{
    return ui->lineEditHighlighter->text().split(",", xSkipEmptyParts);
}

QVariantMap OutputSettings::save()
{
    QVariantMap map;
    map.insert("enableFilter", isEnableFilter());
    map.insert("enableHighlighter", isEnableHighlighter());
    map.insert("filterText", ui->lineEditFilter->text());
    map.insert("highlighterKeywords", ui->lineEditHighlighter->text());
    return map;
}

void OutputSettings::load(const QVariantMap &data)
{
    ui->checkBoxFilter->setChecked(data.value("enableFilter").toBool());
    ui->checkBoxHighlighter->setChecked(data.value("enableHighlighter").toBool());
    ui->lineEditFilter->setText(data.value("filterText").toString());
    ui->lineEditHighlighter->setText(data.value("highlighterKeywords").toString());
}
