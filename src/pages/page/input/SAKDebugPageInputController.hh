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
#include <QMutex>
#include <QObject>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QListWidget>
#include <QWaitCondition>

class SAKDebugPage;
class SAKCRCInterface;
class SAKInputDataFactory;
class SAKInputDataPresetItem;
class SAKInputCrcSettingsDialog;
class SAKInputDataPresetItemManager;
/// @brief input module controller
class SAKDebugPageInputController:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageInputController(SAKDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageInputController();

    struct InputParametersContext {
        bool addCRC; // true: append crc value to the tail of data frame
        bool bigEndian; // true: crc value is big endian
        int cycleTime; // auto send data interval
        int inputModel; // input text format, such as bin, otc, dec, hex and so on
        int crcModel; // the parameter model of crc value
        int startByte; // start byte of crc section
        int endByte; // end byte of crc section
    };

    /**
     * @brief showCrcSettingsDialog: popup crc setting dialog
     */
    void showCrcSettingsDialog();

    /**
     * @brief formattingInputText: formattting input text According to model
     * @param textEdit: Text source
     * @param model: text model
     */
    static void formattingInputText(QTextEdit *textEdit, int model);
private:
    SAKDebugPage *mDebugPage;
    QComboBox *mInputModelComboBox;
    QCheckBox *mCycleEnableCheckBox;
    QLineEdit *mCycleTimeLineEdit;
    QPushButton *mSaveInputDataPushButton;
    QPushButton *mReadinFilePushButton;
    QCheckBox *mAddCRCCheckBox;
    QPushButton *mClearInputPushButton;
    QPushButton *mSendPushButton;
    QTextEdit *mInputTextEdit;
    QComboBox *mCrcParameterModelsComboBox;
    QLabel *mCrcLabel;
    QPushButton *mPresetPushButton;
    QPushButton *mSendPresetPushButton;
    QPushButton *mCrcSettingsPushButton;

    QTimer mTimingTimer;
    SAKInputDataFactory *mInputDataFactory;
    InputParametersContext mInputParameters;
    SAKCRCInterface *mCrcInterface;
    SAKInputDataPresetItemManager *mInputDataItemManager;
    SAKInputCrcSettingsDialog *mCrcSettingsDialog;

    // variables about settings
    QSettings *mSettings;
    QString mSettingStringInputTextFromat;
    QString mSettingStringCycleTime;
    QString mSettingStringAddCRC;
    QString mSettingStringBigEndian;
    QString mSettingStringCrcParameterModel;
    QString mSettingStringCrcStartByte;
    QString mSettingStringCrcEndByte;
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
    void initParameters();
    void setCycleEnable();       
    void cycleTimerTimeout();
    void updateCRC();
    void appendAction(SAKInputDataPresetItem *item);
    void removeAction(SAKInputDataPresetItem *item);
    void changeDescription(SAKInputDataPresetItem *item);
    void actionTriggered();
    void readinSettings();
signals:
    void rawDataChanged(QString rawData, InputParametersContext parameters);
};
Q_DECLARE_METATYPE(SAKDebugPageInputController::InputParametersContext);

#endif
