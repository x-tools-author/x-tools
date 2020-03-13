/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGPAGEINPUTMANAGER_HH
#define SAKDEBUGPAGEINPUTMANAGER_HH

#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>

class SAKDebugPage;
class SAKInputDataItem;
class SAKCRCInterface;
class SAKInputDataFactory;
class SAKInputDataItemManager;
class SAKDebugPageInputManager:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageInputManager(SAKDebugPage *debugPage, QObject *parent = nullptr);
    ~SAKDebugPageInputManager();

    /**
     * @brief The InputParameters struct    --  输入上下文
     */
    struct InputParameters {
        bool    addCRC;         // 追加crc校验数据标志
        bool    bigEndian;      // 大端序添加crc值
        int     cycleTime;      // 循环发送时间（周期）
        int     inputModel;     // 数据输入模式
        int     crcModel;       // crc参数模型
    };

    friend class SAKInputDataItem;
private:
    SAKDebugPage *debugPage;

    QComboBox   *inputModelComboBox;
    QCheckBox   *cycleEnableCheckBox;
    QLineEdit   *cycleTimeLineEdit;
    QPushButton *saveInputDataPushButton;
    QPushButton *readinFilePushButton;
    QCheckBox   *addCRCCheckBox;
    QCheckBox   *bigeEndianCheckBox;
    QPushButton *clearInputPushButton;
    QPushButton *sendPushButton;
    QTextEdit   *inputTextEdit;
    QComboBox   *crcParameterModelsComboBox;
    QLabel      *crcLabel;
    QPushButton *presetPushButton;
    QPushButton *sendPresetPushButton;

private:
    /// 输入文本格式法神改变时执行该函数
    void changeInputModel(const QString &text);
    /// 循环使能发生改变时执行该函数
    void changeCycleEnableFlag();
    /// 改变循环发送时间
    void changeCycleTime();
    /// 保存发送输入框数据至文本
    void saveInputDataToFile();
    /// 读入文本文件
    void readinFile();
    /// crc添加标志发生该表是执行该函数
    void changeAddCRCFlag();
    /// 字节序标志发生改变时，执行该函数
    void changeEndianFlag();
    /// 清空输入区域
    void clearInputArea();
    /// 输入框数据发生改变时，执行该函数
    void inputTextEditTextChanged();
    /// 发送数据
    void sendRawData();
    /// 发送数据
    void sendOtherRawData(QString data, int textFormat);
    /// crc参数模型发生改变时执行该函数
    void changeCRCModel();
    /// 设置预设数据
    void setPresetData();
    /// 发送预设数据
    void sendPresetData();

private:
    /// 初始化输入参数
    void initParameters();
    /// 启动/停止循环发送定时器
    void setCycleEnable();    
private:
    /// 循环发送定时器
    QTimer timingTimer;
    /// 循环发送定时器溢出后，执行该函数，执行该函数后，将会发送一个请求发送数据信号
    void cycleTimerTimeout();

private:
    SAKInputDataFactory *inputDataFactory;
    InputParameters inputParameters;
    SAKCRCInterface *crcInterface;
    SAKInputDataItemManager *inputDataItemManager;

    void updateCRC();
    void formattingInputText(QTextEdit *textEdit, int model);
signals:
    /// 该函数并不会真的发送数据，而是发送一个信号，该信号携带需要发送的数据,数据需要经过处理后才能发送
    void rawDataChanged(QString rawData, InputParameters parameters);
};
Q_DECLARE_METATYPE(SAKDebugPageInputManager::InputParameters);

#endif
