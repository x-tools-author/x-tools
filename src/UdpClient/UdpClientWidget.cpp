/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "UdpClientAssistant.h"
#include "UdpClientWidget.h"
#include "ui_UdpClientWidget.h"

UdpClientWidget::UdpClientWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::UdpClientWidget),
    mpOutputWindow(new OutputWindow),
    mpInputWindow(new InputWindow),
    mpSendDataPanel(new SendDataPanel),
    mpUdpClientAssistant(new UdpClientAssistant)
{
    mpUdpClientAssistant->start();
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
    /// 关联各种信号与槽
    Connect();
    /// 获取主机地址
    InitHostAddress();
}

UdpClientWidget::~UdpClientWidget()
{

}

void UdpClientWidget::Connect()
{
    /// 打开套接字/关闭套接字
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(Open()));
    connect(this, SIGNAL(Need2Open(QString,QString)), mpUdpClientAssistant, SLOT(Open(QString,QString)));
    connect(this, SIGNAL(Need2Close()), mpUdpClientAssistant, SLOT(Close()));
    connect(mpUdpClientAssistant, SIGNAL(UdpSocketOpenSuccessfully()), this, SLOT(AfterUdpSocketOpen()));
    connect(mpUdpClientAssistant, SIGNAL(UdpSocketCloseSuccessfully()), this, SLOT(AfterUdpSocketClose()));
    /// 接收数据
    connect(mpUdpClientAssistant, SIGNAL(NewDataHadBeenRead(QByteArray)), mpOutputWindow, SLOT(OutputData(QByteArray)));
    /// 发送数据
    connect(mpSendDataPanel, SIGNAL(Need2WriteData()), this, SLOT(Write()));
    connect(this, SIGNAL(Need2Write(QByteArray, QString, QString)), mpUdpClientAssistant, SLOT(Write(QByteArray,QString,QString)));

}

void UdpClientWidget::InitHostAddress()
{
    QList <QHostAddress> addressList =  QNetworkInterface::allAddresses();
    foreach(QHostAddress address, addressList){
        if (address.toIPv4Address()){
            ui->comboBox_localAddress->addItem(address.toString());
        }
    }
}

void UdpClientWidget::Open()
{
    if (mIsOpen){
        emit Need2Close();
    }else{
        QString hostAddress = ui->comboBox_localAddress->currentText();
        QString hostPort = ui->lineEdit_localPort->text();
        emit Need2Open(hostAddress, hostPort);
    }
}

void UdpClientWidget::AfterUdpSocketClose()
{
    ui->pushButton_open->setText(tr("监听端口"));
    mpSendDataPanel->StopSendDataSignal();

    ui->comboBox_localAddress->setEnabled(true);
    ui->lineEdit_localPort->setEnabled(true);
    ui->lineEdit_peerAddress->setEnabled(true);
    ui->lineEdit_peerPort->setEnabled(true);

    mIsOpen = false;
}

void UdpClientWidget::AfterUdpSocketOpen()
{
    ui->pushButton_open->setText(tr("停止监听"));
    ui->comboBox_localAddress->setEnabled(false);
    ui->lineEdit_localPort->setEnabled(false);
    ui->lineEdit_peerAddress->setEnabled(false);
    ui->lineEdit_peerPort->setEnabled(false);

    mIsOpen = true;
}

void UdpClientWidget::Write()
{
    QByteArray data = mpInputWindow->Data();
    QString peerHostAddress = ui->lineEdit_peerAddress->text();
    QString peerHostPort = ui->lineEdit_peerPort->text();

    emit Need2Write(data, peerHostAddress, peerHostPort);
}
