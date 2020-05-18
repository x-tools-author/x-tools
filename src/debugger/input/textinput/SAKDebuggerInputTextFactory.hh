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
#ifndef SAKDEBUGGERINPUTTEXTFACTORY_HH
#define SAKDEBUGGERINPUTTEXTFACTORY_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class SAKDebuggerTextInput;
class SAKDebuggerInputSettings;
class SAKDebuggerInputTextFactory : public QThread
{
    Q_OBJECT
public:
    SAKDebuggerInputTextFactory(QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInputTextFactory();

    /**
     * @brief addRawData 将待发送原始数据添加至发送数据处理列表中
     * @param rawData 输入文本
     */
    void addRawData(QString rawData);

    /**
     * @brief wakeMe 唤醒线程
     */
    void wakeMe();

    /**
     * @brief setInputSettings 赋值
     * @param settings 输入设置类指针
     */
    void setInputSettings(SAKDebuggerInputSettings *settings);

    /**
     * @brief setTextInput 赋值
     * @param input 文本输入管理类实例
     */
    void setTextInput(SAKDebuggerTextInput *input);
protected:
    void run() final;
private:
    QStringList rawDataList;
    QMutex rawDataListMutex;
    QMutex threadMutex;
    QWaitCondition threadWaitCondition;
    SAKDebuggerInputSettings *inputSettings;
    SAKDebuggerTextInput *textInput;
private:
    /// @brief 提取待处理数据链表中的第一个元素，如果链表为空则返回空（empty）字符串
    QString takeRawData();
    /// @brief 将数据按照参数处理
    QByteArray cookData(QString rawData, int format);
    /// @brief 计算输入数据crc
    QByteArray crcCalculate(QByteArray input, bool bigEndian, int model);
signals:
    /// @brief 根据参数完成处理后的数据由此信号发出
    void dataCooked(QByteArray bytes);
};

#endif
