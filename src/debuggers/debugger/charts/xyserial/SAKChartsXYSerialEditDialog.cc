/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QColorDialog>
#include <QMapIterator>

#include "SAKDebugPage.hh"
#include "SAKChartsXYSerialEditDialog.hh"

#include "ui_SAKChartsXYSerialEditDialog.h"

SAKChartsXYSerialEditDialog::SAKChartsXYSerialEditDialog(QWidget *parent)
    :QDialog (parent)
    ,mUi (new Ui::SAKChartsXYSerialEditDialog)
{
    mUi->setupUi(this);
    mDataTypeComboBox = mUi->dataTypeComboBox;
    mStartByteSpinBox = mUi->startByteSpinBox;
    mBigEndianCheckBox = mUi->bigEndianCheckBox;
    mChartTypeComboBox = mUi->chartTypeComboBox;
    mChartNameLineEdit = mUi->chartNameLineEdit;
    mChartColorPushButton = mUi->chartColorPushButton;

    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt8, QString("int8_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint8, QString("uint8_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt16, QString("int16_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint16, QString("uint16_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt32, QString("int32_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint32, QString("uint32_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt64, QString("int64_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint64, QString("uint64_t"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat32, QString("float"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat64, QString("double"));

    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt8, 1);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint8, 1);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt16, 2);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint16, 2);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt32, 4);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint32, 4);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt64, 8);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint64, 8);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat32, 4);
    mLengthOfDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat64, 8);

    // Initializing data tyep combobox
    QMapIterator<quint32, QString> dataTypeMapInterator(mDataTypeMap);
    while (dataTypeMapInterator.hasNext()) {
        dataTypeMapInterator.next();
        mDataTypeComboBox->addItem(dataTypeMapInterator.value(), QVariant::fromValue(dataTypeMapInterator.key()));
    }

    // Two types of chart
    mChartTypeComboBox->addItem(tr("Line"), ParametersContext::ChartParametersContext::ChartTypeLine);
    mChartTypeComboBox->addItem(tr("Scatter"), ParametersContext::ChartParametersContext::ChartTypeScatter);

    setModal(true);

    // Initializing parameters
    mParametersContext.extractParameters.dataType = ParametersContext::ExtractParametersContext::DataType(mDataTypeComboBox->currentData().toInt());
    mParametersContext.extractParameters.startIndex = mStartByteSpinBox->value();
    mParametersContext.extractParameters.isBigEndian = mBigEndianCheckBox->isChecked();
    mParametersContext.chartParameters.chartName = mChartNameLineEdit->text();
    mParametersContext.chartParameters.chartType = ParametersContext::ChartParametersContext::ChartType( mChartTypeComboBox->currentData().toInt());
    if(!mParametersContext.chartParameters.chartColor.isValid()){
        mParametersContext.chartParameters.chartColor = QString("#0000ff");
        mChartColorPushButton->setStyleSheet(QString("QPushButton{background:%1}").arg(mParametersContext.chartParameters.chartColor.name(QColor::HexRgb)));
    }
}

SAKChartsXYSerialEditDialog::~SAKChartsXYSerialEditDialog()
{
    delete mUi;
}

SAKChartsXYSerialEditDialog::ParametersContext SAKChartsXYSerialEditDialog::parameters()
{
    return mParametersContext;
}

void SAKChartsXYSerialEditDialog::setParameters(ParametersContext ctx)
{
    // Update ui
    for (int i = 0; i < mDataTypeComboBox->count(); i++){
        if (mDataTypeComboBox->itemData(i).toInt() == ctx.extractParameters.dataType){
            mDataTypeComboBox->setCurrentIndex(i);
        }
    }
    mStartByteSpinBox->setValue(ctx.extractParameters.startIndex);
    mBigEndianCheckBox->setChecked(ctx.extractParameters.isBigEndian);

    for (int i = 0; i < mChartTypeComboBox->count(); i++){
        if (mChartTypeComboBox->itemData(i).toInt() == ctx.chartParameters.chartType){
            mChartTypeComboBox->setCurrentIndex(i);
        }
    }
    mChartNameLineEdit->setText(ctx.chartParameters.chartName);
    mChartColorPushButton->setStyleSheet(QString("QPushButton{background:%1}").arg(ctx.chartParameters.chartColor.name(QColor::HexRgb)));

    // Update parameters(not all)
    mParametersContext = ctx;
}

int SAKChartsXYSerialEditDialog::lengthOfDataType(int type)
{
    int len = mLengthOfDataTypeMap.value(type, 0);
    return len;
}

void SAKChartsXYSerialEditDialog::on_dataTypeComboBox_currentIndexChanged(int index)
{
    int type = mDataTypeComboBox->itemData(index).toInt();
    mParametersContext.extractParameters.dataType = ParametersContext::ExtractParametersContext::DataType(type);
}

void SAKChartsXYSerialEditDialog::on_startByteSpinBox_valueChanged(int value)
{
    mParametersContext.extractParameters.startIndex = value;
}

void SAKChartsXYSerialEditDialog::on_bigEndianCheckBox_clicked()
{
    mParametersContext.extractParameters.isBigEndian = mBigEndianCheckBox->isChecked();
}

void SAKChartsXYSerialEditDialog::on_chartTypeComboBox_currentIndexChanged(int index)
{
    int type = mDataTypeComboBox->itemData(index).toInt();
    mParametersContext.chartParameters.chartType = ParametersContext::ChartParametersContext::ChartType(type);
}

void SAKChartsXYSerialEditDialog::on_chartNameLineEdit_textChanged(const QString &text)
{
    mParametersContext.chartParameters.chartName = text.isEmpty() ? QString("Empty") : text;
}

void SAKChartsXYSerialEditDialog::on_chartColorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, tr("Color settings"));
    if (!color.isValid()){
        return;
    }

    // "#000000"
    QString hexRgb = color.name(QColor::HexRgb);
    mParametersContext.chartParameters.chartColor = color;
    mChartColorPushButton->setStyleSheet(QString("QPushButton{background:%1}").arg(hexRgb));
}
