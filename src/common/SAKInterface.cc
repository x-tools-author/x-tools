/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKInterface.hh"

SAKInterface::SAKInterface(QObject *parent)
    :QObject (parent)
{

}

QByteArray SAKInterface::byteArrayToHex(QByteArray &array, char separator)
{
    if (array.isEmpty()){
        return array;
    }

    /*
     * 好神奇的数据，参考Qt源码的写法，有点意思
     */
    auto toHex = [](quint8 value)->char{
        return "0123456789abcdef"[value & 0xF];
    };

    const int length = separator ? (array.size() * 3 - 1) : (array.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(array.constData());
    for (int i = 0, o = 0; i < array.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}
