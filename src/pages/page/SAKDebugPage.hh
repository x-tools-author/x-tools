/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTABPAGE_HH
#define SAKTABPAGE_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QMutex>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>

#include "SAKCRCInterface.hh"
#include "SAKReadWriteSettingsWidget.hh"
#include "SAKAutoResponseSettingsWidget.hh"

class SAKDevice;
class SAKDataFactory;
class SAKCRCInterface;
class SAKStatisticsManager;
class SAKOtherSettingsManager;
class SAKTransmissionSettings;
class SAKDebugPageInputManager;
class SAKDebugPageOutputManager;
class SAKHighlightSettingsWidget;
#ifdef SAK_IMPORT_CHARTS_MODULE
class SAKDataVisualizationManager;
#endif

namespace Ui {
    class SAKDebugPage;
}

class SAKDebugPage : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief SAKDebugPage 调试页面基类
     * @param type 调试页面类型（SAKGlobal::SAKEnumDebugPageType）
     * @param parent 资源管理类
     */
    SAKDebugPage(int type, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPage();        

    /**
     * @brief changeDeviceState 更改设备状态
     * @param isOpened true表示设备已打开，false表示设备已关闭
     */
    void changeDeviceState(bool isOpened);
public:
    /**
     * @brief write             -- 写数据
     * @param data              -- 待写的数据
     */
    void write(QByteArray data);

    /**
     * @brief writeRawData      -- 写数据
     * @param rawData           -- 代写数据（特殊格式字符串，未经过处理）
     * @param textFormat        -- SAKGlobal::SAKTextFormat，文本格式
     */
    void writeRawData(QString rawData, int textFormat);

    /**
     * @brief outputMessage 向ui输出信息
     * @param msg 带输出信息
     * @param isInfo true表示一般信息，false表示错误、警告信息
     */
    void outputMessage(QString msg, bool isInfo = true);
protected:
    /**
     * @brief setupDevice 初始化与设备有关的信号关联
     * @brief dev 设备实例指针
     */
    void setupDevice(SAKDevice *dev);

    /**
     * @brief refreshDevice         -- 刷新设备
     */
    virtual void refreshDevice();

    /**
     * @brief openOrColoseDevice    -- 打开或者关闭设备
     */
    virtual void openOrColoseDevice();
private:
    SAKDevice *device;
signals:
    /// 读取数据后发射该信号，参数为已读取的数据
    void bytesRead(QByteArray data);
    /// 发送数据后发射该信号，参数为已发送的数据
    void bytesWritten(QByteArray data);
    /// 子类关联该信号来发送数据即可
    void writeDataRequest(QByteArray data);
    /// 子类设备状态发生改变时(是否就绪)发送该信号
    void deviceStatusChanged(bool ready);
signals:
    void writeRawDataRequest(QString data, int textFormat);

    /// @brief 读写参数
public:
    struct ReadWriteParameters {
        int waitForBytesWrittenTime;
        int waitForReadyReadTime;
    };

    struct ReadWriteParameters readWriteParameters();
    void setReadWriteParameters(struct ReadWriteParameters parameters);
private:
    struct ReadWriteParameters _readWriteParameters;
    QMutex readWriteParametersQMutex;


protected:

    /**
     * @brief controllerWidget  -- 安装控制面板
     */
    virtual QWidget *controllerWidget();
    /**
     * @brief setUpController -- 安装控制器（控制面板）
     */
    void setUpController();
    /**
     * @brief changedDeviceStatus   -- 设备打开或者关闭时执行该函数
     * @param opened                -- true: 设备一打开 false：设备已关闭
     */
    void changedDeviceStatus(bool opened);
private:
    QTimer clearInfoTimer;
    void cleanInfo();

    // ------------------------------------------------------------------------
    // 设备设置
protected:    
    QPushButton *refreshPushButton              = Q_NULLPTR;  // 刷新按钮
    QPushButton *switchPushButton               = Q_NULLPTR;  // 打开关闭设备按钮
    QFrame      *deviceSettingFrame             = Q_NULLPTR;  // 控制面板
private slots:
    void on_refreshPushButton_clicked();
    void on_switchPushButton_clicked();

     // 输入设置组
protected:
    QComboBox   *inputModelComboBox             = Q_NULLPTR;  // 输入模式预选框
    QCheckBox   *cycleEnableCheckBox            = Q_NULLPTR;  // 循环使能复选框
    QLineEdit   *cycleTimeLineEdit              = Q_NULLPTR;  // 循环周期输入框
    QPushButton *readinFilePushButton           = Q_NULLPTR;  // 读入文件按钮
    QPushButton *saveInputDataPushButton        = Q_NULLPTR;  // 保存发送输入框数据
    QCheckBox   *addCRCCheckBox                 = Q_NULLPTR;  // 发送数据添加crc校验
    QCheckBox   *bigeEndianCheckBox             = Q_NULLPTR;  // crc值以大端形式添加
    QPushButton *clearInputPushButton           = Q_NULLPTR;  // 清空输入框按钮
    QPushButton *sendPushButton                 = Q_NULLPTR;  // 发送数据按钮
    QTextEdit   *inputTextEdit                  = Q_NULLPTR;  // 数据输入框
    QComboBox   *crcParameterModelsComboBox     = Q_NULLPTR;  // crc计算参数模型
    QLabel      *crcLabel                       = Q_NULLPTR;  // crc显示标签
    QPushButton *addInputItemPushButton;
    QPushButton *deleteInputItemPushButton;
    QListWidget *inputDataItemListWidget;
    QPushButton *presetPushButton;
    QPushButton *sendPresetPushButton;

    QString settingStringInputModel;
    QString settingStringCycleTime;
    QString settingStringAddCRC;
    QString settingStringBigeEndian;
    QString settingStringcrcParameterModel;
private slots:
    void on_inputModelComboBox_currentIndexChanged(int index);
    void on_cycleTimeLineEdit_textChanged(const QString &text);
    void on_addCRCCheckBox_clicked();
    void on_bigeEndianCheckBox_clicked();
    void on_crcParameterModelsComboBox_currentIndexChanged(int index);

    // 消息输出组管理
protected:
    QLabel *infoLabel                           = Q_NULLPTR;  // 消息输标签

    // 消息输出组
protected:
    QLabel      *rxLabel                        = Q_NULLPTR;  // 接受指示灯
    QLabel      *txLabel                        = Q_NULLPTR;  // 发送指示灯
    QComboBox   *outputTextFormatComboBox       = Q_NULLPTR;  // 输出模式复选框（接收）
    QCheckBox   *showDateCheckBox               = Q_NULLPTR;  // 显示日期使能复选框
    QCheckBox   *autoWrapCheckBox               = Q_NULLPTR;  // 自动换行使能复选框
    QCheckBox   *showTimeCheckBox               = Q_NULLPTR;  // 显示时间使能复选框
    QCheckBox   *showMsCheckBox                 = Q_NULLPTR;  // 显示毫秒使能复选框
    QCheckBox   *showRxDataCheckBox             = Q_NULLPTR;  // 显示接收数据使能复选框
    QCheckBox   *showTxDataCheckBox             = Q_NULLPTR;  // 显示发送数据使能复选框
    QCheckBox   *saveOutputFileToFilecheckBox   = Q_NULLPTR;  // 使能输出数据保存
    QPushButton *outputFilePathPushButton       = Q_NULLPTR;  // 显示输出设置面板
    QPushButton *clearOutputPushButton          = Q_NULLPTR;  // 清空输出按钮
    QPushButton *saveOutputPushButton           = Q_NULLPTR;  // 保存输出按钮
    QTextBrowser *outputTextBroswer             = Q_NULLPTR;  // 用于输出显示收发的数据

    QString settingStringOutputTextFormat;
    QString settingStringShowDate;
    QString settingStringAutoWrap;
    QString settingStringShowTime;
    QString settingStringShowMs;
    QString settingStringShowRx;
    QString settingStringShowTx;

private slots:
    void on_outputTextFormatComboBox_currentIndexChanged(int index);
    void on_showDateCheckBox_clicked();
    void on_autoWrapCheckBox_clicked();
    void on_showTimeCheckBox_clicked();
    void on_showMsCheckBox_clicked();
    void on_showRxDataCheckBox_clicked();
    void on_showTxDataCheckBox_clicked();

    // 数据统计
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

    // 其他设置
protected:
    QPushButton *transmissionSettingPushButton;
    QPushButton *readWriteSettingPushButton;
    QPushButton *autoResponseSettingPushButton;    
    QPushButton *timingSendingPushButton;
    QPushButton *highlightSettingPushButton;
    QPushButton *moreSettingsPushButton;

    // 数据可视化
protected:
    QPushButton *dataVisualizationPushButton;
private slots:
    void on_dataVisualizationPushButton_clicked();

private:
#ifdef SAK_IMPORT_CHARTS_MODULE
    SAKDataVisualizationManager *dataVisualizationManager;
#endif
    SAKOtherSettingsManager *otherSettings;
    SAKStatisticsManager    *statisticsManager;
    SAKDebugPageOutputManager  *outputManager;
    SAKDebugPageInputManager   *debugPageInputManager;

private:
    /**
     * @brief ui    -- 界面文件
     */
    Ui::SAKDebugPage *ui = Q_NULLPTR;

    /**
     * @brief initUiPointer -- 初始化指向ui控件的数据成员（指针）
     */
    void initUiPointer();    
public:
    friend class SAKDataVisualizationManager;
    friend class SAKOtherSettingsManager;
    friend class SAKStatisticsManager;
    friend class SAKDebugPageInputManager;
    friend class SAKDebugPageOutputManager;

private:
    bool isInitializing;
    int debugPageType = -1;
    QString settingKey;

    void initSettingKey();

    void initSettingString();
    void initInputSettingString();
    void initOutputSettingString();

    void readinSettings();
    void readinInputSettings();
    void readinOutputSettings();
};

#endif  // SAKTabPage_H
