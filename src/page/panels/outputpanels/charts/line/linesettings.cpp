/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "linesettings.h"
#include "ui_linesettings.h"

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

#include "../chartsview.h"
#include "common/xtools.h"
#include "linepanel.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using namespace QtCharts;
#endif

LineSettings::LineSettings(QWidget *parent)
    : ChartSettings(parent)
    , ui(new Ui::LineSettings)
{
    ui->setupUi(this);

    setupChartsDataFormat(ui->comboBoxDataType);
    ui->checkBoxLegend->setChecked(true);
    connect(ui->comboBoxDataType, xComboBoxActivated, this, [=]() {
        emit this->dataFormatChanged(ui->comboBoxDataType->currentData().toInt());
    });

    connect(ui->checkBoxLegend, &QCheckBox::clicked, this, &LineSettings::invokeSetLegendVisible);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &LineSettings::invokeClearChannels);
    connect(ui->pushButtonImport, &QPushButton::clicked, this, &LineSettings::invokeImportChannels);
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &LineSettings::invokeExportChannels);

    QGridLayout *parametersGridLayout = new QGridLayout(ui->widgetControl);
    parametersGridLayout->addWidget(new QLabel(tr("Channel"), this), 0, 0, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Visible"), this), 0, 1, Qt::AlignCenter);
    auto *typeLabel = new QLabel(tr("Type"), this);
    parametersGridLayout->addWidget(typeLabel, 0, 2, Qt::AlignCenter);
    typeLabel->hide();
    parametersGridLayout->addWidget(new QLabel(tr("Color"), this), 0, 3, Qt::AlignCenter);
    parametersGridLayout->addWidget(new QLabel(tr("Name"), this), 0, 4, Qt::AlignCenter);

    const int channelNumber = channelCount();
    while (m_channelContexts.size() < channelNumber) {
        m_channelContexts.append({nullptr});
    }

    for (int i = 0; i < channelNumber; ++i) {
        int row = i + 1;
        QString str = QString::number(row);
        auto *label = new QLabel(str, this);
        parametersGridLayout->addWidget(label, row, 0, Qt::AlignCenter);

        auto *visibleCheckedBox = new QCheckBox(this);
        parametersGridLayout->addWidget(visibleCheckedBox, row, 1, Qt::AlignCenter);
        setupVisibleCheckBox(visibleCheckedBox, i);

        auto *typeComboBox = new QComboBox(this);
        parametersGridLayout->addWidget(typeComboBox, row, 2, Qt::AlignCenter);
        setupTypeComboBox(typeComboBox, i);
        typeComboBox->hide();

        auto *colorButton = new QPushButton(this);
        parametersGridLayout->addWidget(colorButton, row, 3, Qt::AlignCenter);
        setupColorButton(colorButton, i);

        auto *nameLineEdit = new QLineEdit(this);
        parametersGridLayout->addWidget(nameLineEdit, row, 4, Qt::AlignCenter);
        setupNameLineEdit(nameLineEdit, i);
    }
}

LineSettings::~LineSettings()
{
    delete ui;
}

QVariantMap LineSettings::save() const
{
    return QVariantMap();
}

void LineSettings::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    ChartsUiDataKeys keys;

    setDataType(parameters.value(keys.dataType).toInt());

    bool legendVisible = parameters.value(keys.legendVisible).toBool();
    ui->checkBoxLegend->setChecked(legendVisible);

    int cachePoints = parameters.value(keys.cachePoints).toInt();
    ui->spinBoxCachePoints->setValue(cachePoints);

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
        QString name = obj.value(keys.channelName).toString();
        bool visible = obj.value(keys.channelVisible).toBool();
        QString color = obj.value(keys.channelColor).toString();
        int type = obj.value(keys.channelType).toInt();

        m_channelContexts[i].nameLineEdit->setText(name);
        m_channelContexts[i].visibleCheckBox->setChecked(visible);
        m_channelContexts[i].colorButton->setStyleSheet("background-color: " + color + ";");
        int index = m_channelContexts[i].typeComboBox->findData(type);
        m_channelContexts[i].typeComboBox->setCurrentIndex(index);
    }
}

int LineSettings::channelCount()
{
    return 16;
}

int LineSettings::dataType()
{
    return ui->comboBoxDataType->currentData().toInt();
}

void LineSettings::setDataType(int type)
{
    int index = ui->comboBoxDataType->findData(type);
    if (index != -1) {
        ui->comboBoxDataType->setCurrentIndex(index);
    }
}

bool LineSettings::legendVisible()
{
    return ui->checkBoxLegend->isChecked();
}

void LineSettings::setLegendVisible(bool visible)
{
    Q_UNUSED(visible);
    ui->checkBoxLegend->setChecked(true);
}

int LineSettings::cachePoints()
{
    return ui->spinBoxCachePoints->value();
}

void LineSettings::setCachePoints(int points)
{
    ui->spinBoxCachePoints->setValue(points);
}

void LineSettings::updateUiState(bool ioIsOpened)
{
    ui->comboBoxDataType->setEnabled(!ioIsOpened);
}

void LineSettings::setupVisibleCheckBox(QCheckBox *checkBox, int channelIndex)
{
    checkBox->setCheckable(true);
    checkBox->setChecked(true);
    m_channelContexts[channelIndex].visibleCheckBox = checkBox;
    connect(checkBox, &QCheckBox::clicked, this, [=](bool checked) {
        emit channelVisibleChanged(channelIndex, checked);
    });
}

void LineSettings::setupTypeComboBox(QComboBox *comboBox, int channelIndex)
{
    m_channelContexts[channelIndex].typeComboBox = comboBox;
    comboBox->clear();
    comboBox->addItem(QIcon(":/res/icons/line_series.svg"),
                      seriesTypeToString(QAbstractSeries::SeriesType::SeriesTypeLine),
                      QAbstractSeries::SeriesType::SeriesTypeLine);
    comboBox->addItem(QIcon(":/res/icons/spline_series.svg"),
                      seriesTypeToString(QAbstractSeries::SeriesType::SeriesTypeSpline),
                      QAbstractSeries::SeriesType::SeriesTypeSpline);
    comboBox->addItem(QIcon(":/res/icons/scatter_series.svg"),
                      seriesTypeToString(QAbstractSeries::SeriesType::SeriesTypeScatter),
                      QAbstractSeries::SeriesType::SeriesTypeScatter);
    connect(comboBox, QOverload<int>::of(xComboBoxActivated), this, [=](int index) {
        emit channelTypeChanged(channelIndex, comboBox->itemData(index).toInt());
    });
}

void LineSettings::setupColorButton(QPushButton *button, int channelIndex)
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

void LineSettings::setupNameLineEdit(QLineEdit *lineEdit, int channelIndex)
{
    m_channelContexts[channelIndex].nameLineEdit = lineEdit;
    lineEdit->setText(tr("Channel") + " " + QString::number(channelIndex + 1));

    connect(lineEdit, &QLineEdit::textEdited, this, [=]() {
        emit channelNameChanged(channelIndex, lineEdit->text());
    });
}

QString LineSettings::seriesTypeToString(int type) const
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
