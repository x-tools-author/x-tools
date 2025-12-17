/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QFile>
#include <QHostAddress>
#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include <QUdpSocket>

#include "common/xdq.h"

// TFTP操作码
enum TftpOpcode {
    RRQ = 1,   // read request
    WRQ = 2,   // write request
    DATA = 3,  // data packet
    ACK = 4,   // acknowledgement
    ERROR = 5, // error code
    OACK = 6   // option acknowledge
};

// TFTP错误码
enum TftpErrorCode {
    UNDEFINED_ERROR = 0,     /* not defined */
    FILE_NOT_FOUND = 1,      /* file not found */
    ACCESS_VIOLATION = 2,    /* access violation */
    DISK_FULL = 3,           /* disk full or allocation exceeded */
    ILLEGAL_OPERATION = 4,   /* illegal TFTP operation */
    UNKNOWN_TRANSFER_ID = 5, /* unknown transfer ID */
    FILE_ALREADY_EXISTS = 6, /* file already exists */
    NO_SUCH_USER = 7         /* no such user */
};

// TFTP常量
constexpr quint16 DEFAULT_TFTP_PORT = 69;
constexpr quint16 DEFAULT_BLOCK_SIZE = 512;
constexpr quint32 MAX_BUFFER_SIZE = 65536;
constexpr int MAX_RETRY_COUNT = 3;
constexpr int TIMEOUT_INTERVAL = 1000;

// TFTP传输模式
constexpr const char *TFTP_MODE_OCTET = "octet";

// TFTP选项键名
constexpr const char *TFTP_OPTION_BLOCK_SIZE = "blksize";
constexpr const char *TFTP_OPTION_TIMEOUT = "timeout";
constexpr const char *TFTP_OPTION_TRANSFER_SIZE = "tsize";

// TFTP会话状态
enum class TftpSessionState {
    Idle,
    ReadRequestSent,
    WriteRequestSent,
    Transferring,
    Completed,
    Error
};

class TftpBasePrivate;
class TftpBase : public QObject
{
    Q_OBJECT
    X_DECLARE_PRIVATE(TftpBase)

public:
    explicit TftpBase(QObject *parent = nullptr);
    ~TftpBase() override;

protected:
    // 获取UDP socket
    QUdpSocket *udpSocket() const;

    // 发送TFTP数据包
    void sendDatagram(const QByteArray &data, const QHostAddress &address, quint16 port);

    // 发送错误码
    void sendErrorCode(TftpErrorCode code,
                       const QString &message,
                       const QHostAddress &address,
                       quint16 port);

    // 解析TFTP选项
    QMap<QString, QString> parseTftpOptions(const char *data, quint32 dataLength);

    // 创建TFTP选项数据
    QByteArray createTftpOptions(const QMap<QString, QString> &options);

    // 计算文件MD5
    QString calculateFileMd5(const QString &filePath);

    // 生成唯一会话ID
    QString generateSessionId();

protected slots:
    // 处理接收到的数据
    virtual void processPendingDatagrams() = 0;

    // 处理超时
    virtual void handleTimeout() = 0;

signals:
    // 错误信号
    void errorOccurred(const QString &errorMessage);
};

class TftpClientPrivate;
class TftpClient : public TftpBase
{
    Q_OBJECT
    X_DECLARE_PRIVATE(TftpClient)

public:
    explicit TftpClient(QObject *parent = nullptr);
    ~TftpClient() override;

    // 设置服务器地址和端口
    void setServerAddress(const QHostAddress &address, quint16 port = DEFAULT_TFTP_PORT);

    // 设置本地工作目录
    void setLocalWorkPath(const QString &path);

    // 下载文件
    bool downloadFile(const QString &remoteFileName, const QString &localFileName = "");

    // 上传文件
    bool uploadFile(const QString &localFileName, const QString &remoteFileName = "");

    // 停止传输
    void stop();

    // 检查是否正在传输
    bool isTransferring() const;

protected slots:
    // 处理接收到的数据
    void processPendingDatagrams() override;

    // 处理超时
    void handleTimeout() override;

signals:
    // 传输进度信号
    void progressChanged(const QString &fileName, qint64 transferred, qint64 total, int speed);

    // 传输完成信号
    void transferCompleted(const QString &fileName);

    // 错误信号
    void errorOccurred(const QString &errorMessage);
};

class TftpSessionPrivate;
class TftpSession : public TftpBase
{
    Q_OBJECT
    X_DECLARE_PRIVATE(TftpSession)

public:
    explicit TftpSession(const QHostAddress &clientAddress,
                         quint16 clientPort,
                         QObject *parent = nullptr);
    ~TftpSession() override;

    // 获取客户端地址和端口
    QHostAddress clientAddress() const;
    quint16 clientPort() const;

    // 获取会话ID
    QString sessionId() const;

    // 处理请求
    void handleRequest(const QByteArray &data);

    // 检查会话是否处于活跃状态
    bool isActive() const;

    // 设置根目录
    void setRootPath(const QString &path);

protected slots:
    // 处理接收到的数据
    void processPendingDatagrams() override;

    // 处理超时
    void handleTimeout() override;

signals:
    // 传输进度信号
    void progressChanged(const QString &fileName, qint64 transferred, qint64 total);

    // 传输完成信号
    void transferCompleted(const QString &fileName, bool isUpload);

    // 会话结束信号
    void sessionFinished(const QString &sessionId);

    // 错误信号
    void errorOccurred(const QString &errorMessage);
};

class TftpServerPrivate;
class TftpServer : public TftpBase
{
    Q_OBJECT
    X_DECLARE_PRIVATE(TftpServer)

public:
    explicit TftpServer(QObject *parent = nullptr);
    ~TftpServer() override;

    // 启动服务器
    bool start(quint16 port = DEFAULT_TFTP_PORT);

    // 停止服务器
    void stop();

    // 检查服务器是否正在运行
    bool isRunning() const;

    // 设置服务器根目录
    void setRootPath(const QString &path);

    // 获取当前端口
    quint16 port() const;

protected slots:
    // 处理接收到的数据
    void processPendingDatagrams() override;

    // 处理超时
    void handleTimeout() override;

    // 处理会话结束
    void onSessionFinished(const QString &sessionId);

signals:
    // 客户端连接信号
    void clientConnected(const QHostAddress &clientAddress, quint16 clientPort);

    // 客户端断开连接信号
    void clientDisconnected(const QHostAddress &clientAddress, quint16 clientPort);

    // 文件传输进度信号
    void transferProgress(const QString &fileName,
                          const QHostAddress &clientAddress,
                          qint64 transferred,
                          qint64 total);

    // 文件传输完成信号
    void transferCompleted(const QString &fileName,
                           const QHostAddress &clientAddress,
                           bool isUpload);

    // 错误信号
    void errorOccurred(const QString &errorMessage);
};