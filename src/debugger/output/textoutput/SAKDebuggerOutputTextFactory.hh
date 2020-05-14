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
#ifndef SAKDEBUGGERTOUTPUTTEXTFACTORY_HH
#define SAKDEBUGGERTOUTPUTTEXTFACTORY_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class SAKDebuggerOutputSettings;
class SAKDebuggerOutputTextFactory : public QThread
{
    Q_OBJECT
public:
    SAKDebuggerOutputTextFactory(QObject *parent = Q_NULLPTR);
    ~SAKDebuggerOutputTextFactory();

    /// @brief 文本处理信息
    struct TextContext {
        bool isRxData;          // true表示数据为已读取的数据，false表示数据为已发送的数据
        QByteArray text;        // 待处理数据
    };

    /**
     * @brief addRawData 添加待处理数据
     * @param text 待处理数据
     * @param isRxData true表示数据为接收数据，false表示数据为发送数据
     */
    void addRawData(QByteArray text, bool isRxData);

    /**
     * @brief wakeMe 唤醒线程
     */
    void wakeMe();

    /**
     * @brief setOutputSettings 赋值
     * @param settings 输出设置类实例
     */
    void setOutputSettings(SAKDebuggerOutputSettings *settings);
protected:
    void run() final;
private:
    SAKDebuggerOutputSettings *outputSettings;
    QMutex textInfosListMutex;
    QList<TextContext> textInfosList;
    QWaitCondition threadWaitCondition;
    QMutex threadMutex;
private:
    TextContext takeRawData();
    QString prefixString(bool date, bool time, bool ms, bool isRxData);
    QString dataToString(QByteArray rawData, int textModel);
signals:
    void dataCooked(QString text);
};

#endif
