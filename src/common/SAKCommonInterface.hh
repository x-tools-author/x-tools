/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCOMMONINTERFACE_HH
#define SAKCOMMONINTERFACE_HH

#include <QObject>
#include <QLineEdit>

/// @brief This class provides interfaces to replace the functions introduced in the new version of Qt
/// so that the old version of Qt can also compile the project.
/// Also, it will provide some common interfaces used by the project.
class SAKCommonInterface:public QObject
{
    Q_OBJECT
public:
    SAKCommonInterface(QObject *parent = Q_NULLPTR);

    enum ValidatorType {
        ValidatorBin,
        ValidatorOtc,
        ValidatorDec,
        ValidatorHex,
        ValidatorAscii,
        ValidatorFloat
    };

    /**
     * @brief byteArrayToHex: Just like QByteArray::toHex(char) which introduced in Qt 5.9.
     * @param array:
     * @param separator: Can not be '\0' or it will be same as QByteArray::toHex()
     * @return ...
     */
    QByteArray byteArrayToHex(QByteArray &array, char separator);

    /**
     * @brief setLineEditValidator: Set validator to a line edit, it will delete the old validator.
     * @param lineEdit: Target
     * @param type: See ValidatorType for more information.
     */
    void setLineEditValidator(QLineEdit *lineEdit, ValidatorType type, int maxLength = INT_MAX);
};

#endif
