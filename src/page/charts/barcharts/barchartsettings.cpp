/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "barchartsettings.h"
#include "ui_barchartsettings.h"

#include <QAbstractSeries>
#include <QChartView>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPointF>
#include <QTimer>

#include "barchartview.h"
#include "common/xtools.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using namespace QtCharts;
#endif

BarChartSettings::BarChartSettings(QWidget *parent)
    : ChartSettings(parent)
    , ui(new Ui::BarChartSettings)
{
    ui->setupUi(this);

    setupChartsDataFormat(ui->comboBoxDataFormat);
    connect(ui->comboBoxDataFormat, xComboBoxActivated, this, [=]() {
        emit dataFormatChanged(ui->comboBoxDataFormat->currentData().toInt());
    });

    QGridLayout *gridTable = ui->gridLayoutTable;
    gridTable->addWidget(new QLabel(tr("Channel"), this), 0, 0);
    gridTable->addWidget(new QLabel(tr("Color"), this), 0, 1);
    gridTable->addWidget(new QLabel(tr("Name"), this), 0, 2);

    const int channelNumber = 16;
    while (m_channelContexts.size() < channelNumber) {
        m_channelContexts.append({nullptr});
    }

    for (int i = 0; i < channelNumber; ++i) {
        int row = i + 1;
        QString str = QString::number(row);
        auto *label = new QLabel(str, this);
        gridTable->addWidget(label, row, 0, Qt::AlignCenter);

        auto *colorButton = new QPushButton(this);
        gridTable->addWidget(colorButton, row, 1);
        setupColorButton(colorButton, i);

        auto *nameLineEdit = new QLineEdit(this);
        gridTable->addWidget(nameLineEdit, row, 2);
        setupNameLineEdit(nameLineEdit, i);
    }
}

BarChartSettings::~BarChartSettings()
{
    delete ui;
}

QVariantMap BarChartSettings::save() const
{
    return QVariantMap();
}

void BarChartSettings::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    BarChartSettingsKeys keys;

    setDataType(parameters.value(keys.dataFormat).toInt());

    QJsonArray channels = parameters.value(keys.channels).toJsonArray();
    int channelCount = channels.count();
    int contextCount = m_channelContexts.size();
    if (channels.count() != m_channelContexts.size()) {
        qWarning("The number(%d) of channels is not equal to the number of series(%d).",
                 channelCount,
                 contextCount);
        return;
    }

    for (int i = 0; i < channels.size(); ++i) {
        QJsonObject obj = channels[i].toObject();
        QString name = obj.value(keys.channel.channelName).toString();
        QString color = obj.value(keys.channel.channelColor).toString();

        m_channelContexts[i].nameLineEdit->setText(name);
        m_channelContexts[i].colorButton->setStyleSheet("background-color: " + color + ";");
    }
}

int BarChartSettings::channelCount()
{
    return 16;
}

int BarChartSettings::dataType()
{
    return ui->comboBoxDataFormat->currentData().toInt();
}

void BarChartSettings::setDataType(int type)
{
    int index = ui->comboBoxDataFormat->findData(type);
    if (index != -1) {
        ui->comboBoxDataFormat->setCurrentIndex(index);
    }
}

void BarChartSettings::updateUiState(bool ioIsOpened)
{
    ui->comboBoxDataFormat->setEnabled(!ioIsOpened);
}

void BarChartSettings::setupColorButton(QPushButton *button, int channelIndex)
{
    m_channelContexts[channelIndex].colorButton = button;
    button->setStyleSheet("background-color: rgb(0, 0, 255);");

    connect(button, &QPushButton::clicked, this, [=]() {
        QColor color = QColorDialog::getColor(Qt::blue, this);
        if (color.isValid()) {
            button->setStyleSheet("background-color: " + color.name() + ";");
            emit channelColorChanged(channelIndex, color);
        }
    });
}

void BarChartSettings::setupNameLineEdit(QLineEdit *lineEdit, int channelIndex)
{
    m_channelContexts[channelIndex].nameLineEdit = lineEdit;
    lineEdit->setText(QString::number(channelIndex + 1));

    connect(lineEdit, &QLineEdit::textEdited, this, [=]() {
        emit channelNameChanged(channelIndex, lineEdit->text());
    });
}

QString BarChartSettings::seriesTypeToString(int type) const
{
    switch (type) {
    case QAbstractSeries::SeriesType::SeriesTypeLine:
        return tr("Line");
    case QAbstractSeries::SeriesType::SeriesTypeSpline:
        return tr("Spline");
    case QAbstractSeries::SeriesType::SeriesTypeScatter:
        return tr("Scatter");
    default:
        return QString();
    }
}
