/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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

class SAKDebugPageDevice;
class SAKDataFactory;
class SAKCRCInterface;
class SAKDebugPageStatisticsController;
class SAKDebugPageOtherController;
class SAKTransmissionSettings;
class SAKDebugPageInputController;
class SAKDebugPageOutputController;
class SAKHighlightSettingsWidget;
#ifdef SAK_IMPORT_CHARTS_MODULE
class SAKChartsController;
#endif
class SAKDebugPageDatabaseInterface;

namespace Ui {
    class SAKDebugPage;
}

/// @brief 调试页面
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

    /// @brief 读写参数，参数单位为ms
    struct ReadWriteParameters {
        int waitForBytesWrittenTime;// 写等待时间
        int waitForReadyReadTime;   // 读就绪等待时间
        int runIntervalTime;        // while循环执行时间间隔
    };

    friend class SAKChartsController;
    friend class SAKDebugPageOtherController;
    friend class SAKDebugPageStatisticsController;
    friend class SAKDebugPageInputController;
    friend class SAKDebugPageOutputController;

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

    /**
     * @brief readWriteParameters 获取读写参数
     * @return 读写参数
     */
    ReadWriteParameters readWriteParameters();

    /**
     * @brief setReadWriteParameters 设置读写参数
     * @param parameters 读写参数
     */
    void setReadWriteParameters(ReadWriteParameters parameters);

    /**
     * @brief databaseInterfaceInstance 获取数据库接口类实例
     * @return 数据库接口类实例
     */
    SAKDebugPageDatabaseInterface *databaseInterfaceInstance();

    /**
     * @brief pageType 获取调试页面类型（SAKDataStruct::SAKEnumDebugPageType）
     * @return 调试页面类型
     */
    quint32 pageType();
protected:
    /// @brief 刷新设备
    virtual void refreshDevice();
    /// @brief 返回控制面板
    virtual QWidget *controllerWidget();
    /// @brief 创建设备,创建的设备由基类管理，不能在子类中对设备进行销毁操作
    virtual SAKDebugPageDevice* createDevice();
    /// @brief 使能ui
    virtual void setUiEnable(bool ebable);
    /// @brief 初始化页面，子类在重新实现所有虚函数后，条用该函数即可
    void initPage();
private:
    SAKDebugPageDevice *mDevice;
    bool mIsInitializing;
    int mDebugPageType = -1;
    QString mSettingKey;
    QTimer mClearInfoTimer;
    struct ReadWriteParameters mRreadWriteParameters;
    QMutex mReadWriteParametersMutex;
    SAKDebugPageDatabaseInterface *mDatabaseInterface;
private:
    void initSettingKey();
    /// @brief 初始化配置选项名称
    void initSettingString();
    void initInputSettingString();
    void initOutputSettingString();

    /// @brief 从配置文件中读入配置选项
    void readinSettings();
    void readinInputSettings();
    void readinOutputSettings();

    /// @brief 清空ui消息显示
    void cleanInfo();

    /// @brief 打开或者关闭设备
    void openOrColoseDevice();
    void closeDevice();
    void openDevice();

    /// 初始化与设备有关的信号关联
    void setupDevice();
    /// @brief 安装设备控制面板
    void setupController();
    /// @brief 更改设备状态
    void changedDeviceState(bool opened);
signals:
    /// 读取数据后发射该信号，参数为已读取的数据
    void bytesRead(QByteArray data);
    /// 发送数据后发射该信号，参数为已发送的数据
    void bytesWritten(QByteArray data);
    /// 子类关联该信号来发送数据即可
    void requestWriteData(QByteArray data);
    /// 请求处理输出
    void requestWriteRawData(QString data, int textFormat);





    /*************************************************************************/
    /// @brief ui文件初始化
private:
    Ui::SAKDebugPage *mUi;
private:
    /// @brief initUiPointer -- 初始化指向ui控件的数据成员（指针）
    void initUiPointer();

    /*************************************************************************/
    /// @brief 设备设置
protected:
    QPushButton *mRefreshPushButton;
    QPushButton *mSwitchPushButton;
    QFrame *mDeviceSettingFrame;
private slots:
    void on_refreshPushButton_clicked();
    void on_switchPushButton_clicked();

    /*************************************************************************/
    /// @brief 输入设置组
