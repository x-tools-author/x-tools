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
class SAKDebugPageOutputManager:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageOutputManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageOutputManager();

    /**
     * @brief 输出参数
     */
    struct OutputParameters {
        bool showDate;      // 是否显示日期
        bool showTime;      // 是否显示时间
        bool showMS;        // 是否显示毫秒
        bool isReceivedData;// 是否为接收到的数据
        int  textModel;     // 输出数据格式SAKGlobal::SAKTextFormat
    };
private:
    SAKDebugPage *debugPage;
    SAKOutputDataFactory *dataFactory;
    SAKSaveOutputDataSettings *outputSettings;

    QTimer updateRxFlagTimer;
    void updateRxFlag();
    qint8 rxFlagCount;

    QTimer updateTxFlagTimer;
    void updateTxFlag();
    qint8 txFlagCount;

    void setLineWrapMode();
    void saveOutputTextToFile();
    void saveOutputDataSettings();
    void saveOutputDataToFile();

    /*
     * ui指针
     */
    QTextBrowser *messageTextBrowser;
    QLabel       *rxLabel;
    QLabel       *txLabel;
    QComboBox    *outputTextFormatComboBox;
    QCheckBox    *showDateCheckBox;
    QCheckBox    *autoWrapCheckBox;
    QCheckBox    *showTimeCheckBox;
    QCheckBox    *showMsCheckBox;
    QCheckBox    *showRxDataCheckBox;
    QCheckBox    *showTxDataCheckBox;
    QCheckBox    *saveOutputFileToFilecheckBox;
    QPushButton  *outputFilePathPushButton;
    QPushButton  *clearOutputPushButton;
    QPushButton  *saveOutputPushButton;
    QTextBrowser *outputTextBroswer;

private:
    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void outputData(QString data);
    OutputParameters outputDataParameters(bool isReceivedData);
signals:
    void cookData(QByteArray rawData, OutputParameters parameters);
};
Q_DECLARE_METATYPE(SAKDebugPageOutputManager::OutputParameters);

#endif
