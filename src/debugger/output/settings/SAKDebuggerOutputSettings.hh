/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGGEROUTPURTSETTINGS_HH
#define SAKDEBUGGEROUTPURTSETTINGS_HH

#include <QMutex>
#include <QObject>

class SAKDebugger;
class SAKDebuggerOutputStatistics;
class SAKDebuggerOutputSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList textFormats READ textFormats CONSTANT)
    Q_PROPERTY(SAKDebuggerOutputStatistics* statistics READ statistics CONSTANT)
    Q_PROPERTY(QString currentTextFormat READ currentTextFormat WRITE setCurrentTextFormat NOTIFY currentTextFormatChanged)

    /// @brief 以下是输出参数相关属性
    Q_PROPERTY(bool outputDate READ outputDate WRITE setOutputDate NOTIFY outputDateChanged)
    Q_PROPERTY(bool outputTime READ outputTime WRITE setOutputTime NOTIFY outputTimeChanged)
    Q_PROPERTY(bool outputMs READ outputMs WRITE setOutputMs NOTIFY outputMsChanged)
    Q_PROPERTY(bool outputRx READ outputRx WRITE setOutputRx NOTIFY outputRxChanged)
    Q_PROPERTY(bool outputTx READ outputTx WRITE setOutputTx NOTIFY outputTxChanged)
    Q_PROPERTY(bool outputWrap READ outputWrap WRITE setOutputWrap NOTIFY outputWrapChanged)
public:
    /// @brief 输出格式格式
    enum OutputTextFormat {
        Bin,
        Oct,
        Dec,
        Hex,
        Ascii,
        Utf8,
        Utf16,
        Ucs4,
        StdW,
        System
    };
    Q_ENUM(OutputTextFormat)

    /// @brief 输出参数上下文
    struct ParametersContext {
        bool outputDate;    // 输出数据时显示日期
        bool outputTime;    // 输出数据时显示时间
        bool outputMs;      // 输出数据时时间精确到毫秒
        bool outputRx;      // 允许输出已接收的数据
        bool outputTx;      // 允许输出已发送的数据
        bool outputWrap;    // 显示自动换行

        int outputFormat;   // 文本格式
    };

    SAKDebuggerOutputSettings(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerOutputSettings();

    /**
     * @brief parameters 获取输出参数
     * @return 输出参数
     */
    struct ParametersContext parameters();

    /**
     * @brief statistics 获取统计类实例指针
     * @return 统计类实例指针
     */
    SAKDebuggerOutputStatistics *statisticsInstance();

    /**
     * @brief clearOutput 清空输出
     */
    Q_INVOKABLE void clearOutput();

    /**
     * @brief saveOutputData 保存输出
     */
    Q_INVOKABLE void saveOutputData();
private:
    SAKDebugger *debugger;
    QMutex parameterCtxMutex;
    struct ParametersContext parameterCtx;
signals:
    void clearOutputReauest();
    void saveOutputDataRequest();
private:
    QStringList textFormats();

    SAKDebuggerOutputStatistics *_statistics;
    SAKDebuggerOutputStatistics *statistics();

    QString _currentTextFormat;
    QString currentTextFormat();
    void setCurrentTextFormat(QString format);

    bool outputDate();
    void setOutputDate(bool enable);

    bool outputTime();
    void setOutputTime(bool enable);

    bool outputMs();
    void setOutputMs(bool enable);

    bool outputRx();
    void setOutputRx(bool enable);

    bool outputTx();
    void setOutputTx(bool enable);

    bool outputWrap();
    void setOutputWrap(bool enable);
signals:
    void currentTextFormatChanged();
    void outputDateChanged();
    void outputTimeChanged();
    void outputMsChanged();
    void outputRxChanged();
    void outputTxChanged();
    void outputWrapChanged();
};

#endif
