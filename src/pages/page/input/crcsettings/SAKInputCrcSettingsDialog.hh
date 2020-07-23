/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKINPUTCRCSETTINGDIALOG_HH
#define SAKINPUTCRCSETTINGDIALOG_HH

#include <QMutex>
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>

namespace Ui {
    class SAKInputCrcSettingsDialog;
}

/// @brief crc设置弹窗
class SAKInputCrcSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SAKInputCrcSettingsDialog(QWidget *parent = Q_NULLPTR);
    ~SAKInputCrcSettingsDialog();

    /// @brief crc参数上下文
    struct ParameterContext {
        bool bigEndianCRC;
        quint32 startByte;
        quint32 endByte;    // 结束字节，从右边开始，如结束字节为最后的字节，则该值为1
    };

    /**
     * @brief parameters 获取crc设置参数
     * @return crc设置参数
     */
    ParameterContext parametersContext();
private:
    ParameterContext mParametersContext;
    QMutex mParametersContextMutex;
private:
    Ui::SAKInputCrcSettingsDialog *mUi;
    QCheckBox *mBigEndianCheckBox;
    QSpinBox *mStartSpinBox;
    QSpinBox *mEndSpinBox;
private slots:
    void on_bigEndianCheckBox_clicked();
    void on_startSpinBox_valueChanged(int value);
    void on_endSpinBox_valueChanged(int value);
signals:
    void parametersChanged();
};

#endif
