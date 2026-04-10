/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpserversession.h"

#include <QFileInfo>

#include "tools/tftp/common/tftpcommon.h"

TftpServerSession::TftpServerSession(Direction direction,
                                     const QString& sessionId,
                                     const QHostAddress& localAddress,
                                     const QHostAddress& peerAddress,
                                     quint16 peerPort,
                                     const QString& absoluteFilePath,
                                     const QString& relativeFilePath,
                                     qint64 totalBytes,
                                     const QMap<QString, QString>& requestOptions,
                                     QObject* parent)
    : QObject(parent)
    , m_direction(direction)
    , m_sessionId(sessionId)
    , m_localAddress(localAddress)
    , m_peerAddress(peerAddress)
    , m_peerPort(peerPort)
    , m_absoluteFilePath(absoluteFilePath)
    , m_relativeFilePath(relativeFilePath)
    , m_totalBytes(totalBytes)
    , m_transferredBytes(0)
    , m_requestOptions(requestOptions)
    , m_socket(new QUdpSocket(this))
    , m_currentBlock(0)
    , m_lastBlockSent(false)
    , m_retryCount(0)
{
    m_timeoutTimer.setInterval(Tftp::kTimeoutMs);
    connect(m_socket, &QUdpSocket::readyRead, this, &TftpServerSession::onReadyRead);
    connect(&m_timeoutTimer, &QTimer::timeout, this, &TftpServerSession::onTimeout);
}

TftpServerSession::~TftpServerSession() {}

