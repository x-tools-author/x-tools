/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKCHARTSXYSERIALWIDGET_HH
#define SAKCHARTSXYSERIALWIDGET_HH

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>

namespace Ui {
    class SAKChartsXYSerialWidget;
};

class SAKXYSerialEditDialog;
/// @brief 散点图及曲线图显示类
class SAKChartsXYSerialWidget:public QWidget
{
    Q_OBJECT
public:
    SAKChartsXYSerialWidget(QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerialWidget();

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

    /**
     * @brief inputBytes 数据输入接口，参数bytes按照一帧数据来处理
     * @param bytes 数据
     */
    void inputBytes(QByteArray bytes);
private:
    SAKXYSerialEditDialog *mXYSerialEditDialog;
private:
    Ui::SAKChartsXYSerialWidget *mUi;
    QPushButton *mDeletePushButton;
    QPushButton *mEditPushButton;
    QPushButton *mAddPushButton;
private slots:
    void on_editPushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
