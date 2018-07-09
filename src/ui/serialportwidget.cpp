#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "serialportwidget.h"
#include "ui_serialportwidget.h"

SerialPortWidget::SerialPortWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SerialportWidget),
    mpOutputWindow(new OutputWindow),
    mpInputWindow(new InputWindow),
    mpSendDataPanel(new SendDataPanel)
{
    /// 初始化UI
    ui->setupUi(this);
    QHBoxLayout *pOutputLayout = new QHBoxLayout;
    ui->widget_output->setLayout(pOutputLayout);
    pOutputLayout->addWidget(mpOutputWindow);
    pOutputLayout->setMargin(0);
    QHBoxLayout *pInputLayout = new QHBoxLayout;
    ui->widget_input->setLayout(pInputLayout);
    pInputLayout->addWidget(mpInputWindow);
    pInputLayout->setMargin(0);
    QHBoxLayout *pSendLayout = new QHBoxLayout;
    ui->widget_sendPanel->setLayout(pSendLayout);
    pSendLayout->addWidget(mpSendDataPanel);
    pSendLayout->setMargin(0);
    /// 获取本地串口设备
    RefreshDevice();
    /// 设置波特率
    SetBaudRate();
    /// 关联各种信号与槽
    Connect();
}

SerialPortWidget::~SerialPortWidget()
{

}

void SerialPortWidget::Connect()
{
    /// 说新设备
    connect(ui->pushButton_refresh, SIGNAL(clicked(bool)), this, SLOT(RefreshDevice()));
    /// 打开串口
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(Open()));
}

void SerialPortWidget::SetBaudRate()
{
    QList <qint32> baudRateList = QSerialPortInfo::standardBaudRates();
    foreach (qint32 baudRate, baudRateList) {
        ui->comboBox_baudrate->addItem(QString::number(baudRate));
    }
    ui->comboBox_baudrate->setCurrentText("9600");
}

void SerialPortWidget::RefreshDevice()
{
    ui->comboBox_serialport->clear();
    QList <QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    if (infoList.isEmpty()){
        ui->comboBox_serialport->addItem(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning), tr("未找到可用串口设备！"));
    }else {
        foreach (QSerialPortInfo info, infoList) {
            ui->comboBox_serialport->addItem(info.portName() + " " + info.description());
        }
    }
}

void SerialPortWidget::Open()
{
    static bool isOpen = false;
    if (isOpen){
        emit Need2Close();
        ui->pushButton_open->setText(tr("打开串口"));

        ui->comboBox_serialport->setEnabled(true);
        ui->comboBox_baudrate->setEnabled(true);
        ui->comboBox_databits->setEnabled(true);
        ui->comboBox_stopbits->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
        ui->pushButton_refresh->setEnabled(true);
    }else{
        QString portName, baudRate, dataBits, stopBits, parity;
        portName = ui->comboBox_serialport->currentText().split(' ').at(0);
        baudRate = ui->comboBox_baudrate->currentText();
        dataBits = ui->comboBox_databits->currentText();
        stopBits = ui->comboBox_stopbits->currentText();
        parity   = ui->comboBox_parity->currentText();

        emit Need2Open(portName, baudRate, dataBits, stopBits, parity);
        ui->pushButton_open->setText(tr("关闭串口"));

        ui->comboBox_serialport->setEnabled(false);
        ui->comboBox_baudrate->setEnabled(false);
        ui->comboBox_databits->setEnabled(false);
        ui->comboBox_stopbits->setEnabled(false);
        ui->comboBox_parity->setEnabled(false);
        ui->pushButton_refresh->setEnabled(false);
    }
    isOpen = !isOpen;
}
