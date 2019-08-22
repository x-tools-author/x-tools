/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAKTABPAGE_HH
#define SAKTABPAGE_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>

#include "SAKCRCInterface.hh"
#include "ReadWriteSettingsWidget.hh"
#include "AutoResponseSettingWidget.hh"

class SAKChartManager;
class SAKDataFactory;
class SAKCRCInterface;
class SAKOtherSettings;
class TransmissionSettings;
class SAKStatisticsManager;
class HighlightSettingsWidget;

namespace Ui {
class SAKDebugPage;
}

/*
 * Hei, Developer! Takeacre of this warning!
 * 由于使用了信号与槽的自动关联，指向ui控件的指针变量的名称必须与ui控件的objectName保持一致。
 * 使用designer改变ui控件变量名称时，objectName属性会自动更新，不要在designer中手动更改ui控件的objectName。
 * objectName与变量名称不一致会导致信号与槽的关联失效。
 */

class SAKDebugPage : public QWidget
{
    Q_OBJECT
public:
    SAKDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKDebugPage();

    /**
     * @brief   -- 数据显示模式
     */
    enum TextDisplayModel {
        Bin,
        Oct,
        Dec,
        Hex,
        Ascii,
        Local8bit
    };
    Q_ENUM(TextDisplayModel)

    /**
     * @brief The OutputParameters struct   -- 输出参数
     */
    struct OutputParameters {
        bool showDate;
        bool showTime;
        bool showMS;
        bool isReceivedData;
        TextDisplayModel textModel;
    };

    /**
     * @brief The InputParameters struct    --  输入上下文
     */
    struct InputParameters {
        bool    sendCircularly;             // 循环发送标志
        bool    addCRC;                     // 追加crc校验数据标志
        bool    bigEnfian;                  // 大端序添加crc值
        quint32 cycleTime;                  // 循环发送时间（周期）
        TextDisplayModel inputModel;        // 数据输入模式
        SAKCRCInterface::CRCModel crcModel; // crc参数模型
    };

    /**
     * @brief readDelayTimes    -- 收到读就绪信号后，延时一段时间
     * @return                  -- 延时时间
     */
    unsigned long readDelayTimes();
    /**
     * @brief readDelayTimes    -- 执行发送函数前，延时一段时间发送
     * @return                  -- 延时时间
     */
    unsigned long writeDelayTimes();
    /**
     * @brief write             -- 写数据
     * @param data              -- 代写的数据
     */
    void write(QByteArray data);
    /**
     * @brief outputMessage -- 输出信息到ui显示
     * @param msg           -- 需要输出的消息
     * @param isInfo        -- 是否为普通消息
     */
    void outputMessage(QString msg, bool isInfo = true);

    friend class SAKOtherSettings;
    friend class SAKStatisticsManager;
protected:
    /**
     * @brief openOrColoseDevice    -- 打开或者关闭设备
     */
    virtual void openOrColoseDevice(){}
    /**
     * @brief refreshDevice         -- 刷新设备
     */
    virtual void refreshDevice(){}
    /**
     * @brief controllerWidget  -- 安装控制面板
     */
    virtual QWidget *controllerWidget(){return nullptr;}
    /**
     * @brief setUpController -- 安装控制器（控制面板）
     */
    void setUpController();   
    /**
     * @brief bytesRead -- 处理接受到的数据
     * @param data      -- 接受到的数据
     */
    void bytesRead(QByteArray data);
    /**
     * @brief bytesWritten  -- 处理已发送的数据
     * @param data          -- 已发送的数据
     */
    void bytesWritten(QByteArray data);   
    /**
     * @brief changedDeviceStatus   -- 设备打开或者关闭时执行该函数
     * @param opened                -- true: 设备一打开 false：设备已关闭
     */
    void changedDeviceStatus(bool opened);
protected:
    /// 循环发送定时器
    QTimer cycleTimer;
    SAKDataFactory *dataFactory;
private:
    /// 初始化ui
    void initUI();
    /// 注册用户自定义数据类型，自定义数据类型作为信号参数时，必须先注册
    void registerMetaType();
    /// 获取输出参数
    OutputParameters outputParameters();
    /// 写数据
    void writeBytes(QByteArray data){emit need2writeBytes(data);}
    /// 设置label文本
    void setLabelText(QLabel *label, quint64 text);
    /// 设置输入框的文本输入格式
    void formattingInputText(TextDisplayModel model);
    /// 计算crc
    uint32_t crcCalculate(QByteArray data, SAKCRCInterface::CRCModel model);
    /// 处理文本数据
    QByteArray cookedData(QString rawData);
    /// 循环计时器计数溢出时执行该函数
    void cycleTimerTimeout();
signals:
    /// 该函数并不会真的发送数据，而是发送一个信号，该信号携带需要发送的数据,数据需要经过处理后才能发送
    void sendRawData(QString rawData, InputParameters parameters);
    /// 将读取到的数据或者已发送的数据发射出去处理
    void dataReadOrwritten(QByteArray data, OutputParameters parameters);
    /// 子类关联该信号来发送数据即可
    void need2writeBytes(QByteArray data);
    /// 子类设备状态（打开/关闭）发生改变时发送该信号
    void deviceStatusChanged(bool opened);
private:
    /// 日志类型
    const char *logCategory = "SAKTabPage";
    /// crc计算接口
    SAKCRCInterface *crcInterface;
    friend class SAKDataFactory;
    ///----------------------------------------------------------------
    QString readSetting(QString &option);
    void writeSetting(QString &option, QString &value);    

