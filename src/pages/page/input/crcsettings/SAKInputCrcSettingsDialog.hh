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
#include <QComboBox>
#include <QSettings>

namespace Ui {
    class SAKInputCrcSettingsDialog;
}

/// @brief crc settings dialog
class SAKInputCrcSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SAKInputCrcSettingsDialog(QString settingsGroup, QSettings *settings, QWidget *parent = Q_NULLPTR);
    ~SAKInputCrcSettingsDialog();

    struct CrcParameterContext {
        bool appendCrc;
        bool bigEndianCrc;
        int crcPrameterMoldel;
        int startByte;  // The first byte is one
        int endByte;    // The last byte is one
    };

    /**
     * @brief parametersContext: get the parameters context
     * @return parameters context
     */
    CrcParameterContext parametersContext();
private:
    CrcParameterContext mParametersContext;
    QMutex mParametersContextMutex;
    QSettings *mSettings;

    // Settings key.
    QString mSettingStringAppendCrc;
    QString mSettingStringBigEndian;
    QString mSettingStringCrcParametersModel;
    QString mSettingStringStartByte;
    QString mSettingStringEndByte;
private:
    Ui::SAKInputCrcSettingsDialog *mUi;
    QComboBox *mCrcParametersModelComboBox;
    QCheckBox *mAppendCrcCheckBox;
    QCheckBox *mBigEndianCheckBox;
    QSpinBox *mStartSpinBox;
    QSpinBox *mEndSpinBox;
private slots:
    void on_bigEndianCheckBox_clicked();
    void on_startSpinBox_valueChanged(int value);
    void on_endSpinBox_valueChanged(int value);
    void on_crcParametersModelComboBox_currentIndexChanged(int index);
    void on_appendCrcCheckBox_clicked();
signals:
    void crcParametersChanged();
};

#endif
