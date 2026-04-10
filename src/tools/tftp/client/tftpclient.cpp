/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpclient.h"
#include "ui_tftpclient.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "common/x.h"
#include "tools/tftp/common/tftpcommon.h"

TftpClient::TftpClient(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TftpClient)
    , m_socket(new QUdpSocket(this))
    , m_transferMode(TransferMode::Idle)
    , m_serverPort(Tftp::kDefaultPort)
    , m_serverTid(0)
    , m_currentBlock(0)
    , m_pendingBlock(0)
    , m_totalBytes(0)
    , m_bytesTransferred(0)
    , m_retryCount(0)
    , m_waitingForFinalAck(false)
{
    ui->setupUi(this);
    setupUiState();

    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &TftpClient::browseUploadFile);
    connect(ui->pushButtonUpload, &QPushButton::clicked, this, &TftpClient::onUploadClicked);
    connect(ui->pushButtonDownload, &QPushButton::clicked, this, &TftpClient::onDownloadClicked);
    connect(ui->lineEditDownloadFile,
            &QLineEdit::textChanged,
            this,
            &TftpClient::onDownloadFileChanged);
    connect(m_socket, &QUdpSocket::readyRead, this, &TftpClient::onReadyRead);
    connect(&m_timeoutTimer, &QTimer::timeout, this, &TftpClient::onTimeout);
}

TftpClient::~TftpClient()
{
    delete ui;
}

void TftpClient::setupUiState()
{
    ui->comboBoxServerAddress->setEditable(true);
    xSetupSocketAddress(ui->comboBoxServerAddress);
    if (ui->comboBoxServerAddress->findText(QStringLiteral("127.0.0.1")) < 0) {
        ui->comboBoxServerAddress->addItem(QStringLiteral("127.0.0.1"));
    }
    ui->comboBoxServerAddress->setCurrentText(QStringLiteral("127.0.0.1"));

    ui->progressBarUpload->setValue(0);
    ui->progressBarDownload->setValue(0);
    ui->progressBarUpload->setFormat(tr("Idle"));
    ui->progressBarDownload->setFormat(tr("Idle"));

    m_timeoutTimer.setInterval(Tftp::kTimeoutMs);
}

void TftpClient::browseUploadFile()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Select upload file"));
    if (!fileName.isEmpty()) {
        ui->lineEditUploadFile->setText(fileName);
    }
}

void TftpClient::onUploadClicked()
{
    startUpload();
}

void TftpClient::onDownloadClicked()
{
    startDownload();
}

void TftpClient::onDownloadFileChanged(const QString& text)
{
    if (!ui->lineEditDownloadSaveFile->text().trimmed().isEmpty()) {
        return;
    }

    const QFileInfo remoteInfo(Tftp::sanitizeRelativePath(text));
    if (!remoteInfo.fileName().isEmpty()) {
        ui->lineEditDownloadSaveFile->setText(remoteInfo.fileName());
    }
}

bool TftpClient::ensureSocket()
{
    if (m_socket->state() == QAbstractSocket::BoundState) {
        return true;
    }

    if (!m_socket->bind(QHostAddress::AnyIPv4,
                        0,
                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        QMessageBox::warning(this,
                             tr("TFTP Client"),
                             tr("Failed to bind UDP socket: %1").arg(m_socket->errorString()));
        return false;
    }

    return true;
}

QString TftpClient::selectedServerAddress() const
{
    QString address = ui->comboBoxServerAddress->currentText().trimmed();
    if (address.isEmpty()) {
        address = ui->comboBoxServerAddress->currentData().toString().trimmed();
    }

    return address;
}

void TftpClient::beginTransfer(TransferMode mode)
{
    resetTransfer();
    m_transferMode = mode;
    m_serverTid = 0;
    m_retryCount = 0;
    m_lastPacket.clear();
    m_pendingPayload.clear();
    m_pendingBlock = 0;
    m_currentBlock = 0;
    m_waitingForFinalAck = false;
    updateProgress();
}

void TftpClient::startUpload()
{
    if (m_transferMode != TransferMode::Idle) {
        QMessageBox::information(this, tr("TFTP Client"), tr("A transfer is already in progress."));
        return;
    }

    const QString addressText = selectedServerAddress();
    const QHostAddress address(addressText);
    if (address.isNull()) {
        QMessageBox::warning(this, tr("TFTP Client"), tr("Invalid server address."));
        return;
    }

    const QString uploadFilePath = ui->lineEditUploadFile->text().trimmed();
    if (uploadFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("TFTP Client"), tr("Please select a file to upload."));
        return;
    }

    m_file.setFileName(uploadFilePath);
    if (!m_file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             tr("TFTP Client"),
                             tr("Failed to open file: %1").arg(m_file.errorString()));
        return;
    }

    if (!ensureSocket()) {
        m_file.close();
        return;
    }

    m_serverAddress = address;
    m_currentRemoteFile = QFileInfo(uploadFilePath).fileName();
    beginTransfer(TransferMode::Upload);
    m_totalBytes = m_file.size();
    setProgressIndeterminate(true, false);
    updateProgress();

    QMap<QString, QString> options;
    options.insert(QStringLiteral("tsize"), QString::number(m_totalBytes));
    sendPacket(Tftp::makeRequest(Tftp::Opcode::WriteRequest, m_currentRemoteFile, options),
               Tftp::kDefaultPort);
}

