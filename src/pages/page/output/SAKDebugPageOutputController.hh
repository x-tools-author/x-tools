/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEOUTPUTMANAGER_HH
#define SAKDEBUGPAGEOUTPUTMANAGER_HH
#include <QTimer>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTextBrowser>

class SAKDebugPage;
class SAKOutputDataFactory;
class SAKSaveOutputDataSettings;
/// @brief output data manager
class SAKDebugPageOutputController:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageOutputController(SAKDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOutputController();

    struct OutputParameters {
        bool showDate; // true: show date shen output data
        bool showTime; // true: show time when output data
        bool showMS; // true: show ms shen output data
        bool isReadData; // true: data is read data, false: data is written data
        int  format; // output data format, such as bin, otc and so on
    };
private:
    SAKDebugPage *mDebugPage;
    SAKOutputDataFactory *mDataFactory;
    SAKSaveOutputDataSettings *mOutputSettings;

    // animation
    QTimer mUpdateRxAnimationTimer;
    qint8 mRxAnimationgCount;
    QTimer mUpdateTxAnimationTimer;
    qint8 mTxAnimationCount;

    // ui components
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
    QCheckBox *mSaveOutputFileToFilecheckBox;
    QPushButton *mOutputFilePathPushButton;
    QPushButton *mClearOutputPushButton;
    QPushButton *mSaveOutputPushButton;
    QTextBrowser *mOutputTextBroswer;
private:
    void updateRxAnimation();
    void updateTxAnimation();
    void setLineWrapMode();
    void saveOutputTextToFile();
    void saveOutputDataSettings();
    void saveOutputDataToFile();
    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void outputData(QString data);
    OutputParameters outputDataParameters(bool isReadData);
signals:
    void cookData(QByteArray rawData, OutputParameters parameters);
};
Q_DECLARE_METATYPE(SAKDebugPageOutputController::OutputParameters);

#endif
