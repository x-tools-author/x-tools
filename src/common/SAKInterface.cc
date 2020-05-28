/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKCommonInterface.hh"

SAKCommonInterface::SAKCommonInterface(QObject *parent)
    :QObject (parent)
{

}

QByteArray SAKCommonInterface::byteArrayToHex(QByteArray &array, char separator)
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
