#pragma once

#include <QMap>
#include <QObject>
#include <QUdpSocket>

class TftpServerSession;

class TftpServerService : public QObject
{
    Q_OBJECT
public:
    explicit TftpServerService(QObject* parent = Q_NULLPTR);
    ~TftpServerService();

    bool start(const QHostAddress& address,
               const QString& rootDirectory,
               QString* errorMessage = Q_NULLPTR);
    void stop();
    bool isRunning() const;

signals:
    void logMessage(const QString& message);
    void sessionCreated(const QString& sessionId,
                        const QString& peer,
                        const QString& direction,
                        const QString& fileName,
                        qint64 totalBytes,
                        const QString& status);
    void sessionUpdated(const QString& sessionId,
                        qint64 transferredBytes,
                        qint64 totalBytes,
                        const QString& status);
    void sessionFinished(const QString& sessionId, const QString& status);

private:
    QUdpSocket* m_listener;
    QString m_rootDirectory;
    QMap<QString, TftpServerSession*> m_sessions;

private:
    QString resolveFilePath(const QString& relativePath) const;
    void sendError(const QHostAddress& address,
                   quint16 port,
                   quint16 errorCode,
                   const QString& message);

private slots:
    void onReadyRead();
    void onSessionProgress(const QString& sessionId,
                           qint64 transferredBytes,
                           qint64 totalBytes,
                           const QString& status);
    void onSessionFinished(const QString& sessionId, bool success, const QString& message);
    void onSessionLog(const QString& message);
};