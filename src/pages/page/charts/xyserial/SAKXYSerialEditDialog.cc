/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QMapIterator>

#include "SAKDebugPage.hh"
#include "SAKXYSerialEditDialog.hh"

#include "ui_SAKXYSerialEditDialog.h"

SAKXYSerialEditDialog::SAKXYSerialEditDialog(QWidget *parent)
    :QDialog (parent)
    ,mUi (new Ui::SAKXYSerialEditDialog)
{
    mUi->setupUi(this);
    mDataTypeComboBox = mUi->dataTypeComboBox;
    mStartByteLineEdit = mUi->startByteLineEdit;
    mIsBigEndianCheckBox = mUi->bigEndianCheckBox;
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
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat16, tr("有符号16位浮点数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUfloat16, tr("无符号16位整数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat32, tr("有符号32位浮点数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUfloat32, tr("无符号32位浮点数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeFloat64, tr("有符号64位浮点数"));
    mDataTypeMap.insert(ParametersContext::ExtractParametersContext::DataTypeUfloat64, tr("无符号64位浮点数"));

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
}

SAKXYSerialEditDialog::~SAKXYSerialEditDialog()
{
    delete mUi;
}

void SAKXYSerialEditDialog::editXYSerial()
{
    show();
}

void SAKXYSerialEditDialog::addXYSerial()
{
    show();
}
