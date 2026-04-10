#pragma once

#include <QFile>
#include <QHostAddress>
#include <QMap>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class TftpServerSession : public QObject
{
    Q_OBJECT
public:
    enum class Direction { Send, Receive };

    explicit TftpServerSession(Direction direction,
                               const QString& sessionId,
                               const QHostAddress& localAddress,
                               const QHostAddress& peerAddress,
                               quint16 peerPort,
                               const QString& absoluteFilePath,
                               const QString& relativeFilePath,
                               qint64 totalBytes,
                               const QMap<QString, QString>& requestOptions,
                               QObject* parent = Q_NULLPTR);
    ~TftpServerSession();

    bool start(QString* errorMessage = Q_NULLPTR);
    QString id() const;
    QString peer() const;
    QString directionLabel() const;
    QString fileName() const;
    qint64 totalBytes() const;

signals:
    void progress(const QString& sessionId,
                  qint64 transferredBytes,
                  qint64 totalBytes,
                  const QString& status);
    void finished(const QString& sessionId, bool success, const QString& message);
    void logMessage(const QString& message);

private:
    Direction m_direction;
    QString m_sessionId;
    QHostAddress m_localAddress;
    QHostAddress m_peerAddress;
    quint16 m_peerPort;
    QString m_absoluteFilePath;
    QString m_relativeFilePath;
    qint64 m_totalBytes;
    qint64 m_transferredBytes;
    QMap<QString, QString> m_requestOptions;
    QUdpSocket* m_socket;
    QFile m_file;
    QTimer m_timeoutTimer;
    quint16 m_currentBlock;
    QByteArray m_lastPacket;
    QByteArray m_lastPayload;
    bool m_lastBlockSent;
    int m_retryCount;

private:
    void sendPacket(const QByteArray& packet);
    void sendInitialResponse();
    void sendNextDataBlock();
    void sendAck(quint16 block);
    void fail(quint16 errorCode, const QString& message);
    void complete(const QString& message);
    void updateProgress(const QString& status);

private slots:
    void onReadyRead();
    void onTimeout();
};