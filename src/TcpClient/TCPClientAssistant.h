#ifndef TCPCLIENTASSISTANT_H
#define TCPCLIENTASSISTANT_H

#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <QHostAddress>

class TcpClientAssistant : public QThread
{
    Q_OBJECT
public:
    TcpClientAssistant(QObject *parent = Q_NULLPTR);
    ~TcpClientAssistant();
    /// -------------------------------------------------
    QString ErrorString(){return mErrorString;}
public slots:
    /// 关闭串口
    void Close();
    /// 打开串口
    void Open(QString localhostAddress, QString LocalhostPort, QString serverAddress, QString serverPort);
    /// 读取串口数据
    QByteArray ReadAll();
    /// 向串口发送数据,data位需要发送的数据
    qint64 Write(QByteArray data);
protected:
    void run();
private:
    /// 错误信息描述
    QString mErrorString = "No error!";
    /// 套接字
    QTcpSocket *mpTcpSocket;
    /// 输出信息，color为输出文本要显示的颜色，prefix决定显示信息是是否显示日期时间前缀.
    void OutputInfo(QString info, QString color = "black", bool prefix = true){emit Need2OutputInfo(info, color, prefix);}
private slots:
    void Disconnect();
signals:
    /// tcp套接字关闭成功
    void TcpSocketCloseSuccessfully();
    /// tcp套接字打开成功后发射该信号
    void TcpSocketOpenSuccessfully();
    /// 读到的数据后发射该信息
    void NewDataHadBeenRead(QByteArray data);
    /// 发送一帧数据后，发射该信号，data为已发送的数据
    void NewDataHadBeenWrite(QByteArray data);
    /// 外部程序关联该信号，可以获取需要输出的信息
    void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};

#endif
