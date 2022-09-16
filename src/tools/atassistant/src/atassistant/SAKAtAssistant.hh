/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKATASSISTANT_HH
#define SAKATASSISTANT_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QTextBrowser>

namespace Ui {
    class SAKAtAssistant;
}

class SAKAtAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKAtAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKAtAssistant();
private:
    struct {
        struct {
            QComboBox *portName;
            QComboBox *baudRate;
            QComboBox *parity;
            QComboBox *dataBits;
            QComboBox *flowControl;
            QPushButton *refresh;
            QPushButton *open;
        } device;
        struct {
            QComboBox *cycleSending;
            QComboBox *textFormat;
            QComboBox *suffix;
            QCheckBox *customSuffix;
            QLineEdit *customSuffixLineEdit;
            QComboBox *atInput;
            QPushButton *sending;
        } input;
        struct {
            QComboBox *textFormat;
            QCheckBox *date;
            QCheckBox *time;
            QCheckBox *ms;
            QCheckBox *rx;
            QCheckBox *tx;
            QPushButton *exportOutput;
            QPushButton *clear;
            QTextBrowser *textBrowser;
        } output;
        struct {
            QTableView *tableView;
            QComboBox *interval;
            QComboBox *timeout;
            QComboBox *number;
            QPushButton *startAutoTest;
            QPushButton *importButton;
            QPushButton *exportButton;
        } command;
    } uiCtx_;
    Ui::SAKAtAssistant *ui_;
private:
private:
    void initUiCtx();
    void initUiCtxDevice();
    void initUiCtxInput();
    void initUiCtxOutput();
    void initUiCtxCommand();

    void initSettings();
    void initSettingsDevice();
    void initSettingsInput();
    void initSettingsOutput();
    void initSettingsCommand();

    void initSignals();
    void initSignalsDevice();
    void initSignalsInput();
    void initSignalsOutput();
    void initSignalsCommand();

    void onDevicePortNameChanged();
    void onDeviceBaudRateChanged();
    void onDeviceParityChanged();
    void onDeviceDataBitsChanged();
    void onDeviceRefreshClicked();
    void onDeviceOpenClicked();

    void onInputCycleSendingChanged();
    void onInputTextFormatChanged();
    void onInputSuffixChanged();
    void onInputCustomSuffixChanged();
    void onInputCustomSuffixLineEditChanged();
    void onInputCustomAtInputChanged();
    void onInputSendingClicked();

    void onOutputTextFormatChanged();
    void onOutputDateChanged();
    void onOutputTimeChanged();
    void onOutputMsChanged();
    void onOutputRxChanged();
    void onOutputTxChanged();
    void onOutputExportOutputClikced();
    void onOutputClearClicked();

    void onCommandIntervalChanged();
    void onCommandTimeoutChanged();
    void onCommandNumberChanged();
    void onCommandStartAutoTestClicked();
    void onCommandImportButtonClicked();
    void onCommandExportButtonClicked();
private:
    void setupPortName(QComboBox *cb);
    void setupBaudRate(QComboBox *cb);
    void setupParity(QComboBox *cb);
    void setupDataBits(QComboBox *cb);
    void setupFlowControl(QComboBox *cb);
};

#endif // SAKATASSISTANT_HH