    /**
     * @brief outputData        -- 更新输出数据
     * @param data              -- 需要输出显示的数据
     */
    void outputData(QString data);    
private slots:
    void updateRxImage();
    void updateTxImage();

    void cancleCycle();

    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode(QString mode);
    void readOutputMode();

    void setInputMode(QString mode);
    void readInputMode();

    void checkedBoxCycleClicked(bool checked);

    void resetSendDataCount();
    void resetReceiveDataCount();



    // ------------------------------------------------------------------------
    // 设备设置
protected:    
    QPushButton *refreshPushButton              = nullptr;  // 刷新按钮
    QPushButton *switchPushButton               = nullptr;  // 打开关闭设备按钮
    QGroupBox   *deviceSettingGroupBox          = nullptr;  // 控制面板
private slots:
    void on_refreshPushButton_clicked(){refreshDevice();}
    void on_switchPushButton_clicked(){openOrColoseDevice();}

    // 消息输出组
protected:
    QTextBrowser *messageTextBrowser            = nullptr;  // 消息输出框

     // 输入设置组
protected:
    InputParameters inputParameters;                        // 输入参数

    QComboBox   *inputModelComboBox             = nullptr;  // 输入模式预选框
    QCheckBox   *cycleEnableCheckBox            = nullptr;  // 循环使能复选框
    QLineEdit   *cycleTimeLineEdit              = nullptr;  // 循环周期输入框
    QPushButton *readinFilePushButton           = nullptr;  // 读入文件按钮
    QCheckBox   *addCRCCheckBox                 = nullptr;  // 发送数据添加crc校验
    QCheckBox   *bigeEndianCheckBox             = nullptr;  // crc值以大端形式添加
    QPushButton *clearInputPushButton           = nullptr;  // 清空输入框按钮
    QPushButton *sendPushButton                 = nullptr;  // 发送数据按钮
    QTextEdit   *inputTextEdit                  = nullptr;  // 数据输入框
    QComboBox   *crcParameterModelsComboBox     = nullptr;  // crc计算参数模型
    QLabel      *crcLabel                       = nullptr;  // crc显示标签
private slots:
    void on_inputModelComboBox_currentTextChanged(const QString &text);
    void on_cycleEnableCheckBox_clicked();
    void on_cycleTimeLineEdit_textChanged(const QString &text);
    void on_readinFilePushButton_clicked();
    void on_addCRCCheckBox_clicked();
    void on_bigeEndianCheckBox_clicked();
    void on_clearInputPushButton_clicked();
    void on_sendPushButton_clicked();
    void on_inputTextEdit_textChanged();
    void on_crcParameterModelsComboBox_currentTextChanged(const QString &text);

    //数据输出组管理
protected:
    TextDisplayModel outputTextModel = SAKDebugPage::Hex;

    QLabel      *rxLabel                        = nullptr;  // 接受指示灯
    QLabel      *txLabel                        = nullptr;  // 发送指示灯
    QComboBox   *outputModelComboBox            = nullptr;  // 输出模式复选框
    QCheckBox   *showDateCheckBox               = nullptr;  // 显示日期使能复选框
    QCheckBox   *autoWrapCheckBox               = nullptr;  // 自动换行使能复选框
    QCheckBox   *showTimeCheckBox               = nullptr;  // 显示时间使能复选框
    QCheckBox   *showMsCheckBox                 = nullptr;  // 显示毫秒使能复选框
    QCheckBox   *showRxDataCheckBox             = nullptr;  // 显示接收数据使能复选框
    QCheckBox   *showTxDataCheckBox             = nullptr;  // 显示发送数据使能复选框
    QPushButton *clearOutputPushButton          = nullptr;  // 清空输出按钮
    QPushButton *saveOutputPushButton           = nullptr;  // 保存输出按钮
    QTextBrowser *outputTextBroswer             = nullptr;  // 用于输出显示收发的数据
private slots:
    void on_outputModelComboBox_currentTextChanged(const QString &text);
    void on_autoWrapCheckBox_clicked();
    void on_clearOutputPushButton_clicked();
    void on_saveOutputPushButton_clicked();

    // 数据管理组
protected:
    bool        receivedFlag                    = false;    // 接受状态指示灯
    bool        sendFlag                        = false;    // 接受指示灯状态

    QLabel      *rxSpeedLabel;
    QLabel      *txSpeedLabel;
    QLabel      *rxFramesLabel;
    QLabel      *txFramesLabel;
    QLabel      *rxBytesLabel;
    QLabel      *txBytesLabel;
    QPushButton *resetTxCountPushButton;
    QPushButton *resetRxCountPushButton;
private:
    /// 其他设置
    SAKOtherSettings *otherSettings;
    /// 数据统计
    SAKStatisticsManager *statisticsManager;
    /// 图标显示
    SAKChartManager *chart;

private:
    /**
     * @brief ui    -- 界面文件
     */
    Ui::SAKDebugPage *ui = nullptr;

    /**
     * @brief initUiPointer -- 初始化指向ui控件的数据成员（指针）
     */
    void initUiPointer();       
};

Q_DECLARE_METATYPE(SAKDebugPage::TextDisplayModel);
Q_DECLARE_METATYPE(SAKDebugPage::OutputParameters);

#endif  // SAKTabPage_H
