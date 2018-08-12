#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "UdpClientAssistant.h"

UdpClientAssistant::UdpClientAssistant(QObject *parent):
    QThread(parent)
{
    /// 一个神奇的操作
    moveToThread(this);
}

UdpClientAssistant::~UdpClientAssistant()
{

}

QByteArray UdpClientAssistant::ReadAll()
{
    QByteArray datagram;
    while (mpUdpSocket->hasPendingDatagrams()){
        datagram.resize(mpUdpSocket->pendingDatagramSize());
        mpUdpSocket->readDatagram(datagram.data(), datagram.length());
    }
    emit NewDataHadBeenRead(datagram);
    return datagram;
}

qint64 UdpClientAssistant::Write(QByteArray data, QString hostAddress, QString port)
{
    qint64 ret = mpUdpSocket->writeDatagram(data, QHostAddress(hostAddress), quint16(port.toInt()));
    if (ret == -1){
        mErrorString = mpUdpSocket->errorString();
    }else {
        mErrorString = "No error!";
        emit NewDataHadBeenWrite(data);
    }
    return ret;
}

void UdpClientAssistant::Open(QString hostAddress, QString port)
{
    if (mpUdpSocket->isOpen()){
        mpUdpSocket->close();
    }else {
        if (port.isEmpty()){
            port = "0";
        }
        if (mpUdpSocket->bind(QHostAddress(hostAddress), (qint16)port.toInt())){
            if (mpUdpSocket->open(QUdpSocket::ReadWrite)){
                emit UdpSocketOpenSuccessfully();
            }else{

            }
        }else {
            qWarning() << tr("绑定主机地址或者端口失败！");
            return;
        }
    }
}

void UdpClientAssistant::Close()
{
    mpUdpSocket->close();
    emit UdpSocketCloseSuccessfully();
}

void UdpClientAssistant::run()
{
     mpUdpSocket = new QUdpSocket;
    /// 读就绪信号关联
    connect(mpUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadAll()));
    this->exec();
}
