#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "TCPServerAssistant.h"
#include "TCPServerWidget.h"
#include "ui_TcpServerWidget.h"

TcpServerWidget::TcpServerWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::TcpServerWidget),
    mpOutputWindow(new OutputWindow),
    mpInputWindow(new InputWindow),
    mpSendDataPanel(new SendDataPanel),
    mpTcpServerAssistant(new TcpServerAssistant)
{
    mpTcpServerAssistant->start();
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

TcpServerWidget::~TcpServerWidget()
{

}

void TcpServerWidget::Connect()
{
    /// 打开套接字/关闭套接字
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(Open()));
    connect(this, SIGNAL(Need2Open(QString,QString)), mpTcpServerAssistant, SLOT(Open(QString,QString)));
    connect(this, SIGNAL(Need2Close()), mpTcpServerAssistant, SLOT(Close()));
    connect(mpTcpServerAssistant, SIGNAL(TcpServerOpenSuccessfully()), this, SLOT(AfterUdpSocketOpen()));
    connect(mpTcpServerAssistant, SIGNAL(TcpServerCloseSuccessfully()), this, SLOT(AfterUdpSocketClose()));
    /// 接收数据
    connect(mpTcpServerAssistant, SIGNAL(NewDataHadBeenRead(QByteArray)), mpOutputWindow, SLOT(OutputData(QByteArray)));
    /// 发送数据
    connect(mpSendDataPanel, SIGNAL(Need2WriteData()), this, SLOT(Write()));
    connect(this, SIGNAL(Need2Write(QByteArray)), mpTcpServerAssistant, SLOT(Write(QByteArray)));
    /// 更新客户端列表
    connect(mpTcpServerAssistant, SIGNAL(UpdateTcpClientList(QList<QTcpSocket*>)), this, SLOT(UpdateClientList(QList<QTcpSocket*>)));

}

void TcpServerWidget::InitHostAddress()
{
    QList <QHostAddress> addressList =  QNetworkInterface::allAddresses();
    foreach(QHostAddress address, addressList){
        if (address.toIPv4Address()){
            ui->comboBox_localAddress->addItem(address.toString());
        }
    }
}

void TcpServerWidget::Open()
{
    if (mIsOpen){
        emit Need2Close();
    }else{
        QString localhostAddress = ui->comboBox_localAddress->currentText();
        QString localhostPort = ui->lineEdit_localPort->text();

        emit Need2Open(localhostAddress, localhostPort);
    }
}

void TcpServerWidget::AfterUdpSocketClose()
{
    ui->pushButton_open->setText(tr("连接服务器"));
    mpSendDataPanel->StopSendDataSignal();

    ui->comboBox_localAddress->setEnabled(true);
    ui->lineEdit_localPort->setEnabled(true);

    mIsOpen = false;
}

void TcpServerWidget::AfterUdpSocketOpen()
{
    ui->pushButton_open->setText(tr("断开连接"));
    ui->comboBox_localAddress->setEnabled(false);
    ui->lineEdit_localPort->setEnabled(false);

    mIsOpen = true;
}

void TcpServerWidget::Write()
{
    QByteArray data = mpInputWindow->Data();

    emit Need2Write(data);
}

void TcpServerWidget::UpdateClientList(QList<QTcpSocket *> list)
{
    ui->comboBox_client->clear();
    if (list.length() == 0){
        ui->comboBox_client->addItem(tr("无客户端连接"));
    }else {
        QString str;
        for (int i = 0; i < list.length(); i++){
            str = list.at(i)->peerAddress().toString() + ":" + QString::number(list.at(i)->peerPort());
            ui->comboBox_client->addItem(str);
        }
    }
}
