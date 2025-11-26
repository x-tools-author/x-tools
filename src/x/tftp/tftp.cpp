/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftp.h"

#include <fcntl.h>
#include <string.h>
#include <QCollator>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QMetaType>
#include <QMutex>
#include <QThread>
#include <QtEndian>

Tftp::Tftp(QObject *parent)
    : QThread{parent}
{
    timerout = new QTimer();
    timerout->setInterval(1000);
    timerout->setSingleShot(true);
    readBuf.clear();
    connect(timerout, &QTimer::timeout, this, [&]() {
        if (retryCount-- <= 0) {
            ////print << "\ntimeout\n";
            clearStatus();
            sendErrorCode(EBADOP);
            retryCount = 3;
        }
    });

    sendBuf = new char[BUFFER_SIZE];
    memset(sendBuf, 0, BUFFER_SIZE);
    sptr = sendBuf;
    setWorkPathSlots(QDir::currentPath());

    if (!udpSocket) {
        udpSocket = new QUdpSocket;
        udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 65536);

        connect(udpSocket, &QUdpSocket::readyRead, this, &Tftp::readPendingDatagramsSlots);
    }
}

Tftp::~Tftp()
{
    if (this->isRunning()) {
        this->exit(0);
    }
    clearStatus();
    if (udpSocket) {
        QObject::disconnect(udpSocket,
                            &QUdpSocket::readyRead,
                            this,
                            &Tftp::readPendingDatagramsSlots);
        udpSocket->deleteLater();
        udpSocket = nullptr;
    }
    if (file) {
        file->close();
        file->deleteLater();
        file = nullptr;
    }
    if (timerout) {
        timerout->stop();
        timerout->deleteLater();
        timerout = nullptr;
    }
    if (sendBuf) {
        delete[] sendBuf;
        sendBuf = nullptr;
    }
}

void Tftp::run()
{
    on_recvfrom();
}

void Tftp::setWorkPathSlots(QString path)
{
    strncpy_s(file_path, sizeof(file_path), path.toLatin1().data(), path.length());
    if (file_path[strlen(file_path) - 1] != '/') {
        file_path[strlen(file_path)] = '/';
    }
    curFilePath = QString(QByteArray(file_path, (int) strlen(file_path)));
}

void Tftp::readPendingDatagramsSlots()
{
    if (!this->isRunning())
        this->start();
    this->dataAvalivabe = true;
}
void Tftp::printProcess(int maxValue, int curValue)
{
    if (curValue <= 0 || maxValue <= 0)
        return;
    int progress = (((float) curValue / (float) maxValue)) * 100;
    if (progress != progress_bak) {
        progress_bak = progress;
        emit tftpProcessSignal(curFilePath + pre_filePath, progress);
        ////print << __FUNCTION__ << progress;
    }
}

void Tftp::setPortSlots(int port)
{
    this->port = port;
    udpSocket->disconnectFromHost();
    if (udpSocket->bind(QHostAddress::Any, this->port) == false) {
        QString msg = "bind port 69 failed";
        ////print << msg;
        emit tftpErrorSignal(msg);
    } else {
        ////print << "bind port 69 success";
        initFlag = true;
    }
}

