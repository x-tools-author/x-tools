/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ChartsUiSettings.h"
#include "ui_ChartsUiSettings.h"

#include <QChartView>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPointF>
#include <QTimer>

#include "IO/IO/DataVisualization/2D/Charts.h"

namespace xTools {

ChartsUiSettings::ChartsUiSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartsUiSettings)
{
    ui->setupUi(this);

    ui->comboBoxDataType->addItem("BinaryY", static_cast<int>(Charts::DataFormat::BinaryY));
    ui->comboBoxDataType->addItem("BinaryXY", static_cast<int>(Charts::DataFormat::BinaryXY));
    ui->comboBoxDataType->addItem("TextY", static_cast<int>(Charts::DataFormat::TextY));
    ui->comboBoxDataType->addItem("TextXY", static_cast<int>(Charts::DataFormat::TextXY));

    QGridLayout *parametersGridLayout = new QGridLayout(ui->widgetControl);
    parametersGridLayout->addWidget(new QLabel(tr("Channel"), this), 0, 0, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Visible"), this), 0, 1, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Type"), this), 0, 2, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Color"), this), 0, 3, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Name"), this), 0, 4, Qt::AlignCenter);

    const int channelNumber = channelCount();
    m_channelContexts.resize(channelNumber);
    for (int i = 0; i < channelNumber; ++i) {
        int row = i + 1;
        QString str = QString::number(row);

        parametersGridLayout->addWidget(new QLabel(str, this), row, 0, Qt::AlignCenter);
        auto *visibleCheckedBox = new QCheckBox(this);
        parametersGridLayout->addWidget(visibleCheckedBox, row, 1, Qt::AlignCenter);
        setupVisibleCheckBox(visibleCheckedBox, i);

        auto *typeComboBox = new QComboBox(this);
        parametersGridLayout->addWidget(typeComboBox, row, 2, Qt::AlignCenter);
        setupTypeComboBox(typeComboBox, i);

        auto *colorButton = new QPushButton(this);
        parametersGridLayout->addWidget(colorButton, row, 3, Qt::AlignCenter);
        setupColorButton(colorButton, i);

        auto *nameLineEdit = new QLineEdit(this);
        parametersGridLayout->addWidget(nameLineEdit, row, 4, Qt::AlignCenter);
        setupNameLineEdit(nameLineEdit, i);
    }
}

ChartsUiSettings::~ChartsUiSettings()
{
    delete ui;
}

int ChartsUiSettings::channelCount()
{
    return 16;
}

void ChartsUiSettings::setupVisibleCheckBox(QCheckBox *checkBox, int channelIndex)
{
    checkBox->setCheckable(true);
    checkBox->setChecked(true);
    m_channelContexts[channelIndex].visibleCheckBox = checkBox;
}

void ChartsUiSettings::setupTypeComboBox(QComboBox *comboBox, int channelIndex)
{
    m_channelContexts[channelIndex].typeComboBox = comboBox;
    comboBox->clear();
    comboBox->addItem(QIcon(":/Resources/Icons/IconLineSeries.svg"),
                      seriesTypeToString(LineSeries),
                      LineSeries);
    comboBox->addItem(QIcon(":/Resources/Icons/IconSplineSeries.svg"),
                      seriesTypeToString(SplineSeries),
                      SplineSeries);
    comboBox->addItem(QIcon(":/Resources/Icons/IconScatterSeries.svg"),
                      seriesTypeToString(ScatterSeries),
                      ScatterSeries);
}

void ChartsUiSettings::setupColorButton(QPushButton *button, int channelIndex)
{
    m_channelContexts[channelIndex].colorButton = button;
    button->setFlat(true);
    button->setStyleSheet("background-color: rgb(0, 0, 255);");
}

void ChartsUiSettings::setupNameLineEdit(QLineEdit *lineEdit, int channelIndex)
{
    m_channelContexts[channelIndex].nameLineEdit = lineEdit;
    lineEdit->setText(tr("Channel") + " " + QString::number(channelIndex + 1));
}

QString ChartsUiSettings::seriesTypeToString(int type) const
{
    switch (type) {
    case LineSeries:
        return tr("Line");
    case SplineSeries:
        return tr("Spline");
    case ScatterSeries:
        return tr("Scatter");
    default:
        return QString();
    }
}

} // namespace xTools
