/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef SAKTABPAGE_HH
#define SAKTABPAGE_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>

#include "SAKIODevice.hh"
#include "SAKReadWriteSetting.hh"
#include "SAKIODeviceControler.hh"
#include "SAKAutoResponseSettingPanel.hh"

class SAKHighlighterSettingPanel;

namespace Ui {
class SAKTabPage;
}

/*
 * Hei, Developer! Takeacre of this warning!
 * 由于使用了信号与槽的自动关联，指向ui控件的指针变量的名称必须与ui控件的objectName保持一致。
 * 使用designer改变ui控件变量名称时，objectName属性会自动更新，不要在designer中手动更改ui控件的objectName。
 * objectName与变量名称不一致会导致信号与槽的关联失效。
 */

class SAKTabPage : public QWidget
{
    Q_OBJECT
public:
    SAKTabPage(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~SAKTabPage();

    void setCustomControler(SAKIODeviceControler *controler);
    void setRefreshPBtText(QString text);

    /**
     * @brief The TextDisplayModel 输出输入显示模式
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
protected:
    // 设备管理组
    QPushButton *refreshPushButton              = nullptr;  /// 刷新按钮
    QPushButton *switchPushButton               = nullptr;  /// 打开关闭设备按钮
    QLabel      *rxLabel                        = nullptr;  /// 接受指示灯
    QLabel      *txLabel                        = nullptr;  /// 发送指示灯
private slots:
    void on_refreshPushButton_clicked(){emit need2refresh();}
    void on_switchPushButton_clicked(){emit need2openOrClose();}

     // 输入设置组
protected:
    TextDisplayModel inputModel;
    bool cyclEnable;
    quint32 cycleTime = 0;

    QComboBox   *inputModelComboBox             = nullptr;  /// 输入模式预选框
    QCheckBox   *cycleEnableCheckBox            = nullptr;  /// 循环使能复选框
    QLineEdit   *cycleTimeLineEdit              = nullptr;  /// 循环周期输入框
    QPushButton *readinFilePushButton           = nullptr;  /// 读入文件按钮
    QPushButton *clearInputPushButton           = nullptr;  /// 清空输入框按钮
    QPushButton *sendPushButton                 = nullptr;  /// 发送数据按钮
    QTextEdit   *inputTextEdit                  = nullptr;  /// 数据输入框        
private slots:
    void on_inputModelComboBox_currentTextChanged(const QString &text);
    void on_cycleEnableCheckBox_clicked();
    void on_cycleTimeLineEdit_textChanged(const QString &text);
    void on_readinFilePushButton_clicked();
    void on_clearInputPushButton_clicked();
    void on_sendPushButton_clicked();

    //数据输出组管理
protected:
    TextDisplayModel outputTextModel = SAKTabPage::Hex;
    bool showDate;
    bool autoWrap;
    bool showTime;
    bool showMS;
    bool showRxData;
    bool showTxData;

    QComboBox   *outputModelComboBox            = nullptr;  /// 输出模式复选框
    QCheckBox   *showDateCheckBox               = nullptr;  /// 显示日期使能复选框
    QCheckBox   *autoWrapCheckBox               = nullptr;  /// 自动换行使能复选框
    QCheckBox   *showTimeCheckBox               = nullptr;  /// 显示时间使能复选框
    QCheckBox   *showMsCheckBox                 = nullptr;  /// 显示毫秒使能复选框
    QCheckBox   *showRxDataCheckBox             = nullptr;  /// 显示接收数据使能复选框
    QCheckBox   *showTxDataCheckBox             = nullptr;  /// 显示发送数据使能复选框
    QPushButton *clearOutputPushButton          = nullptr;  /// 清空输出按钮
    QPushButton *saveOutputPushButton           = nullptr;  /// 保存输出按钮
    QTextBrowser *outputTextBroswer             = nullptr;  /// 用于输出显示收发的数据
private slots:
    void on_outputModelComboBox_currentTextChanged(const QString &text);
    void on_showDateCheckBox_clicked();
    void on_autoWrapCheckBox_clicked();
    void on_showTimeCheckBox_clicked();
    void on_showMsCheckBox_clicked();
    void on_showRxDataCheckBox_clicked();
    void on_showTxDataCheckBox_clicked();
    void on_clearOutputPushButton_clicked();
    void on_saveOutputPushButton_clicked();


    // 数据管理组
protected:
    quint64     receiveFrames                   = 0;        /// 接受帧数
    quint64     receiveBytes                    = 0;        /// 接受字节数
    quint64     sendFrames                      = 0;        /// 发送帧数
    quint64     sendBytes                       = 0;        /// 发送字节数

    QLabel      *rxFramesLabel                  = nullptr;  /// 用于显示接受帧数
    QLabel      *txFramesLabel                  = nullptr;  /// 用于显示发送帧数
    QLabel      *rxBytesLabel                   = nullptr;  /// 用于显示接收字节数
    QLabel      *txBytesLabel                   = nullptr;  /// 用于显示发送字节数
    QPushButton *resetRxCountPushButton         = nullptr;  /// 重置接收计数
    QPushButton *resetTxCountPushButton         = nullptr;  /// 重置发送计数
private slots:
    void on_resetRxCountPushButton_clicked();
    void on_resetTxCountPushButton_clicked();


    // 其他设置组
protected:
    QPushButton *autoResponseSettingPushButton  = nullptr;  /// 自动回复设置面板调出按钮
    QPushButton *highlightSettingPushButton     = nullptr;  /// 高亮设置面板调出按钮
    QPushButton *readWriteSettingPushButton     = nullptr;  /// 读写设置面包调出按钮
private slots:
    void on_autoResponseSettingPushButton_clicked();        /// 创建一个高亮设置窗口并显示，该窗口关闭后将被销毁
    void on_highlightSettingPushButton_clicked();           /// 创建一个高亮设置窗口并显示，该窗口关闭后将被销毁
    void on_readWriteSettingPushButton();                   /// 创建一个读写参数设置窗口并显示，该窗口关闭后将被销毁



private:
    /**
     * @brief ui    -- 界面文件
     */
    Ui::SAKTabPage *ui                  = nullptr;

    /**
     * @brief initUiPointer -- 初始化指向ui控件的数据成员（指针）
     */
    void initUiPointer();    
    void setLabelText(QLabel* label, quint64 text);
private:
    const char *logCategory = "SAKTabPage";
    SAKIODevice                 *device                     = nullptr;
    SAKIODeviceControler        *controler                  = nullptr;
    SAKAutoResponseSettingPanel *autoResponseSettingPanel   = nullptr;
    QTimer                      *cycleTimer                 = nullptr;
    QHBoxLayout                 *customControlerLayout      = nullptr;
    QTimer                      *clearInfoTimer             = nullptr;
    SAKHighlighterSettingPanel  *highlighterSettingPanel    = nullptr;
    QPushButton                 *highlighterSettingButton   = nullptr;

    QByteArray                  dataTemp                    = "(null)";
    const QSize                 rxtxSize                    = QSize(18, 18);

    QString                     inputTextMode               = "Hex";
    QString                     outputTextMode              = "Hex";

    QComboBox*                  inputTextModelComboBox      = nullptr;
    QComboBox*                  outputTextModelComboBox     = nullptr;

    QMetaEnum                   textModel;

    QPushButton*                rwParameterSettingButton    = nullptr;
    SAKReadWriteSetting                 rwParameterSettingDialog;
    ///----------------------------------------------------------------
    void initUI();
    QByteArray dataBytes();
    void Connect();

    QString readSetting(QString &option);
    void writeSetting(QString &option, QString &value);

    /**
     * @brief refreshOutputData -- 更新输出数据
     * @param data              -- 需要输出显示的数据
     * @param isReceivedData    -- true表示数据是接受到的数据，否则是发送数据
     */
    void refreshOutputData(QByteArray &data, bool isReceivedData = true);
private slots:
    virtual void updateRxImage();
    virtual void updateTxImage();

    void cancleCycle();

    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode(QString mode);
    void readOutputMode();

    void setInputMode(QString mode);
    void readInputMode();

    void textFormatControl();

    void outputTimeInfoCheckBoxClicked(bool checked);
    void outputReceiveDataOnlyCheckBoxClicked(bool cheaked);

    void openOrClose();
    void open(){emit need2open();}
    void close(){emit need2close();}
    void refresh(){emit need2refresh();}

    void afterDeviceOpen();
    void afterDeviceClose();

    void cycleTimerTimeout();
    void checkedBoxCycleClicked(bool checked);

    void outputInfo(QString info, QString color = "black", bool prefix = true);
    void outputErrorString(QString str);
    void outputInformationString(QString str);
    void clearInfo();

    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void writeBytes();

    void resetSendDataCount();
    void resetReceiveDataCount();

    void setAutoResponseFlag(bool enableAutoResponse);
    void readAutoResponseFlag();

    void handleReadBytes(QByteArray data);
signals:
    void need2writeBytes(QByteArray data);
    void need2open();
    void need2close();
    void need2refresh();
    void need2openOrClose();
    void need2write();
};

#endif  // SAKTabPage_H