void Tftp::on_recvfrom()
{
    while (1) {
        if (!initFlag) {
            if (udpSocket->bind(QHostAddress::Any, this->port) == false) {
                //print << "bind port 69 failed";
                QString msg = "bind port 69 failed";
                //print << msg;
                emit tftpErrorSignal(msg);
                break;
            } else {
                //print << "bind port 69 success";
                initFlag = true;
            }
        }
        //         if(this->dataAvalivabe==false)
        //             continue;
        uint16_t code, cmd = 0;
        char *ptr;

        int readLen = 0, pos = 0;
        uint16_t index = 0;
        bool ok = false;
        retryCount = 3;
        uint32_t readbytes = 0;
        uint16_t block_current_index = 0;
        while (udpSocket->hasPendingDatagrams()) {
            datagram = udpSocket->receiveDatagram();
            if (!datagram.isValid()) {
                //print << __LINE__;
                continue;
            }
            destAddress = datagram.senderAddress();
            destPort = datagram.senderPort();
            //print << __LINE__ << destAddress << destPort;
            readBuf.clear();
            readBuf = datagram.data();
            datagram.clear();
            ptr = readBuf.data();
            code = ((ptr[0] >> 8) & 0xff) | (ptr[1] & 0xff);
            pos += 2;
            //print << __LINE__ << code;

            switch (code) {
            case _RRQ:
            case _WRQ:
                pre_filePath = &ptr[pos];
                //print << "file name is " + curFilePath + pre_filePath;
                pos += pre_filePath.length() + 1;
                if (code == _WRQ) {
                    QFileInfo info(curFilePath + pre_filePath);

                    if (info.isDir()) {
                        sendErrorCode(EACCESS);
                        clearStatus();
                        break;
                    }
                    file = new QFile(curFilePath + pre_filePath);
                    file->open(QIODevice::WriteOnly | QIODevice::Truncate);
                    if (!file) {
                        sendErrorCode(ENOTFOUND);
                        clearStatus();
                        break;
                    }
                } else {
                    /*获取文件信息，把信息放到s_buf中*/
                    QFileInfo info(curFilePath + pre_filePath);

                    if (info.isDir()) {
                        sendErrorCode(ENOTFOUND);
                        clearStatus();
                        break;
                    }
                    file = new QFile(curFilePath + pre_filePath);
                    file->open(QIODevice::ReadOnly);
                    if (!file) {
                        sendErrorCode(ENOTFOUND);
                        clearStatus();
                        break;
                    }
                }

                while (1) {
                    if (pos >= readBuf.count())
                        break;
                    QString tmp = "";
                    tmp = &ptr[pos];
                    if (tmp == "octet") {
                        transportType = OCTET;
                        pos += tmp.length() + 1;
                    } else if (tmp == "tsize") {
                        pos += (int) strlen("tsize") + 1;
                        tmp = "";
                        tmp = &ptr[pos];
                        file_size = tmp.toUInt(&ok, 10);
                        if (!ok) {
                            sendErrorCode(EUNDEF);
                            if (file)
                                file->close();
                            clearStatus();
                            return;
                        }
                        totalSize = file_size;
                        char tt[256] = {0};
                        pos += sprintf(tt, "%d", file_size) + 1;
                        //print << __LINE__ << file_size;
                    } else if (tmp == "timeout") {
                        pos += (int) strlen("timeout") + 1;
                        tmp = "";
                        tmp = &ptr[pos];
                        timeout = tmp.toUInt(&ok, 10);
                        if (!ok) {
                            sendErrorCode(EUNDEF);
                            if (file)
                                file->close();
                            clearStatus();
                            return;
                        }
                        char tt[256] = {0};
                        pos += sprintf(tt, "%d", timeout) + 1;
                        //print << QString::asprintf("timeout:%d\n", timeout);
                        timerout->setInterval(timeout * 1000);
                    } else if (tmp == "blksize") {
                        pos += (int) strlen("blksize") + 1;
                        tmp = "";
                        tmp = &ptr[pos];
                        blockSize = tmp.toUInt(&ok, 10);
                        if (!ok) {
                            sendErrorCode(EUNDEF);
                            if (file)
                                file->close();
                            clearStatus();
                            return;
                        }
                        char tt[256] = {0};
                        ptr += sprintf(tt, "%d", blockSize) + 1;
                        //print << __LINE__ << blockSize;
                    } else {
                        sendErrorCode(EUNDEF);
                        if (file)
                            file->close();
                        clearStatus();
                        return;
                    }
                }
                if (!timerout) {
                    timerout->start();
                }

                cmd = qToBigEndian((uint16_t) _OACK);
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;
                memcpy(sptr, &cmd, 2);
                sptr += 2;
                memcpy(sptr, "tsize", strlen("tsize"));
                sptr += strlen("tsize") + 1;
                if (code == _RRQ) {
                    file_size = getFileSize(curFilePath + pre_filePath);
                    //print << __LINE__ << file_size;
                    totalSize = file_size;
                } else {
                }
                sptr += sprintf(sptr, "%d", file_size);
                sptr++;
                if (timeout) {
                    memcpy(sptr, "timeout", strlen("timeout"));
                    sptr += strlen("timeout") + 1;
                    sptr += sprintf(sptr, "%d", timeout);
                    sptr++;
                    //print << QString::asprintf("111timeout:%d\n", timeout);
                }
                if (blockSize) {
                    memcpy(sptr, "blksize", strlen("blksize"));
                    sptr += strlen("blksize") + 1;
                    sptr += sprintf(sptr, "%d", blockSize);
                    sptr++;
                    //print << __LINE__ << blockSize;
                } else {
                    blockSize = BLOCK_SIZE;
                    //print << __LINE__ << blockSize;
                }
                {
                    QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                    //print << __LINE__ << data;

                    udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                }
                break; // read request
            case _DATA:
                //print << "_DATA\n";
                if (!file) {
                    timerout->stop();
                    sendErrorCode(ENOTFOUND);
                    clearStatus();
                    break;
                }
                readbytes = readBuf.count() - 4;
                pos += 2;

                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;
                printProcess(totalSize, totalSize - file_size);
                code = qToBigEndian((uint16_t) _ACK);
                memcpy(sptr, &code, 2);
                sptr += 2;
                memcpy(&block_current_index, &ptr[pos - 2], 2);
                memcpy(sptr, &block_current_index, 2);

                sptr += 2;
                if (qToBigEndian(block_current_index) == blockIndex) {
                    //print << __LINE__ << qToBigEndian(block_current_index) << blockIndex;

                    blockIndex++;
                    if (readbytes >= blockSize) {
                        readLen = file->write(&ptr[pos], blockSize);
                        if (readLen < 0) {
                            sendErrorCode(ENOTFOUND);
                            clearStatus();
                            break;
                        }
                        file_size -= readLen;

                        //print << "writing" << __LINE__ << file_size << readLen << readbytes;
                    } else {
                        readLen = file->write(&ptr[pos], readbytes);
                        if (readLen < 0) {
                            sendErrorCode(ENOTFOUND);
                            clearStatus();
                            break;
                        }
                        file_size -= readLen;
                        //print << "write finished" << __LINE__ << file_size << readLen;
                        // if(file_size<=0)
                        {
                            file->close();
                            timerout->stop();
                            endtftp = 1;
                        }
                    }
                }

                {
                    QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                    //print << __LINE__;
                    //print << __LINE__ << destAddress << destPort;
                    udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                }
                if (endtftp) {
                    timerout->stop();

                    printProcess(totalSize, totalSize);
                    //print << "\ntftp done\n";
                    clearStatus();
                }
                break; // data packet
            case _ACK:
                if (endtftp) {
                    timerout->stop();

                    printProcess(totalSize, totalSize);
                    //print << "\ntftp done\n";
                    clearStatus();
                    break;
                }
                if (!file) {
                    sendErrorCode(ENOTFOUND);
                    break;
                }
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;
                if (file_size < blockSize) {
                    readLen = file->read(sptr + 4, file_size);
                    readLen = file_size;
                    endtftp = 1;
                    file->close();
                } else {
                    readLen = file->read(sptr + 4, blockSize);
                    //readLen = blockSize*readLen;
                }
                //timerout->start();
                file_size -= readLen;
                //print << __LINE__ << _DATA << "blocksize" << blockSize << readLen;
                code = qToBigEndian((uint16_t) _DATA);
                //print << __LINE__ << code;
                memcpy(sptr, &code, 2);
                sptr += 2;
                index = qToBigEndian((uint16_t) blockIndex);
                //print << __LINE__ << index;
                memcpy(sptr, &index, 2);
                sptr += 2;
                blockIndex++;
                sptr += readLen;
                {
                    QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                    //print << __LINE__ << (uint32_t) (sptr - sendBuf);
                    //print << __LINE__ << destAddress << destPort;
                    udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                }
                printProcess(totalSize, totalSize - file_size);

                break; // acknowledgement
            case _ERROR:
                timerout->stop();

                //print << ("_ERROR\n");
                clearStatus();
                //print << "error!\r\n";
                break; // error code
            case _OACK:
                // if(timer)
                // 	htimer_del(timer);
                //print << ("_OACK\n");
                break;
            default:
                // if(timer)
                // 	htimer_del(timer);
                //print << ("default\n");
                break;
            }
        }
    }
}

