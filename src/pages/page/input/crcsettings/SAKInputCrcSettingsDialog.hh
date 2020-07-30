/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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

/// @brief crc settings dialog
class SAKInputCrcSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SAKInputCrcSettingsDialog(QWidget *parent = Q_NULLPTR);
    ~SAKInputCrcSettingsDialog();

    struct ParameterContext {
        bool bigEndianCRC;
        quint32 startByte;
        quint32 endByte;    // the last byte is one
    };

    /**
     * @brief parametersContext: get the parameters context
     * @return parameters context
     */
    ParameterContext parametersContext();

    // Set value
    void setBigEndian(bool bigEndian);
    void setStartByte(int startByte);
    void setEndByte(int endByte);
private:
    ParameterContext mParametersContext;
    QMutex mParametersContextMutex;
    bool mIsInitializing;
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
