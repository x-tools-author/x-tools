/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTABPAGE_HH
#define SAKTABPAGE_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>

class SAKDataFactory;
class SAKCRCInterface;
class SAKDebugPageDevice;
class SAKTransmissionSettings;
class SAKHighlightSettingsWidget;
class SAKDebugPageOtherController;
class SAKDebugPageInputController;
class SAKDebugPageOutputController;
#ifdef SAK_IMPORT_CHARTS_MODULE
class SAKDebugPageChartsController;
#endif
class SAKDebugPageDatabaseInterface;
class SAKDebugPageStatisticsController;

namespace Ui {
    class SAKDebugPage;
}

/// @brief Debugging page
class SAKDebugPage : public QWidget
{
    Q_OBJECT
public:
    SAKDebugPage(int type, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPage();

    /// @brief 读写参数，参数单位为ms
    struct ReadWriteParameters {
        int waitForBytesWrittenTime;// 写等待时间
        int waitForReadyReadTime;   // 读就绪等待时间
        int runIntervalTime;        // while循环执行时间间隔
    };

    friend class SAKDebugPageOtherController;
    friend class SAKDebugPageInputController;
    friend class SAKDebugPageChartsController;
    friend class SAKDebugPageOutputController;
    friend class SAKDebugPageStatisticsController;

    /**
     * @brief write: Write data to device
     * @param data: Data need to be written
     */
    void write(QByteArray data);

    /**
     * @brief writeRawData: Write raw data
     * @param rawData: Input text
     * @param textFormat: input text fromat
     */
    void writeRawData(QString rawData, int textFormat);

    /**
     * @brief outputMessage: Output message to ui
     * @param msg: The message that need to be show
     * @param isInfo: true-text color is green, false-text color is red
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
     * @brief databaseInterfaceInstance: Get the interface for database operation
     * @return The interface for database operation
     */
    SAKDebugPageDatabaseInterface *databaseInterfaceInstance();

    /**
     * @brief pageType: The type of debugging page.(SAKDataStruct::SAKEnumDebugPageType)
     * @return Type of debugging page
     */
    quint32 pageType();

    /**
     * @brief otherController: Get SAKDebugPageOtherController instance pointer
     * @return SAKDebugPageOtherController instance pointer
     */
    SAKDebugPageOtherController *otherController();

    /**
     * @brief inputController: Get SAKDebugPageInputController instance pointer
     * @return SAKDebugPageInputController instance pointer
     */
    SAKDebugPageInputController *inputController();

#ifdef SAK_IMPORT_CHARTS_MODULE
    /**
     * @brief chartsController: Get SAKDebugPageChartsController instance pointer
     * @return SAKDebugPageChartsController instance pointer
     */
    SAKDebugPageChartsController *chartsController();
#endif

    /**
     * @brief outputController: Get SAKDebugPageOutputController instance pointer
     * @return SAKDebugPageOutputController instance pointer
     */
    SAKDebugPageOutputController *outputController();

    /**
     * @brief statisticsController: Get SAKDebugPageStatisticsController instance pointer
     * @return SAKDebugPageStatisticsController instance pointer
     */
    SAKDebugPageStatisticsController *statisticsController();
protected:
    /**
     * @brief refreshDevice: Refresh system device
     */
    virtual void refreshDevice();

    /**
     * @brief controllerWidget: Get device control widget
     * @return Device control widget
     */
    virtual QWidget *controllerWidget();

    /**
     * @brief createDevice: Create the device instance
     * @return Device instance pointer
     */
    virtual SAKDebugPageDevice* createDevice();

    /**
     * @brief setUiEnable: Set some components enable or disable
     * @param ebable: true-enable ui components, false-disable ui components
     */
    virtual void setUiEnable(bool ebable);

    /**
     * @brief initPage: Initializing,
     * the function must be called in the constructor of child class
     */
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

    // Debug page modules
    SAKDebugPageOtherController *mOtherController;
    SAKDebugPageInputController *mInputController;
#ifdef SAK_IMPORT_CHARTS_MODULE
    SAKDebugPageChartsController *mChartsController;
#endif
    SAKDebugPageOutputController *mOutputController;
    SAKDebugPageStatisticsController *mStatisticsController;
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
};

#endif  // SAKTABPAGE_HH
