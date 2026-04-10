/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QHostAddress>
#include <QMap>
#include <QString>

namespace Tftp {

constexpr quint16 kDefaultPort = 69;
constexpr int kDefaultBlockSize = 512;
constexpr int kTimeoutMs = 5000;
constexpr int kMaxRetries = 5;

enum class Opcode : quint16 {
    ReadRequest = 1,
    WriteRequest = 2,
    Data = 3,
    Ack = 4,
    Error = 5,
    OptionAck = 6
};

enum class ErrorCode : quint16 {
    Undefined = 0,
    FileNotFound = 1,
    AccessViolation = 2,
    DiskFull = 3,
    IllegalOperation = 4,
    UnknownTransferId = 5,
    FileAlreadyExists = 6,
    NoSuchUser = 7,
    OptionNegotiationFailed = 8
};

struct Packet
{
    Opcode opcode{Opcode::Error};
    QString fileName;
    QString mode;
    QMap<QString, QString> options;
    quint16 block{0};
    QByteArray payload;
    quint16 errorCode{0};
    QString errorMessage;
};

QByteArray makeRequest(Opcode opcode,
                       const QString& fileName,
                       const QMap<QString, QString>& options = {});
QByteArray makeData(quint16 block, const QByteArray& payload);
QByteArray makeAck(quint16 block);
QByteArray makeError(ErrorCode code, const QString& message);
QByteArray makeOack(const QMap<QString, QString>& options);
bool parsePacket(const QByteArray& datagram, Packet* packet);
QString endpointString(const QHostAddress& address, quint16 port);
QString sanitizeRelativePath(const QString& path);

} // namespace Tftp