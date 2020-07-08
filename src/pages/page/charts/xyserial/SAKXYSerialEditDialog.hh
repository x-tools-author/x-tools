/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKXYSERIALEDITDIALOG_HH
#define SAKXYSERIALEDITDIALOG_HH

#include <QDialog>
#include <QTabWidget>

namespace Ui {
    class SAKXYSerialEditDialog;
};

/// @brief 散点图及曲线图显示类
class SAKXYSerialEditDialog:public QDialog
{
    Q_OBJECT
public:
    SAKXYSerialEditDialog(QWidget *parent = Q_NULLPTR);
    ~SAKXYSerialEditDialog();

    /// @brief 从数据帧中提取图表数据的参数上下文
    struct ExtractParametersContext {
        quint8 startIndex;
        enum DataType {
            DataTypeInt8,
            DataTypeUint8,
            DataTypeInt16,
            DataTypeUint16,
            DataTypeInt32,
            DataTypeUint32,
            DataTypeInt64,
            DataTypeUint64,
            DataTypeFloat16,
            DataTypeUfloat16,
            DataTypeFloat32,
            DataTypeUfloat32,
            DataTypeFloat64,
            DataTypeUfloat64
        }dataType;
    };
private:
    Ui::SAKXYSerialEditDialog *mUi;
};

#endif
