/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKDEBUGPAGEOUTPUTMANAGER_HH
#define SAKDEBUGPAGEOUTPUTMANAGER_HH

#include "SAKDebugPage.hh"

#include <QTimer>
#include <QObject>

class SAKOutputDataFactory;
class SAKSaveOutputDataSettings;
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
