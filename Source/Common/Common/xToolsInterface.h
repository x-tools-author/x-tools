/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QSettings>
#include <QVariant>

#define SAK_STATIC Q_INVOKABLE static

class xToolsInterface : public QObject
{
    Q_OBJECT
public:
    explicit xToolsInterface(QObject *parent = nullptr);

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
    static void setLineEditValidator(QLineEdit *lineEdit, int type, int maxLength = INT_MAX);
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
