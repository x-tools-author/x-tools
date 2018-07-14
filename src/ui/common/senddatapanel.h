#ifndef SENDDATAPANEL_H
#define SENDSATAPANEL_H

#include <QWidget>

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
    Ui::SendDataPanel *ui;
    /// ----------------------------------------------------------
    /// 信号与槽关联
    void Connect();
    /// 初始化ui
    void InitUi();
private slots:
    void SendData();
signals:
    /// 外部程序可以关联该信号来发送数据
    void Need2SendData(bool cycle = false, quint64 cycleTime = 0);
    /// 停止循环发送数据
    void StopCycling();
};

#endif
