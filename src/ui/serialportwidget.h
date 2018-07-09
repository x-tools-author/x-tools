#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QApplication>

#include "outputwindow.h"
#include "inputwindow.h"
#include "senddatapanel.h"

namespace Ui {
class SerialportWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    SerialPortWidget(QWidget *parent = Q_NULLPTR);
    ~SerialPortWidget();
    /// -------------------------------------------------------
private:
    Ui::SerialportWidget *ui;
    OutputWindow *mpOutputWindow;
    InputWindow *mpInputWindow;
    SendDataPanel *mpSendDataPanel;
    /// -------------------------------------------------------
    /// 关联各种信号与槽
    void Connect();
    /// 标准波特率
    void SetBaudRate();
private slots:
    /// 打开串口
    void Open();
    /// 获取串口设备
    void RefreshDevice();
signals:
    /// 关闭串口
    void Need2Close();
    /// 外部关联该信号来打开串口
    void Need2Open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
};

#endif