bool TftpServerSession::start(QString* errorMessage)
{
    const QIODevice::OpenMode openMode = m_direction == Direction::Send
                                             ? QIODevice::ReadOnly
                                             : (QIODevice::WriteOnly | QIODevice::Truncate);
    m_file.setFileName(m_absoluteFilePath);
    if (!m_file.open(openMode)) {
        if (errorMessage) {
            *errorMessage = tr("Failed to open file: %1").arg(m_file.errorString());
        }
        return false;
    }

    const QHostAddress bindAddress = m_localAddress.isNull()
                                             || m_localAddress == QHostAddress::AnyIPv4
                                         ? QHostAddress::AnyIPv4
                                         : m_localAddress;
    if (!m_socket->bind(bindAddress, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        if (errorMessage) {
            *errorMessage = tr("Failed to bind session socket: %1").arg(m_socket->errorString());
        }
        return false;
    }

    if (m_direction == Direction::Send && m_totalBytes < 0) {
        m_totalBytes = m_file.size();
    }

    sendInitialResponse();
    emit logMessage(tr("%1 %2 %3")
                        .arg(directionLabel(),
                             Tftp::endpointString(m_peerAddress, m_peerPort),
                             m_relativeFilePath));
    return true;
}

QString TftpServerSession::id() const
{
    return m_sessionId;
}

QString TftpServerSession::peer() const
{
    return Tftp::endpointString(m_peerAddress, m_peerPort);
}

QString TftpServerSession::directionLabel() const
{
    return m_direction == Direction::Send ? tr("Send") : tr("Receive");
}

QString TftpServerSession::fileName() const
{
    return m_relativeFilePath;
}

qint64 TftpServerSession::totalBytes() const
{
    return m_totalBytes;
}

void TftpServerSession::sendPacket(const QByteArray& packet)
{
    m_lastPacket = packet;
    m_socket->writeDatagram(packet, m_peerAddress, m_peerPort);
    m_timeoutTimer.start();
}

void TftpServerSession::sendInitialResponse()
{
    QMap<QString, QString> responseOptions;
    if (m_requestOptions.contains(QStringLiteral("tsize"))) {
        responseOptions.insert(QStringLiteral("tsize"),
                               QString::number(qMax<qint64>(m_totalBytes, 0)));
    }

    if (!responseOptions.isEmpty()) {
        sendPacket(Tftp::makeOack(responseOptions));
        updateProgress(tr("Negotiating"));
        return;
    }

    if (m_direction == Direction::Send) {
        sendNextDataBlock();
    } else {
        sendAck(0);
    }
}

void TftpServerSession::sendNextDataBlock()
{
    m_lastPayload = m_file.read(Tftp::kDefaultBlockSize);
    ++m_currentBlock;
    m_lastBlockSent = m_lastPayload.size() < Tftp::kDefaultBlockSize;
    sendPacket(Tftp::makeData(m_currentBlock, m_lastPayload));
    updateProgress(tr("Sending"));
}

void TftpServerSession::sendAck(quint16 block)
{
    sendPacket(Tftp::makeAck(block));
    updateProgress(tr("Receiving"));
}

void TftpServerSession::fail(quint16 errorCode, const QString& message)
{
    m_timeoutTimer.stop();
    m_socket->writeDatagram(Tftp::makeError(static_cast<Tftp::ErrorCode>(errorCode), message),
                            m_peerAddress,
                            m_peerPort);
    emit logMessage(tr("%1 failed: %2").arg(peer(), message));
    emit progress(m_sessionId, m_transferredBytes, m_totalBytes, message);
    emit finished(m_sessionId, false, message);
}

void TftpServerSession::complete(const QString& message)
{
    m_timeoutTimer.stop();
    if (m_file.isOpen()) {
        m_file.close();
    }
    emit logMessage(tr("%1 completed: %2").arg(peer(), m_relativeFilePath));
    emit progress(m_sessionId, m_transferredBytes, m_totalBytes, message);
    emit finished(m_sessionId, true, message);
}

void TftpServerSession::updateProgress(const QString& status)
{
    emit progress(m_sessionId, m_transferredBytes, m_totalBytes, status);
}

void TftpServerSession::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_socket->pendingDatagramSize()));
        QHostAddress senderAddress;
        quint16 senderPort = 0;
        m_socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        if (senderAddress != m_peerAddress || senderPort != m_peerPort) {
            m_socket->writeDatagram(Tftp::makeError(Tftp::ErrorCode::UnknownTransferId,
                                                    tr("Unexpected transfer id")),
                                    senderAddress,
                                    senderPort);
            continue;
        }

        Tftp::Packet packet;
        if (!Tftp::parsePacket(datagram, &packet)) {
            fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                 tr("Invalid session packet"));
            return;
        }

        m_retryCount = 0;
        m_timeoutTimer.start();

        if (packet.opcode == Tftp::Opcode::Error) {
            fail(packet.errorCode,
                 packet.errorMessage.isEmpty() ? tr("Remote peer reported an error")
                                               : packet.errorMessage);
            return;
        }

        if (m_direction == Direction::Send) {
            if (packet.opcode == Tftp::Opcode::Ack && packet.block == 0 && m_currentBlock == 0) {
                sendNextDataBlock();
                return;
            }

            if (packet.opcode != Tftp::Opcode::Ack) {
                fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                     tr("Expected ACK packet"));
                return;
            }

            if (packet.block == m_currentBlock) {
                m_transferredBytes += m_lastPayload.size();
                updateProgress(tr("Sending"));
                if (m_lastBlockSent) {
                    complete(tr("Transfer completed"));
                } else {
                    sendNextDataBlock();
                }
                return;
            }

            if (packet.block == static_cast<quint16>(m_currentBlock - 1)) {
                sendPacket(m_lastPacket);
                return;
            }

            fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                 tr("Unexpected ACK block"));
            return;
        }

        if (packet.opcode == Tftp::Opcode::OptionAck) {
            fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                 tr("Unexpected OACK packet"));
            return;
        }

        if (packet.opcode != Tftp::Opcode::Data) {
            fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation),
                 tr("Expected DATA packet"));
            return;
        }

        if (packet.block == static_cast<quint16>(m_currentBlock + 1)) {
            if (m_file.write(packet.payload) != packet.payload.size()) {
                fail(static_cast<quint16>(Tftp::ErrorCode::DiskFull),
                     tr("Failed to write file data"));
                return;
            }

            m_currentBlock = packet.block;
            m_transferredBytes += packet.payload.size();
            sendAck(packet.block);
            if (packet.payload.size() < Tftp::kDefaultBlockSize) {
                complete(tr("Transfer completed"));
            }
            return;
        }

        if (packet.block == m_currentBlock) {
            sendAck(packet.block);
            return;
        }

        fail(static_cast<quint16>(Tftp::ErrorCode::IllegalOperation), tr("Unexpected DATA block"));
        return;
    }
}

void TftpServerSession::onTimeout()
{
    ++m_retryCount;
    if (m_retryCount > Tftp::kMaxRetries) {
        fail(static_cast<quint16>(Tftp::ErrorCode::Undefined), tr("Session timed out"));
        return;
    }

    m_socket->writeDatagram(m_lastPacket, m_peerAddress, m_peerPort);
}