int Tftp::getFileSize(QString path)
{
    QFileInfo info(path);
    if (info.exists())
        return info.size();
    return 0;
}

void Tftp::sendErrorCode(enum TftpError code)
{
    char buf[128] = {0};
    const char *errorCodeMsg[] = {
        "Undefined error code",
        "File not found",
        "Access violation",
        "Disk full or allocation exceeded",
        "Illegal TFTP operation",
        "Unknown transfer ID",
        "File already exists",
        "No such user",
    };
    const char *msg = errorCodeMsg[code];
    //print << curFilePath + pre_filePath << msg;
    buf[0] = 0x00;
    buf[1] = 0x05;
    buf[2] = 0x00;
    buf[3] = (char) code;
    memcpy(&buf[4], msg, strlen(msg) + 1);
    {
        QByteArray data = QByteArray(buf, (int) strlen(msg) + 5);
        //print << __LINE__ << data;
        ;
        //print << __LINE__ << destAddress << destPort;
        udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
    }
    emit tftpErrorSignal(curFilePath + pre_filePath + "\n" + msg);
}

void Tftp::clearStatus()
{
    transportType = 0;
    file_size = 0;
    blockSize = 0;
    blockIndex = 1;
    if (file) {
        //print << fileMd5(*file);
        file->close();
        file->deleteLater();
    }
    file = nullptr;
    endtftp = 0;
    totalSize = 0;
    retryCount = 3;
    progress_bak = 0;
    //    if(udpSocket)
    //    {
    //        QObject::disconnect(udpSocket, &QUdpSocket::readyRead, this, &Tftp::readPendingDatagramsSlots);
    //        udpSocket->deleteLater();
    //        udpSocket=nullptr;
    //    }
    this->dataAvalivabe = false;
}

const QString Tftp::fileMd5(const QString &path)
{
    QFile sourceFile(path);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}
const QString Tftp::fileMd5(QFile &sourceFile)
{
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;
    if (sourceFile.isOpen()) {
        sourceFile.close();
    }
    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}