QString TftpClient::defaultDownloadPath() const
{
    const QFileInfo remoteInfo(Tftp::sanitizeRelativePath(ui->lineEditDownloadFile->text()));
    return remoteInfo.fileName();
}

bool TftpClient::ensureDownloadPath()
{
    QString localPath = ui->lineEditDownloadSaveFile->text().trimmed();
    if (localPath.isEmpty()) {
        localPath = QFileDialog::getSaveFileName(this,
                                                 tr("Select download destination"),
                                                 defaultDownloadPath());
        if (localPath.isEmpty()) {
            return false;
        }
        ui->lineEditDownloadSaveFile->setText(localPath);
    }

    QFileInfo fileInfo(localPath);
    QDir parentDir = fileInfo.dir();
    if (!parentDir.exists() && !parentDir.mkpath(QStringLiteral("."))) {
        QMessageBox::warning(this, tr("TFTP Client"), tr("Failed to create destination directory."));
        return false;
    }

    return true;
}

void TftpClient::startDownload()
{
    if (m_transferMode != TransferMode::Idle) {
        QMessageBox::information(this, tr("TFTP Client"), tr("A transfer is already in progress."));
        return;
    }

    const QString addressText = selectedServerAddress();
    const QHostAddress address(addressText);
    if (address.isNull()) {
        QMessageBox::warning(this, tr("TFTP Client"), tr("Invalid server address."));
        return;
    }

    m_currentRemoteFile = Tftp::sanitizeRelativePath(ui->lineEditDownloadFile->text());
    if (m_currentRemoteFile.isEmpty()) {
        QMessageBox::warning(this, tr("TFTP Client"), tr("Please provide the remote file path."));
        return;
    }

    if (!ensureDownloadPath()) {
        return;
    }

    m_file.setFileName(ui->lineEditDownloadSaveFile->text().trimmed());
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this,
                             tr("TFTP Client"),
                             tr("Failed to open destination file: %1").arg(m_file.errorString()));
        return;
    }

    if (!ensureSocket()) {
        m_file.close();
        return;
    }

    m_serverAddress = address;
    beginTransfer(TransferMode::Download);
    m_totalBytes = -1;
    setProgressIndeterminate(false, true);
    updateProgress();

    QMap<QString, QString> options;
    options.insert(QStringLiteral("tsize"), QStringLiteral("0"));
    sendPacket(Tftp::makeRequest(Tftp::Opcode::ReadRequest, m_currentRemoteFile, options),
               Tftp::kDefaultPort);
}

void TftpClient::sendPacket(const QByteArray& packet, quint16 port)
{
    m_lastPacket = packet;
    m_socket->writeDatagram(packet, m_serverAddress, port);
    m_timeoutTimer.start();
}

