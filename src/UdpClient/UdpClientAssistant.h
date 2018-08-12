#ifndef UDPCLIENTASSISTANT_H
#define UDPCLIENTASSISTANT_H

#include <QUdpSocket>
#include <QThread>
#include <QDebug>

class UupClientAssistant : public QThread
{
    Q_OBJECT
public:
    UupClientAssistant(QObject *parent = Q_NULLPTR);
    ~UupClientAssistant();
    /// -------------------------------------------------
    QString ErrorString(){return mErrorString;}
public slots:
    /// 关闭串口
    void Close();
    /// 打开串口
    void Open(QString hostAddress, QString port);
    /// 读取串口数据
    QByteArray ReadAll();
    /// 向串口发送数据,data位需要发送的数据
    qint64 Write(QByteArray data, QString hostAddress, QString port);
protected:
    void run();
private:
    /// 错误信息描述
    QString mErrorString = "No error!";
    /// 套接字
    QUdpSocket *mpUdpSocket;
    /// 输出信息，color为输出文本要显示的颜色，prefix决定显示信息是是否显示日期时间前缀.
    void OutputInfo(QString info, QString color = "black", bool prefix = true){emit Need2OutputInfo(info, color, prefix);}
signals:
    /// 串口关闭成功
    void UdpSocketCloseSuccessfully();
    /// 串口打开成功后发射该信号
    void UdpSocketOpenSuccessfully();
    /// 读到的数据后发射该信息
    void NewDataHadBeenRead(QByteArray data);
    /// 发送一帧数据后，发射该信号，data为已发送的数据
    void NewDataHadBeenWrite(QByteArray data);
    /// 外部程序关联该信号，可以获取需要输出的信息
    void Need2OutputInfo(QString info, QString color = "black", bool prefix = true);
};

#endif
