/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
    SAKDebugPageOutputManager(SAKDebugPage *debugPage, QObject *parent = nullptr);
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
