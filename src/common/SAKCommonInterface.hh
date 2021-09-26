/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKCOMMONINTERFACE_HH
#define SAKCOMMONINTERFACE_HH

#include <QObject>
#include <QSettings>
#include <QLineEdit>
#include <QComboBox>

#define microSetComboBoxIndexFromSettings(settings, settingsGroup, structMember, comboBox) \
    SAKCommonInterface::setComboBoxIndexFromSettings(settings,\
    settingsGroup + QString("/") + QString(#structMember).split('.').last(),\
    comboBox)
#define microSetSettingsFromComboBoxIndex(settings, settingsGroup, structMember, comboBox) \
    SAKCommonInterface::setSettingsFromComboBoxIndex(settings,\
    settingsGroup + QString("/") + QString(#structMember).split('.').last(),\
    comboBox)

class SAKCommonInterface:public QObject
{
    Q_OBJECT
public:
    SAKCommonInterface(QObject *parent = Q_NULLPTR);

    enum SAKEnumValidatorType {
        ValidatorBin,
        ValidatorOtc,
        ValidatorDec,
        ValidatorHex,
        ValidatorAscii,
        ValidatorFloat,
        ValidatorNone
    };

    /**
     * @brief byteArrayToHex: Just like QByteArray::toHex(char)
     * which introduced in Qt 5.9.
     * @param array:
     * @param separator: Can not be '\0' or it will be same as QByteArray::toHex().
     * @return ...
     */
    static QByteArray byteArrayToHex(QByteArray &array, char separator);

    /**
     * @brief setLineEditValidator: Set validator to a line edit, i
     * t will delete the old validator.
     * @param lineEdit: Target line edit.
     * @param type: See ValidatorType for more information.
     */
    static void setLineEditValidator(QLineEdit *lineEdit,
                                     SAKEnumValidatorType type,
                                     int maxLength = INT_MAX);
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    // Serialport interfaces
    static void addSerialPortNametItemsToComboBox(QComboBox *comboBox);
    static void addSerialPortBaudRateItemsToComboBox(QComboBox *comboBox);
    static void addSerialPortDataBitItemsToComboBox(QComboBox *comboBox);
    static void addSerialPortStopBitItemsToComboBox(QComboBox *comboBox);
    static void addSerialPortParityItemsToComboBox(QComboBox *comboBox);
    static void addSerialPortFlowControlItemsToComboBox(QComboBox *comboBox);
#endif
    static void addIpItemsToComboBox(QComboBox *comboBox, bool appendHostAny = false);
    static void setComboBoxIndexFromSettings(QSettings *setting, QString key, QComboBox *comboBox);
    static void setSettingsFromComboBoxIndex(QSettings *setting, QString key, QComboBox *comboBox);
};
#endif