protected:
    QComboBox *mInputModelComboBox;
    QCheckBox *mCycleEnableCheckBox;
    QLineEdit *mCycleTimeLineEdit;
    QPushButton *mReadinFilePushButton;
    QPushButton *mSaveInputDataPushButton;
    QCheckBox *mAddCRCCheckBox;
    QPushButton *mCrcSettingsPushButton;
    QPushButton *mClearInputPushButton;
    QPushButton *mSendPushButton;
    QTextEdit *mInputTextEdit;
    QComboBox *mCrcParameterModelsComboBox;
    QLabel *mCrcLabel;
    QPushButton *mAddInputItemPushButton;
    QPushButton *mDeleteInputItemPushButton;
    QListWidget *mInputDataItemListWidget;
    QPushButton *mPresetPushButton;
    QPushButton *mSendPresetPushButton;

    QString mSettingStringInputModel;
    QString mSettingStringCycleTime;
    QString mSettingStringAddCRC;
    QString mSettingStringBigeEndian;
    QString mSettingStringcrcParameterModel;
private slots:
    void on_inputModelComboBox_currentIndexChanged(int index);
    void on_cycleTimeLineEdit_textChanged(const QString &text);
    void on_addCRCCheckBox_clicked();
    void on_crcSettingsPushButton_clicked();
    void on_crcParameterModelsComboBox_currentIndexChanged(int index);

    /*************************************************************************/
    /// @brief 消息输出组管理
protected:
    QLabel *mInfoLabel;

    /*************************************************************************/
    /// @brief 数据输出组
protected:
    QLabel *mRxLabel;
    QLabel *mTxLabel;
    QComboBox *mOutputTextFormatComboBox;
    QCheckBox *mShowDateCheckBox;
    QCheckBox *mAutoWrapCheckBox;
    QCheckBox *mShowTimeCheckBox;
    QCheckBox *mShowMsCheckBox;
    QCheckBox *mShowRxDataCheckBox;
    QCheckBox *mShowTxDataCheckBox;
    QCheckBox *mSaveOutputToFileCheckBox;
    QPushButton *mOutputFilePathPushButton;
    QPushButton *mClearOutputPushButton;
    QPushButton *mSaveOutputPushButton;
    QTextBrowser *mOutputTextBroswer;

    QString mSettingStringOutputTextFormat;
    QString mSettingStringShowDate;
    QString mSettingStringAutoWrap;
    QString mSettingStringShowTime;
    QString mSettingStringShowMs;
    QString mSettingStringShowRx;
    QString mSettingStringShowTx;
private slots:
    void on_outputTextFormatComboBox_currentIndexChanged(int index);
    void on_showDateCheckBox_clicked();
    void on_autoWrapCheckBox_clicked();
    void on_showTimeCheckBox_clicked();
    void on_showMsCheckBox_clicked();
    void on_showRxDataCheckBox_clicked();
    void on_showTxDataCheckBox_clicked();

    /*************************************************************************/
    /// @brief 数据统计
protected:
    bool mReceivedFlag;
    bool mSendFlag;

    QLabel *mRxSpeedLabel;
    QLabel *mTxSpeedLabel;
    QLabel *mRxFramesLabel;
    QLabel *mTxFramesLabel;
    QLabel *mRxBytesLabel;
    QLabel *mTxBytesLabel;
    QPushButton *mResetTxCountPushButton;
    QPushButton *mResetRxCountPushButton;

    /*************************************************************************/
    /// @brief 其他设置
protected:
    QPushButton *mTransmissionSettingPushButton;
    QPushButton *mReadWriteSettingPushButton;
    QPushButton *mAutoResponseSettingPushButton;
    QPushButton *mTimingSendingPushButton;
    QPushButton *mHighlightSettingPushButton;
    QPushButton *mMoreSettingsPushButton;

    /*************************************************************************/
    /// @brief 数据可视化
protected:
    QPushButton *mDataVisualizationPushButton;
private slots:
    void on_dataVisualizationPushButton_clicked();
private:
#ifdef SAK_IMPORT_CHARTS_MODULE
    SAKChartsController *mDataVisualizationManager;
#endif
    SAKDebugPageOtherController *mOtherSettings;
    SAKDebugPageStatisticsController *mStatisticsManager;
    SAKDebugPageOutputController *mOutputManager;
    SAKDebugPageInputController *mDebugPageInputManager;
};

#endif  // SAKTabPage_H
