/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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

    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt8, tr("有符号8位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint8, tr("无符号8位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt16, tr("有符号16位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint16, tr("无符号16位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt32, tr("有符号32位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint32, tr("无符号32位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeInt64, tr("有符号64位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUint64, tr("无符号64位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat32, tr("有符号32位浮点数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat64, tr("有符号64位浮点数"));

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

    /// @brief 初始化数据类型选择框
    QMapIterator<quint32, QString> dataTypeMapInterator(mDataTypeMap);
    while (dataTypeMapInterator.hasNext()) {
        dataTypeMapInterator.next();
        mDataTypeComboBox->addItem(dataTypeMapInterator.value(), QVariant::fromValue(dataTypeMapInterator.key()));
    }

    /// @brief 初始化图表类型选择框
    mChartTypeComboBox->addItem(tr("曲线图"), ParametersContext::ChartParametersContext::ChartTypeLine);
    mChartTypeComboBox->addItem(tr("散点图"), ParametersContext::ChartParametersContext::ChartTypeScatter);

    /// @brief 设置该窗口为模态弹窗
    setModal(true);

    /// @brief 初始化参数
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
    /// @brief 更新ui
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

    /// @brief 更新参数（更新ui的同时会更新部分参数，但不是全部）
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
    QColor color = QColorDialog::getColor(Qt::white, this, tr("颜色设置"));
    if (!color.isValid()){
        return;
    }

    /// @brief "#000000"
    QString hexRgb = color.name(QColor::HexRgb);
    mParametersContext.chartParameters.chartColor = color;
    mChartColorPushButton->setStyleSheet(QString("QPushButton{background:%1}").arg(hexRgb));
}
