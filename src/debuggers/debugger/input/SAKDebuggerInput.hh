/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
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
                     QComboBox *input,
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
        int suffixsType;
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

    /**
     * @brief inputBytes: Data input entry
     * @param rawBytes: Data String
     * @param parametersContext: Input parameters
     */
    void inputBytes(QString rawBytes, SAKStructInputParametersContext parametersContext);
protected:
    void run() override;
private:
    struct SAKStructSettingKeyContext {
        QString suffixsType;
        QString inputTextFormat;
        QString enableSendingRecord;
    } mSettingKeyCtx;
private:
    QMenu *mQuickSendingMenu;
    QVector<QPair<QString, SAKStructInputParametersContext>> mBytesInfoVector;
    QMutex mBytesInfoVectorMutex;
    bool mEnableSendingRecord;
private:
    QComboBox *mCyclingTimeComboBox;
    QComboBox *mInputModelComboBox;
    QPushButton *mMoreInputSettingsPushButton;
    QPushButton *mSendPushButton;
    QComboBox *mInputComboBox;
    QLabel *mCrcLabel;

    QTimer mCyclingWritingTimer;
    SAKStructInputParametersContext mInputParameters;
    QMutex mInputParametersMutex;
    SAKCommonCrcInterface *mCrcInterface;
    SAKDebuggerInputDataPreset *mInputDataItemManager;
    SAKDebuggerInputCrcSettings *mCrcSettings;

    // Variables about settings
    QSettings *mSettings;
    QString mSettingStringInputTextFromat;
private:
    void changeInputModel(const QString &text);
    void inputTextEditTextChanged();
    void sendRawData();
    void changeCrcModel();
    void initParameters();
    void cyclingWritingTimerTimeout();
    void updateCrc();
    void readinSettings();
    quint32 crcCalculate(QByteArray data, int model);
    QByteArray extractCrcData(QByteArray crcData,
                              SAKStructInputParametersContext parameters);
signals:
    void invokeWriteBytes(QByteArray bytes);
    void messageChanged(QString msg, bool isError);
};
Q_DECLARE_METATYPE(SAKDebuggerInput::SAKStructInputParametersContext);

#endif
