/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ****************************************************************************************/
#ifndef SAKDEBUGGERINPUT_HH
#define SAKDEBUGGERINPUT_HH

#include <QPair>
#include <QLabel>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QListWidget>
#include <QSqlDatabase>
#include <QWaitCondition>

class SAKCommonCrcInterface;
class SAKDebuggerInputDataPreset;
class SAKDebuggerInputCrcSettings;

/// @brief input module controller
class SAKDebuggerInput : public QThread
{
    Q_OBJECT
public:
    SAKDebuggerInput(QComboBox *regularlySending,
                     QComboBox *inputFormat,
                     QPushButton *more,
                     QPushButton *send,
                     QLabel *crc,
                     QTextEdit *input,
                     QSettings *settings,
                     const QString &settingsGroup,
                     QSqlDatabase *sqlDatabase,
                     QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInput();

    struct SAKStructInputParametersContext {
        struct {
            bool appending; // true: append crc value to the tail of data frame
            bool bigEndian; // true: crc value is big endian
            int parametersModel; // The parameter model of crc value
            int startByte; // The start byte of crc section, the first byte is one
            int endByte; // The end byte of crc section, the last byte is one
        } crc;

        int textFormat; // Input text format, such as bin, otc, dec, hex and so on
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

    void inputBytes(QString rawBytes, SAKStructInputParametersContext parametersContext);
protected:
    void run() override;
private:
    QMenu *mWriteDataItemMenu;
    QVector<QPair<QString, SAKStructInputParametersContext>> mBytesInfoVector;
    QMutex mBytesInfoVectorMutex;
private:
    QComboBox *mCyclingTimeComboBox;
    QComboBox *mInputModelComboBox;
    QPushButton *mMoreInputSettingsPushButton;
    QPushButton *mSendPushButton;
    QTextEdit *mInputTextEdit;
    QLabel *mCrcLabel;

    QTimer mCyclingWritingTimer;
    SAKStructInputParametersContext mInputParameters;
    QMutex mInputParametersMutex;
    SAKCommonCrcInterface *mCrcInterface;
    SAKDebuggerInputDataPreset *mInputDataItemManager;
    SAKDebuggerInputCrcSettings *mCrcSettingsDialog;

    // Variables about settings
    QSettings *mSettings;
    QString mSettingStringInputTextFromat;
private:
    void changeInputModel(const QString &text);
    void saveInputDataToFile();
    void readinFile();
    void inputTextEditTextChanged();
    void sendRawData();
    void changeCrcModel();
    void setPresetData();
    void initParameters();
    void cyclingWritingTimerTimeout();
    void updateCrc();
    void readinSettings();
signals:
    void invokeWriteBytes(QByteArray bytes);
};
Q_DECLARE_METATYPE(SAKDebuggerInput::SAKStructInputParametersContext);

#endif
