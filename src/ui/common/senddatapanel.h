#ifndef SENDDATAPANEL_H
#define SENDDATAPANEL_H

#include <QWidget>
#include <QTimer>
#include <QDebug>

namespace Ui {
class SendDataPanel;
}

class SendDataPanel : public QWidget
{
    Q_OBJECT
public:
    SendDataPanel(QWidget *parent = Q_NULLPTR);
    ~SendDataPanel();
private:
    /// 循环周期
    QTimer *mpCycleTimer;
    Ui::SendDataPanel *ui;
    /// ----------------------------------------------------------
    /// 信号与槽关联
    void Connect();
    /// 初始化ui
    void InitUi();
public slots:
    /// 停止发送“发送数据信号”
    void StopSendDataSignal();
private slots:
    void CycleTimerTimeout();
    /// 循环设置发送
    void CycleSetting(bool isChecked);
    /// 发射发送数据信息
    void SendData();
signals:
    /// 外部程序只需关联该信号来发送数据
    void Need2WriteData();
};

#endif