void TftpClient::sendUploadBlock()
{
    m_pendingPayload = m_file.read(Tftp::kDefaultBlockSize);
    ++m_currentBlock;
    m_pendingBlock = m_currentBlock;
    m_waitingForFinalAck = m_pendingPayload.size() < Tftp::kDefaultBlockSize;
    sendPacket(Tftp::makeData(m_pendingBlock, m_pendingPayload), m_serverTid);
    ui->progressBarUpload->setFormat(tr("Uploading %1/%2 bytes")
                                         .arg(m_bytesTransferred + m_pendingPayload.size())
                                         .arg(m_totalBytes));
}

void TftpClient::updateProgress()
{
    if (m_transferMode == TransferMode::Upload) {
        if (m_totalBytes > 0) {
            const int percent = qBound(0,
                                       static_cast<int>((m_bytesTransferred * 100) / m_totalBytes),
                                       100);
            ui->progressBarUpload->setValue(percent);
            ui->progressBarUpload->setFormat(
                tr("%1/%2 bytes (%3%)").arg(m_bytesTransferred).arg(m_totalBytes).arg(percent));
        } else if (m_transferMode == TransferMode::Idle) {
            ui->progressBarUpload->setValue(0);
            ui->progressBarUpload->setFormat(tr("Idle"));
        }
    }

    if (m_transferMode == TransferMode::Download) {
        if (m_totalBytes > 0) {
            const int percent = qBound(0,
                                       static_cast<int>((m_bytesTransferred * 100) / m_totalBytes),
                                       100);
            ui->progressBarDownload->setValue(percent);
            ui->progressBarDownload->setFormat(
                tr("%1/%2 bytes (%3%)").arg(m_bytesTransferred).arg(m_totalBytes).arg(percent));
        } else if (m_totalBytes < 0) {
            ui->progressBarDownload->setFormat(tr("%1 bytes").arg(m_bytesTransferred));
        } else if (m_transferMode == TransferMode::Idle) {
            ui->progressBarDownload->setValue(0);
            ui->progressBarDownload->setFormat(tr("Idle"));
        }
    }

    if (m_transferMode == TransferMode::Idle) {
        if (ui->progressBarUpload->maximum() != 100) {
            ui->progressBarUpload->setRange(0, 100);
        }
        if (ui->progressBarDownload->maximum() != 100) {
            ui->progressBarDownload->setRange(0, 100);
        }
        if (ui->progressBarUpload->value() == 0) {
            ui->progressBarUpload->setFormat(tr("Idle"));
        }
        if (ui->progressBarDownload->value() == 0) {
            ui->progressBarDownload->setFormat(tr("Idle"));
        }
    }
}

void TftpClient::setProgressIndeterminate(bool upload, bool indeterminate)
{
    QProgressBar* progressBar = upload ? ui->progressBarUpload : ui->progressBarDownload;
    if (indeterminate) {
        progressBar->setRange(0, 0);
    } else {
        progressBar->setRange(0, 100);
    }
}

void TftpClient::finishTransfer(bool success, const QString& message)
{
    m_timeoutTimer.stop();
    const TransferMode completedMode = m_transferMode;
    if (m_file.isOpen()) {
        m_file.close();
    }

    if (completedMode == TransferMode::Upload) {
        setProgressIndeterminate(true, false);
        ui->progressBarUpload->setValue(success ? 100 : ui->progressBarUpload->value());
        ui->progressBarUpload->setFormat(message);
    } else if (completedMode == TransferMode::Download) {
        setProgressIndeterminate(false, false);
        ui->progressBarDownload->setValue(success ? 100 : ui->progressBarDownload->value());
        ui->progressBarDownload->setFormat(message);
    }

    resetTransfer();
}

void TftpClient::abortTransfer(const QString& message)
{
    const TransferMode failedMode = m_transferMode;
    const QString failedFilePath = m_file.fileName();
    finishTransfer(false, message);
    if (failedMode == TransferMode::Download && !failedFilePath.isEmpty()) {
        QFile::remove(failedFilePath);
    }
    QMessageBox::warning(this, tr("TFTP Client"), message);
}

void TftpClient::resetTransfer()
{
    m_transferMode = TransferMode::Idle;
    m_serverTid = 0;
    m_currentBlock = 0;
    m_pendingBlock = 0;
    m_pendingPayload.clear();
    m_lastPacket.clear();
    m_totalBytes = 0;
    m_bytesTransferred = 0;
    m_retryCount = 0;
    m_waitingForFinalAck = false;
}

