#ifndef TCPSERVERTASSISTANT_H
#define TCPSERVERTASSISTANT_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QDebug>
#include <QHostAddress>
#include <QTimer>

class TcpServerAssistant : public QThread
{
    Q_OBJECT
public:
    TcpServerAssistant(QObject *parent = Q_NULLPTR);
    ~TcpServerAssistant();
    /// -------------------------------------------------
    QString ErrorString(){return mErrorString;}
public slots:
    /// 关闭串口
    void Close();
    /// 打开串口
    void Open(QString serverAddress, QString serverPort);
    /// 读取串口数据
    QByteArray ReadAll();
    /// 向串口发送数据,data位需要发送的数据
    qint64 Write(QByteArray data);
    /// 更新当前客户单
    void UpdateCurrentTcpClient(QHostAddress address, quint16 port);
protected:
    void run();
private:
    /// 该定时器定时检查客户端是否断开连接
    QTimer *mpCheckStateTimer;

    QList <QTcpSocket *> mTcpClientList;
    QTcpSocket *mpCurrentTcpClient;
    /// 错误信息描述
    QString mErrorString = "No error!";
    /// 套接字
    QTcpServer *mpTcpServer;
    /// 输出信息，color为输出文本要显示的颜色，prefix决定显示信息是是否显示日期时间前缀.
    void OutputInfo(QString info, QString color = "black", bool prefix = true){emit Need2OutputInfo(info, color, prefix);}
private slots:
    void CheckStateTimerTimeout();
    void Disconnect();
    /// 新连接
    void NewConnect();
signals:
    /// 更新客户端列表
    void UpdateTcpClientList(QList <QTcpSocket *> list);
    /// tcp套接字关闭成功
    void TcpServerCloseSuccessfully();
    /// tcp套接字打开成功后发射该信号
    void TcpServerOpenSuccessfully();
    /// 读到的数据后发射该信息
    void NewDataHadBeenRead(QByteArray data);
    /// 发送一帧数据后，发射该信号，data为已发送的数据
    void NewDataHadBeenWrite(QByteArray data);
    /// 外部程序关联该信号，可以获取需要输出的信息
    void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};

Q_DECLARE_METATYPE(QList <QTcpSocket *>)

#endif
