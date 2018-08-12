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

#include "TCPClientAssistant.h"
#include "TcpClientWidget.h"
#include "ui_TcpClientWidget.h"

TcpClientWidget::TcpClientWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TcpClientWidget),
    mpOutputWindow(new OutputWindow),
    mpInputWindow(new InputWindow),
    mpSendDataPanel(new SendDataPanel),
    mpTcpClientAssistant(new TcpClientAssistant)
{
    mpTcpClientAssistant->start();
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

TcpClientWidget::~TcpClientWidget()
{

}

void TcpClientWidget::Connect()
{
    /// 打开套接字/关闭套接字
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(Open()));
    connect(this, SIGNAL(Need2Open(QString,QString,QString,QString)), mpTcpClientAssistant, SLOT(Open(QString,QString,QString,QString)));
    connect(this, SIGNAL(Need2Close()), mpTcpClientAssistant, SLOT(Close()));
    connect(mpTcpClientAssistant, SIGNAL(TcpSocketOpenSuccessfully()), this, SLOT(AfterUdpSocketOpen()));
    connect(mpTcpClientAssistant, SIGNAL(TcpSocketCloseSuccessfully()), this, SLOT(AfterUdpSocketClose()));
    /// 接收数据
    connect(mpTcpClientAssistant, SIGNAL(NewDataHadBeenRead(QByteArray)), mpOutputWindow, SLOT(OutputData(QByteArray)));
    /// 发送数据
    connect(mpSendDataPanel, SIGNAL(Need2WriteData()), this, SLOT(Write()));
    connect(this, SIGNAL(Need2Write(QByteArray)), mpTcpClientAssistant, SLOT(Write(QByteArray)));

}

void TcpClientWidget::InitHostAddress()
{
    QList <QHostAddress> addressList =  QNetworkInterface::allAddresses();
    foreach(QHostAddress address, addressList){
        if (address.toIPv4Address()){
            ui->comboBox_localAddress->addItem(address.toString());
        }
    }
}

void TcpClientWidget::Open()
{
    if (mIsOpen){
        emit Need2Close();
    }else{
        QString localhostAddress = ui->comboBox_localAddress->currentText();
        QString localhostPort = ui->lineEdit_localPort->text();
        QString serverAddress = ui->lineEdit_peerAddress->text();
        QString serverPort = ui->lineEdit_peerPort->text();

        emit Need2Open(localhostAddress, localhostPort, serverAddress, serverPort);
    }
}

void TcpClientWidget::AfterUdpSocketClose()
{
    ui->pushButton_open->setText(tr("连接服务器"));
    mpSendDataPanel->StopSendDataSignal();

    ui->comboBox_localAddress->setEnabled(true);
    ui->lineEdit_localPort->setEnabled(true);
    ui->lineEdit_peerAddress->setEnabled(true);
    ui->lineEdit_peerPort->setEnabled(true);

    mIsOpen = false;
}

void TcpClientWidget::AfterUdpSocketOpen()
{
    ui->pushButton_open->setText(tr("断开连接"));
    ui->comboBox_localAddress->setEnabled(false);
    ui->lineEdit_localPort->setEnabled(false);
    ui->lineEdit_peerAddress->setEnabled(false);
    ui->lineEdit_peerPort->setEnabled(false);

    mIsOpen = true;
}

void TcpClientWidget::Write()
{
    QByteArray data = mpInputWindow->Data();

    emit Need2Write(data);
}
