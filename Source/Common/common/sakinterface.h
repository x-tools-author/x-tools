/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKINTERFACE_H
#define SAKINTERFACE_H

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QSettings>
#include <QVariant>

#define SAK_STATIC Q_INVOKABLE static

#define microIni2CoB(settings, settingsGroup, structMember, comboBox) \
    SAKInterface::setComboBoxIndexFromSettings(settings, \
                                               settingsGroup + QString("/") \
                                                   + QString(#structMember).split('.').last(), \
                                               comboBox)
#define microCoB2Ini(settings, settingsGroup, structMember, comboBox) \
    SAKInterface::setSettingsValueFromComboBoxIndex(settings, \
                                                    settingsGroup + QString("/") \
                                                        + QString(#structMember).split('.').last(), \
                                                    comboBox)
#define microIni2LE(settings, settingsGroup, structMember, lineEdit) \
    SAKInterface::setLineEditTextFromSettings(settings, \
                                              settingsGroup + QString("/") \
                                                  + QString(#structMember).split('.').last(), \
                                              lineEdit)
#define microLE2Ini(settings, settingsGroup, structMember, lineEdit) \
    SAKInterface::setSettingsValueFromLineEditText(settings, \
                                                   settingsGroup + QString("/") \
                                                       + QString(#structMember).split('.').last(), \
                                                   lineEdit)
#define microIni2ChB(settings, settingsGroup, structMember, checkBox) \
    SAKInterface::setCheckBoxValueFromSettings(settings, \
                                               settingsGroup + QString("/") \
                                                   + QString(#structMember).split('.').last(), \
                                               checkBox)
#define microChB2Ini(settings, settingsGroup, structMember, checkBox) \
    SAKInterface::setSettingsValueFromCheckBox(settings, \
                                               settingsGroup + QString("/") \
                                                   + QString(#structMember).split('.').last(), \
                                               checkBox)

class SAKInterface : public QObject
{
    Q_OBJECT
public:
    explicit SAKInterface(QObject *parent = nullptr);

    Q_INVOKABLE static void setMaximumBlockCount(QVariant doc, int maximum);
    Q_INVOKABLE static void setAppFont(const QString &fontFamily);
    Q_INVOKABLE static void setClipboardText(const QString &text);

    Q_INVOKABLE static bool isQtHighDpiScalePolicy(int policy);

    Q_INVOKABLE static QString arrayToString(const QByteArray &array, int format);
    Q_INVOKABLE static QString dateTimeString(const QString &format);
    Q_INVOKABLE static QString cookedFileName(const QString &fileName);
    Q_INVOKABLE static QString string2hexString(const QString &str);
    Q_INVOKABLE static QString hexString2String(const QString &str);
    Q_INVOKABLE static QString buildDateTime(const QString &format);
    Q_INVOKABLE static QString dateFormat();
    Q_INVOKABLE static QString timeFormat();

    Q_INVOKABLE static QByteArray string2array(const QString &input, int format);
    Q_INVOKABLE static QByteArray arrayAppendArray(const QByteArray &a1, const QByteArray &a2);
    Q_INVOKABLE static QByteArray arrayToHex(const QByteArray &array, char separator = '\0');

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
    static void setComboBoxIndexFromSettings(QSettings *settings, QString key, QComboBox *comboBox);
    static void setSettingsValueFromComboBoxIndex(QSettings *settings,
                                                  QString key,
                                                  QComboBox *comboBox);
    static void setLineEditTextFromSettings(QSettings *settings, QString key, QLineEdit *lineEdit);
    static void setSettingsValueFromLineEditText(QSettings *settings,
                                                 QString key,
                                                 QLineEdit *lineEdit);
    static void setCheckBoxValueFromSettings(QSettings *settings, QString key, QCheckBox *checkBox);
    static void setSettingsValueFromCheckBox(QSettings *settings, QString key, QCheckBox *checkBox);
};

#endif // SAKINTERFACE_H
