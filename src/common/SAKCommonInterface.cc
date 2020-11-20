/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
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

void SAKCommonInterface::setLineEditValidator(QLineEdit *lineEdit, SAKEnumValidatorType type, int maxLength)
{
    QMap<int, QRegularExpression> regExpMap;
    regExpMap.insert(ValidatorBin, QRegularExpression("([0-9a-f][0-9A-F][]){0,8}*"));
    regExpMap.insert(ValidatorOtc, QRegularExpression("([0-7][0-7][ ])*"));
    regExpMap.insert(ValidatorDec, QRegularExpression("([0-9][0-9][ ])*"));
    regExpMap.insert(ValidatorHex, QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*"));
    regExpMap.insert(ValidatorAscii, QRegularExpression("([ -~])*"));
    regExpMap.insert(ValidatorFloat, QRegularExpression("^[-+]?[0-9]*\\.?[0-9]+$"));

    if (lineEdit){
        if (regExpMap.contains(type)){
            if (lineEdit->validator()){
                delete lineEdit->validator();
            }

            auto regExpValidator = new QRegularExpressionValidator(regExpMap.value(type), lineEdit);
            lineEdit->setValidator(regExpValidator);
            lineEdit->setMaxLength(maxLength);
        }
    }
}
