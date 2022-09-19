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
#include <QSettings>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include <QSerialPort>
#include <QTextBrowser>

#define SAKATASSISTANT "SAKAtAssistant"

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
    enum class SAKEnumTextFormat {
        Bin, Otc, Dec, Hex, ASCII, UTF8, SYSTEM
    };

    struct SAKStructUiContext {
        struct {
            QComboBox *portName;
            QComboBox *baudRate;
            QComboBox *parity;
            QComboBox *dataBits;
            QComboBox *stopBits;
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
    };

    struct SAKStructSettingsContext {
        struct {
            const QString portName = QString("SAKAtAssistant/device/portNmae");
            const QString baudRate = QString("SAKAtAssistant/device/baudRate");
            const QString parity = QString("SAKAtAssistant/device/parity");
            const QString dataBits = QString("SAKAtAssistant/device/dataBits");
            const QString stopBits = QString("SAKAtAssistant/device/stopBits");
            const QString flowControl =
                    QString("SAKAtAssistant/device/flowControl");
        } device;
        struct {
            const QString cycleSending =
                    QString("SAKAtAssistant/input/cycleSending");
            const QString textFormat =
                    QString("SAKAtAssistant/input/textFormat");
            const QString suffix =
                    QString("SAKAtAssistant/input/suffix");
            const QString customSuffix =
                    QString("SAKAtAssistant/input/customSuffix");
            const QString customSuffixLineEdit =
                    QString("SAKAtAssistant/input/customSuffixLineEdit");
            const QString atInput =
                    QString("SAKAtAssistant/input/atInput");
        } input;
        struct {
            const QString textFormat =
                    QString("SAKAtAssistant/output/textFormat");
            const QString date =
                    QString("SAKAtAssistant/output/date");
            const QString time =
                    QString("SAKAtAssistant/output/time");
            const QString ms =
                    QString("SAKAtAssistant/output/ms");
            const QString rx =
                    QString("SAKAtAssistant/output/rx");
            const QString tx =
                    QString("SAKAtAssistant/output/tx");
        } output;
        struct {
            const QString tableView =
                    QString("SAKAtAssistant/command/tableView");
            const QString interval =
                    QString("SAKAtAssistant/command/interval");
            const QString timeout =
                    QString("SAKAtAssistant/command/timeout");
            const QString number =
                    QString("SAKAtAssistant/command/number");
        } command;
    };
private:
    SAKStructUiContext uiCtx_;
    SAKStructSettingsContext settingsCtx_;
    Ui::SAKAtAssistant *ui_;
    QSettings settings_;
    QSerialPort *serialPort_;
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
    void onDeviceStopBitsChanged();
    void onDeviceFlowControlChanged();
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
    void setupStopBits(QComboBox *cb);
    void setupFlowControl(QComboBox *cb);
    void setupTextFormat(QComboBox *cb);
    void setupCycleSending(QComboBox *cb);
    void setupSuffix(QComboBox *cb);
    void setupAtInput(QComboBox *cb, const QStringList &commands);
    void setupCommandInterval(QComboBox *cb);
    void setupCommandTimeout(QComboBox *cb);
    void setupCommandCycleNumber(QComboBox *cb);
    void setupComboBoxText(QComboBox *cb, const QString &key);
    void setupComboBoxIndex(QComboBox *cb, const QString &key);
    void setupCheckBox(QCheckBox *cb, const QString &key);
    void setupLineEdit(QLineEdit *le, const QString &key);
    void setupUiState(bool opened);
    void setupMessage(QTextBrowser *tb, const QString &message);
    void setupDeviceNotOpenedMessageBox();
};

#endif // SAKATASSISTANT_HH