void TftpClient::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_socket->pendingDatagramSize()));
        QHostAddress senderAddress;
        quint16 senderPort = 0;
        m_socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        handleIncomingDatagram(datagram, senderAddress, senderPort);
    }
}

void TftpClient::handleIncomingDatagram(const QByteArray& datagram,
                                        const QHostAddress& senderAddress,
                                        quint16 senderPort)
{
    if (m_transferMode == TransferMode::Idle) {
        return;
    }

    if (senderAddress != m_serverAddress) {
        return;
    }

    if (m_serverTid != 0 && senderPort != m_serverTid) {
        m_socket->writeDatagram(Tftp::makeError(Tftp::ErrorCode::UnknownTransferId,
                                                tr("Unexpected transfer id")),
                                senderAddress,
                                senderPort);
        return;
    }

    Tftp::Packet packet;
    if (!Tftp::parsePacket(datagram, &packet)) {
        abortTransfer(tr("Received an invalid TFTP packet."));
        return;
    }

    if (m_serverTid == 0) {
        m_serverTid = senderPort;
    }

    m_retryCount = 0;
    m_timeoutTimer.start();

    if (packet.opcode == Tftp::Opcode::Error) {
        abortTransfer(packet.errorMessage.isEmpty() ? tr("Remote server reported an error.")
                                                    : packet.errorMessage);
        return;
    }

    if (m_transferMode == TransferMode::Upload) {
        if (packet.opcode == Tftp::Opcode::OptionAck) {
            if (m_currentBlock == 0) {
                sendUploadBlock();
            } else {
                sendPacket(m_lastPacket, m_serverTid);
            }
            return;
        }

        if (packet.opcode != Tftp::Opcode::Ack) {
            abortTransfer(tr("Unexpected response while uploading."));
            return;
        }

        if (packet.block == 0 && m_currentBlock == 0) {
            sendUploadBlock();
            return;
        }

        if (packet.block == m_pendingBlock) {
            m_bytesTransferred += m_pendingPayload.size();
            updateProgress();
            if (m_waitingForFinalAck) {
                finishTransfer(true, tr("Upload completed"));
            } else {
                sendUploadBlock();
            }
            return;
        }

        if (packet.block == static_cast<quint16>(m_pendingBlock - 1)) {
            sendPacket(m_lastPacket, m_serverTid);
            return;
        }

        abortTransfer(tr("Unexpected ACK block received."));
        return;
    }

    if (packet.opcode == Tftp::Opcode::OptionAck) {
        bool ok = false;
        const qint64 remoteSize = packet.options.value(QStringLiteral("tsize")).toLongLong(&ok);
        if (ok) {
            m_totalBytes = remoteSize;
            setProgressIndeterminate(false, false);
        }
        updateProgress();
        sendPacket(Tftp::makeAck(0), m_serverTid);
        return;
    }

    if (packet.opcode != Tftp::Opcode::Data) {
        abortTransfer(tr("Unexpected response while downloading."));
        return;
    }

    if (packet.block == static_cast<quint16>(m_currentBlock + 1)) {
        if (m_file.write(packet.payload) != packet.payload.size()) {
            abortTransfer(tr("Failed to write downloaded data to file."));
            return;
        }

        m_currentBlock = packet.block;
        m_bytesTransferred += packet.payload.size();
        updateProgress();
        sendPacket(Tftp::makeAck(packet.block), m_serverTid);
        if (packet.payload.size() < Tftp::kDefaultBlockSize) {
            finishTransfer(true, tr("Download completed"));
        }
        return;
    }

    if (packet.block == m_currentBlock) {
        sendPacket(Tftp::makeAck(packet.block), m_serverTid);
        return;
    }

    abortTransfer(tr("Unexpected DATA block received."));
}

void TftpClient::onTimeout()
{
    if (m_transferMode == TransferMode::Idle) {
        return;
    }

    ++m_retryCount;
    if (m_retryCount > Tftp::kMaxRetries) {
        abortTransfer(tr("Transfer timed out."));
        return;
    }

    const quint16 targetPort = m_serverTid == 0 ? Tftp::kDefaultPort : m_serverTid;
    m_socket->writeDatagram(m_lastPacket, m_serverAddress, targetPort);
}