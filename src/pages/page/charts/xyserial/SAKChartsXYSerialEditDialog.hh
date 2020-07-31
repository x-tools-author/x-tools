/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCHARTSXYSERIALEDITDIALOG_HH
#define SAKCHARTSXYSERIALEDITDIALOG_HH

#include <QMap>
#include <QColor>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKChartsXYSerialEditDialog;
};

/// @brief scatter and line serial
class SAKChartsXYSerialEditDialog:public QDialog
{
    Q_OBJECT
public:
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
                DataTypeFloat32,
                DataTypeFloat64,
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
            QColor chartColor;
        }chartParameters;
    };

    SAKChartsXYSerialEditDialog(QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerialEditDialog();

    /**
     * @brief parameters: Get parameters
     * @return parameters
     */
    ParametersContext parameters();

    /**
     * @brief setParameters: Set parameters
     * @param ctx: Parameters
     */
    void setParameters(ParametersContext ctx);

    /**
     * @brief lengthOfDataType: Get the length of data type
     * @param type: Data type
     * @return The length of data type
     */
    int lengthOfDataType(int type);
private:
    QMap<quint32, QString> mDataTypeMap;
    QMap<quint32, int> mLengthOfDataTypeMap;
    ParametersContext mParametersContext;
private:
    Ui::SAKChartsXYSerialEditDialog *mUi;
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
    void on_chartNameLineEdit_textChanged(const QString &text);
    void on_chartColorPushButton_clicked();
};

#endif
