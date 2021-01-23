/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEOUTPUTMANAGER_HH
#define SAKDEBUGPAGEOUTPUTMANAGER_HH

#include <QMutex>
#include <QTimer>
#include <QLabel>
#include <QTimer>
#include <QThread>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QTextBrowser>
#include <QWaitCondition>

class SAKDebugPage;
class SAKOutputLogDialog;
class SAKOutputSave2FileDialog;
class SAKOtherHighlighterManager;
/// @brief output data controller
class SAKDebugPageOutputController:public QThread
{
    Q_OBJECT
public:
    SAKDebugPageOutputController(SAKDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOutputController();
    void outputLog(QString log, bool isInfo = true);

    struct OutputParameters {
        bool showDate; // true: show date shen output data
        bool showTime; // true: show time when output data
        bool showMS; // true: show ms shen output data
        bool isReadData; // true: data is read data, false: data is written data
        bool isRawData;
        int  format; // output data format, such as bin, otc and so on
    };
protected:
    void run() final;
private:
    SAKDebugPage *mDebugPage;
    QSettings *mSettings;
    SAKOutputSave2FileDialog *mSave2FileDialog;
    SAKOutputLogDialog *mSAKOutputLogDialog;
    SAKOtherHighlighterManager *mSAKOtherHighlighterManager;
    bool mHasBeenClear;

    // Animation
    QTimer mUpdateRxAnimationTimer;
    qint8 mRxAnimationgCount;
    QTimer mUpdateTxAnimationTimer;
    qint8 mTxAnimationCount;

    // Ui components
    QTextBrowser *mMessageTextBrowser;
    QLabel *mRxLabel;
    QLabel *mTxLabel;
    QComboBox *mOutputTextFormatComboBox;
    QCheckBox *mShowDateCheckBox;
    QCheckBox *mAutoWrapCheckBox;
    QCheckBox *mShowTimeCheckBox;
    QCheckBox *mShowMsCheckBox;
    QCheckBox *mShowRxDataCheckBox;
    QCheckBox *mShowTxDataCheckBox;
    QCheckBox *mSaveOutputToFileCheckBox;
    QCheckBox *mRawDataCheckBox;
    QPushButton *mMoreOutputSettingsPushButton;
    QPushButton *mClearOutputPushButton;
    QTextBrowser *mOutputTextBroswer;

    // Variables about settings
    QString mSettingStringOutputTextFormat;
    QString mSettingStringShowDate;
    QString mSettingStringAutoWrap;
    QString mSettingStringShowTime;
    QString mSettingStringShowMs;
    QString mSettingStringShowRx;
    QString mSettingStringShowTx;
    QString mSettingStringRawData;

    // Thread controller
    QMutex mThreadMutex;
    QWaitCondition mThreadWaitCondition;

    // Temp data
    struct RawDataStruct {
        QByteArray rawData;
        OutputParameters parameters;
    };
    QList<RawDataStruct> mRawDataList;
    QMutex mRawDataListMutex;
private:
    // C<<<<<
    void updateRxAnimation();
    // C>>>>>
    void updateTxAnimation();
    void setLineWrapMode();
    void saveOutputTextToFile();
    void saveOutputDataSettings();
    void saveOutputDataToFile();
    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void outputData(QString data, bool rawData);
    OutputParameters outputDataParameters(bool isReadData);
    RawDataStruct takeRawData();
    void readinSettings();
    // The function is called by child thread only!
    void innerCookData(QByteArray rawData, OutputParameters parameters);

    // Update settings
    void onOutputTextFormatComboBoxCurrentTextChanged(const QString &text);
    void onShowDateCheckBoxClicked();
    void onAutoWrapCheckBoxClicked();
    void onShowTimeCheckBoxClicked();
    void onShowMsCheckBoxClicked();
    void onShowRxDataCheckBoxClicked();
    void onShowTxDataCheckBoxClicked();
    void onRawDataCheckBoxClicked();
signals:
    void dataCooked(QString data, bool isRawData);
};

#endif
