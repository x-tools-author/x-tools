#include "tftpcommon.h"

#include <QDir>
#include <QtEndian>

namespace {

quint16 readUint16(const QByteArray& data, int offset)
{
    if (data.size() < offset + 2) {
        return 0;
    }

    return qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(data.constData() + offset));
}

void appendUint16(QByteArray& data, quint16 value)
{
    char bytes[2] = {0, 0};
    qToBigEndian<quint16>(value, reinterpret_cast<uchar*>(bytes));
    data.append(bytes, 2);
}

QList<QByteArray> splitNullTerminatedFields(const QByteArray& data, int startIndex)
{
    QList<QByteArray> fields;
    QByteArray field;

    for (int index = startIndex; index < data.size(); ++index) {
        const char current = data.at(index);
        if (current == '\0') {
            fields.append(field);
            field.clear();
            continue;
        }

        field.append(current);
    }

    if (!field.isEmpty()) {
        fields.append(field);
    }

    return fields;
}

} // namespace

namespace Tftp {

QByteArray makeRequest(Opcode opcode, const QString& fileName, const QMap<QString, QString>& options)
{
    QByteArray datagram;
    appendUint16(datagram, static_cast<quint16>(opcode));
    datagram.append(fileName.toUtf8());
    datagram.append('\0');
    datagram.append("octet");
    datagram.append('\0');

    for (auto it = options.constBegin(); it != options.constEnd(); ++it) {
        datagram.append(it.key().toUtf8().toLower());
        datagram.append('\0');
        datagram.append(it.value().toUtf8());
        datagram.append('\0');
    }

    return datagram;
}

QByteArray makeData(quint16 block, const QByteArray& payload)
{
    QByteArray datagram;
    appendUint16(datagram, static_cast<quint16>(Opcode::Data));
    appendUint16(datagram, block);
    datagram.append(payload);
    return datagram;
}

QByteArray makeAck(quint16 block)
{
    QByteArray datagram;
    appendUint16(datagram, static_cast<quint16>(Opcode::Ack));
    appendUint16(datagram, block);
    return datagram;
}

QByteArray makeError(ErrorCode code, const QString& message)
{
    QByteArray datagram;
    appendUint16(datagram, static_cast<quint16>(Opcode::Error));
    appendUint16(datagram, static_cast<quint16>(code));
    datagram.append(message.toUtf8());
    datagram.append('\0');
    return datagram;
}

QByteArray makeOack(const QMap<QString, QString>& options)
{
    QByteArray datagram;
    appendUint16(datagram, static_cast<quint16>(Opcode::OptionAck));
    for (auto it = options.constBegin(); it != options.constEnd(); ++it) {
        datagram.append(it.key().toUtf8().toLower());
        datagram.append('\0');
        datagram.append(it.value().toUtf8());
        datagram.append('\0');
    }

    return datagram;
}

bool parsePacket(const QByteArray& datagram, Packet* packet)
{
    if (!packet || datagram.size() < 2) {
        return false;
    }

    packet->opcode = static_cast<Opcode>(readUint16(datagram, 0));
    packet->fileName.clear();
    packet->mode.clear();
    packet->options.clear();
    packet->block = 0;
    packet->payload.clear();
    packet->errorCode = 0;
    packet->errorMessage.clear();

    switch (packet->opcode) {
    case Opcode::ReadRequest:
    case Opcode::WriteRequest: {
        const QList<QByteArray> fields = splitNullTerminatedFields(datagram, 2);
        if (fields.size() < 2) {
            return false;
        }

        packet->fileName = QString::fromUtf8(fields.at(0));
        packet->mode = QString::fromUtf8(fields.at(1)).toLower();
        for (int index = 2; index + 1 < fields.size(); index += 2) {
            packet->options.insert(QString::fromUtf8(fields.at(index)).toLower(),
                                   QString::fromUtf8(fields.at(index + 1)));
        }
        return true;
    }
    case Opcode::Data:
        if (datagram.size() < 4) {
            return false;
        }
        packet->block = readUint16(datagram, 2);
        packet->payload = datagram.mid(4);
        return true;
    case Opcode::Ack:
        if (datagram.size() != 4) {
            return false;
        }
        packet->block = readUint16(datagram, 2);
        return true;
    case Opcode::Error:
        if (datagram.size() < 5) {
            return false;
        }
        packet->errorCode = readUint16(datagram, 2);
        packet->errorMessage = QString::fromUtf8(datagram.mid(4)).trimmed();
        return true;
    case Opcode::OptionAck: {
        const QList<QByteArray> fields = splitNullTerminatedFields(datagram, 2);
        for (int index = 0; index + 1 < fields.size(); index += 2) {
            packet->options.insert(QString::fromUtf8(fields.at(index)).toLower(),
                                   QString::fromUtf8(fields.at(index + 1)));
        }
        return true;
    }
    }

    return false;
}

QString endpointString(const QHostAddress& address, quint16 port)
{
    return QString("%1:%2").arg(address.toString()).arg(port);
}

QString sanitizeRelativePath(const QString& path)
{
    QString normalized = QDir::fromNativeSeparators(path.trimmed());
    while (normalized.startsWith('/')) {
        normalized.remove(0, 1);
    }

    normalized = QDir::cleanPath(normalized);
    if (normalized == ".") {
        normalized.clear();
    }

    return normalized;
}

} // namespace Tftp