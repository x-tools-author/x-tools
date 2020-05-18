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
#ifndef SAKDEBUGGERTEXTINPUT_HH
#define SAKDEBUGGERTEXTINPUT_HH

#include <QMutex>
#include <QObject>

class SAKDebugger;
class SAKDebuggerInputSettings;
class SAKDebuggerInputTextFactory;
class SAKDebuggerTextInput : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool addCRCFlag READ addCRCFlag WRITE setAddCRCFlag NOTIFY addCRCFlagChanged)
    Q_PROPERTY(bool bigEndianCRCFlag READ bigEndianCRCFlag WRITE setBigEndianCRCFlag NOTIFY bigEndianCRCFlagChanged)
public:
    SAKDebuggerTextInput(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerTextInput();

    /**
     * @brief wirteBytes 发送数据
     * @param bytes 待发送数据
     */
    Q_INVOKABLE void writeBytes(QByteArray bytes);

    /**
     * @brief writeRawData 写（发送）数据
     * @param data 待发送数据，格式为输入框文本格式
     */
    Q_INVOKABLE void writeRawData(QString data);

    /**
     * @brief paraAddCRCFlag 获取是否添加crc标志
     * @return 添加crc标志
     */
    bool paraAddCRCFlag();

    /**
     * @brief paraBigEndianCRCFlag 获取是否大端形式追加crc标志
     * @return crc字节序标志
     */
    bool paraBigEndianCRCFlag();

    /**
     * @brief setInputSettings 赋值
     * @param settings 输入设备类实例指针
     */
    void setInputSettings(SAKDebuggerInputSettings *settings);
private:
    SAKDebugger *debugger;
    QMutex addCRCFlagMutex;
    QMutex bigEndianCRCFlagMutex;
    SAKDebuggerInputTextFactory *inputTextFactory;
    SAKDebuggerInputSettings *inputSettings;
signals:
    void writeBytesRequest(QByteArray bytes);
private:
    bool _addCRCFlag;
    bool addCRCFlag();
    void setAddCRCFlag(bool flag);

    bool _bigEndianCRCFlag;
    bool bigEndianCRCFlag();
    void setBigEndianCRCFlag(bool flag);
signals:
    void addCRCFlagChanged();
    void bigEndianCRCFlagChanged();
};

#endif
