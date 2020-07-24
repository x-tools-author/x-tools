/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEINPUTCONTROLLER_HH
#define SAKDEBUGPAGEINPUTCONTROLLER_HH

#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>

class SAKDebugPage;
class SAKInputDataItem;
class SAKCRCInterface;
class SAKInputDataFactory;
class SAKInputDataItemManager;
class SAKInputCrcSettingsDialog;
class SAKDebugPageInputController:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageInputController(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageInputController();

    struct InputParameters {
        bool    addCRC; // true: append crc value to the tail of data frame
        bool    bigEndian; // true: crc value is big endian
        int     cycleTime; // auto send data interval
        int     inputModel; // input text format, such as bin, otc, dec, hex and so on
        int     crcModel; // the parameter model of crc value
        int     startByte; // start byte of crc section
        int     endByte; // end byte of crc section
    };

    /**
     * @brief showCrcSettingsDialog: popup crc setting dialog
     */
    void showCrcSettingsDialog();

    friend class SAKInputDataItem;
private:
    SAKDebugPage *debugPage;

    QComboBox   *inputModelComboBox;
    QCheckBox   *cycleEnableCheckBox;
    QLineEdit   *cycleTimeLineEdit;
    QPushButton *saveInputDataPushButton;
    QPushButton *readinFilePushButton;
    QCheckBox   *addCRCCheckBox;
    QPushButton *clearInputPushButton;
    QPushButton *sendPushButton;
    QTextEdit   *inputTextEdit;
    QComboBox   *crcParameterModelsComboBox;
    QLabel      *crcLabel;
    QPushButton *presetPushButton;
    QPushButton *sendPresetPushButton;
private:
    void changeInputModel(const QString &text);
    void changeCycleEnableFlag();
    void changeCycleTime();
    void saveInputDataToFile();
    void readinFile();
    void changeAddCRCFlag();
    void clearInputArea();
    void inputTextEditTextChanged();
    void sendRawData();
    void sendOtherRawData(QString data, int textFormat);
    void changeCRCModel();
    void setPresetData();
    void sendPresetData();
private:
    void initParameters();
    void setCycleEnable();    
private:
    QTimer timingTimer;
    void cycleTimerTimeout();
private:
    SAKInputDataFactory *inputDataFactory;
    InputParameters inputParameters;
    SAKCRCInterface *crcInterface;
    SAKInputDataItemManager *inputDataItemManager;
    SAKInputCrcSettingsDialog *crcSettingsDialog;

    void updateCRC();
    void formattingInputText(QTextEdit *textEdit, int model);
signals:
    void rawDataChanged(QString rawData, InputParameters parameters);
};
Q_DECLARE_METATYPE(SAKDebugPageInputController::InputParameters);

#endif
