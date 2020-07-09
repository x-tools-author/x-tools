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

#include <QMap>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKXYSerialEditDialog;
};

/// @brief 散点图及曲线图显示类
class SAKXYSerialEditDialog:public QDialog
{
    Q_OBJECT
public:
    /// @brief 从数据帧中提取图表数据的参数上下文
    struct ParametersContext {
        struct ExtractParametersContext {
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
            quint8 startIndex;
            bool isBigEndian;
        }extractParameters;

        struct ChartParametersContext {
            enum ChartType {
                ChartTypeLine,
                ChartTypeScatter
            }chartType;
            QString chartName;
            QString chartColor;
        }chartParameters;
    };

    SAKXYSerialEditDialog(QWidget *parent = Q_NULLPTR);
    ~SAKXYSerialEditDialog();

    /**
     * @brief setParameters 设置参数上下文，调用该函数会更新ui显示
     * @param ctx 参数上下文
     */
    void setParameters(ParametersContext ctx);
private:
    QMap<quint32, QString> mDataTypeMap;
    ParametersContext mParametersContext;
private:
    Ui::SAKXYSerialEditDialog *mUi;
    QComboBox *mDataTypeComboBox;
    QSpinBox *mStartByteSpinBox;
    QCheckBox *mBigEndianCheckBox;
    QComboBox *mChartTypeComboBox;
    QLineEdit *mChartNameLineEdit;
    QPushButton *mChartColorPushButton;
private slots:
    void on_dataTypeComboBox_currentIndexChanged(int index);
    void on_startByteSpinBox_valueChanged(int value);
    void on_bigEndianCheckBox_clicked();
    void on_chartTypeComboBox_currentIndexChanged(int index);
    void on_chartNameLineEdit_currentTextChanged(const QString &text);
    void on_chartColorPushButton_clicked();
};

#endif
