/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKPROTOCOLANALYZER_HH
#define SAKPROTOCOLANALYZER_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/// @brief 协议分析器，按照指定的数据格式分析数据
class SAKProtocolAnalyzer:public QThread
{
    Q_OBJECT
public:
    /// @brief 参数上下文
    struct ParametersContext {
        bool enable; // 协议分析提取功能是否生效（使能），true表示使能
        bool fixed; // 协议帧长度是否固定，true表示固定
        qint32 length; // 协议的固定长度，fixed为true时该参数可用
        QByteArray startArray; // 协议起始标志，fixed为false时该参数可用
        QByteArray endArray;    // 协议结束标志，fixed为false时该参数可用
    };

    SAKProtocolAnalyzer(QObject *parent = Q_NULLPTR);
    ~SAKProtocolAnalyzer();

    /**
     * @brief clearRawData 清空待处理的数据
     */
    void clearData();

    /**
     * @brief wakeMe 唤醒
     */
    void wakeMe();

    /**
     * @brief inputBytes 添加数据
     * @param array 待分析数据
     */
    void inputBytes(QByteArray array);

    /// @brief 以下是参数设置接口
    void setEnable(bool enable);
    void setFixed(bool fixed);
    void setLength(int length);
    void setStartArray(QByteArray array);
    void setEndArray(QByteArray array);
protected:
    void run() final;
private:
    QMutex mThreadMutex;
    QWaitCondition mThreadCondition;
    QByteArray mWaitingAnalyzingBytes;
    QMutex mWaitingAnalyzingBytesMutex;
    ParametersContext mParameters;
    QMutex mParametersMutex;
signals:
    /// @brief 分析到一帧数据后，数据帧通过该接口对外发送
    void bytesAnalized(QByteArray bytes);
};

#endif
