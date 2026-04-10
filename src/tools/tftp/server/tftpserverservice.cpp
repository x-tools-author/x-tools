#include "tftpserverservice.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>

#include "tftpserversession.h"
#include "tools/tftp/common/tftpcommon.h"

TftpServerService::TftpServerService(QObject* parent)
    : QObject(parent)
    , m_listener(new QUdpSocket(this))
{}

TftpServerService::~TftpServerService()
{
    stop();
}

bool TftpServerService::start(const QHostAddress& address,
                              const QString& rootDirectory,
                              QString* errorMessage)
{
    stop();
    m_rootDirectory = QDir(rootDirectory).absolutePath();

    if (!m_listener->bind(address,
                          Tftp::kDefaultPort,
                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        if (errorMessage) {
            *errorMessage = tr("Failed to bind TFTP server socket: %1")
                                .arg(m_listener->errorString());
        }
        return false;
    }

    connect(m_listener,
            &QUdpSocket::readyRead,
            this,
            &TftpServerService::onReadyRead,
            Qt::UniqueConnection);
    emit logMessage(tr("Listening on %1:%2").arg(address.toString()).arg(Tftp::kDefaultPort));
    return true;
}

void TftpServerService::stop()
{
    if (m_listener->isOpen()) {
        m_listener->close();
    }

    qDeleteAll(m_sessions);
    m_sessions.clear();
}

bool TftpServerService::isRunning() const
{
    return m_listener->isOpen();
}

QString TftpServerService::resolveFilePath(const QString& relativePath) const
{
    const QString safeRelativePath = Tftp::sanitizeRelativePath(relativePath);
    if (safeRelativePath.isEmpty()) {
        return QString();
    }

    const QString rootPath = QDir::cleanPath(
        QDir::fromNativeSeparators(QDir(m_rootDirectory).absolutePath()));
    const QString absolutePath = QDir::cleanPath(
        QDir::fromNativeSeparators(QDir(m_rootDirectory).absoluteFilePath(safeRelativePath)));

    const QString rootLower = rootPath.toLower();
    const QString absoluteLower = absolutePath.toLower();
    if (absoluteLower != rootLower && !absoluteLower.startsWith(rootLower + QLatin1Char('/'))) {
        return QString();
    }

    return absolutePath;
}

void TftpServerService::sendError(const QHostAddress& address,
                                  quint16 port,
                                  quint16 errorCode,
                                  const QString& message)
{
    m_listener->writeDatagram(Tftp::makeError(static_cast<Tftp::ErrorCode>(errorCode), message),
                              address,
                              port);
    emit logMessage(tr("%1 -> error: %2").arg(Tftp::endpointString(address, port), message));
}

void TftpServerService::onReadyRead()
{
    while (m_listener->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_listener->pendingDatagramSize()));
        QHostAddress peerAddress;
        quint16 peerPort = 0;
        m_listener->readDatagram(datagram.data(), datagram.size(), &peerAddress, &peerPort);

        Tftp::Packet packet;
        if (!Tftp::parsePacket(datagram, &packet)) {
            sendError(peerAddress,
                      peerPort,
                      static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                      tr("Invalid TFTP packet"));
            continue;
        }

        if (packet.opcode != Tftp::Opcode::ReadRequest
            && packet.opcode != Tftp::Opcode::WriteRequest) {
            sendError(peerAddress,
                      peerPort,
                      static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                      tr("Only RRQ and WRQ are accepted on the listener socket"));
            continue;
        }

        if (packet.mode != QStringLiteral("octet")) {
            sendError(peerAddress,
                      peerPort,
                      static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                      tr("Only octet mode is supported"));
            continue;
        }

        const QString safeRelativePath = Tftp::sanitizeRelativePath(packet.fileName);
        const QString absoluteFilePath = resolveFilePath(safeRelativePath);
        if (safeRelativePath.isEmpty() || absoluteFilePath.isEmpty()) {
            sendError(peerAddress,
                      peerPort,
                      static_cast<quint16>(Tftp::ErrorCode::AccessViolation),
                      tr("Invalid file path"));
            continue;
        }

        qint64 totalBytes = -1;
        if (packet.opcode == Tftp::Opcode::ReadRequest) {
            QFileInfo fileInfo(absoluteFilePath);
            if (!fileInfo.exists() || !fileInfo.isFile()) {
                sendError(peerAddress,
                          peerPort,
                          static_cast<quint16>(Tftp::ErrorCode::FileNotFound),
                          tr("Requested file does not exist"));
                continue;
            }
            totalBytes = fileInfo.size();
        } else {
            bool ok = false;
            const qint64 requestedSize = packet.options.value(QStringLiteral("tsize"))
                                             .toLongLong(&ok);
            if (ok) {
                totalBytes = requestedSize;
            }
            QDir().mkpath(QFileInfo(absoluteFilePath).absolutePath());
        }

        const QString sessionId = QStringLiteral("%1-%2-%3")
                                      .arg(peerAddress.toString())
                                      .arg(peerPort)
                                      .arg(QDateTime::currentMSecsSinceEpoch());
        TftpServerSession* session
            = new TftpServerSession(packet.opcode == Tftp::Opcode::ReadRequest
                                        ? TftpServerSession::Direction::Send
                                        : TftpServerSession::Direction::Receive,
                                    sessionId,
                                    m_listener->localAddress(),
                                    peerAddress,
                                    peerPort,
                                    absoluteFilePath,
                                    safeRelativePath,
                                    totalBytes,
                                    packet.options,
                                    this);

        connect(session, &TftpServerSession::progress, this, &TftpServerService::onSessionProgress);
        connect(session, &TftpServerSession::finished, this, &TftpServerService::onSessionFinished);
        connect(session, &TftpServerSession::logMessage, this, &TftpServerService::onSessionLog);

        QString errorMessage;
        if (!session->start(&errorMessage)) {
            sendError(peerAddress,
                      peerPort,
                      static_cast<quint16>(Tftp::ErrorCode::Undefined),
                      errorMessage.isEmpty() ? tr("Failed to start session") : errorMessage);
            session->deleteLater();
            continue;
        }

        m_sessions.insert(sessionId, session);
        emit sessionCreated(sessionId,
                            session->peer(),
                            session->directionLabel(),
                            session->fileName(),
                            session->totalBytes(),
                            tr("Running"));
    }
}

void TftpServerService::onSessionProgress(const QString& sessionId,
                                          qint64 transferredBytes,
                                          qint64 totalBytes,
                                          const QString& status)
{
    emit sessionUpdated(sessionId, transferredBytes, totalBytes, status);
}

void TftpServerService::onSessionFinished(const QString& sessionId,
                                          bool success,
                                          const QString& message)
{
    TftpServerSession* session = m_sessions.take(sessionId);
    if (!session) {
        return;
    }

    emit sessionFinished(sessionId, success ? tr("Completed") : message);
    session->deleteLater();
}

void TftpServerService::onSessionLog(const QString& message)
{
    emit logMessage(message);
}