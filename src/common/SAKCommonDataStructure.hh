/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCOMMONDATASTRUCTURE_HH
#define SAKCOMMONDATASTRUCTURE_HH

#include <QObject>
#include <QTextEdit>
#include <QComboBox>

/// @brief The class define some data structure of the project.
/// Also, It provides some interface about these data structure.
class SAKCommonDataStructure:public QObject
{
    Q_OBJECT
public:
    SAKCommonDataStructure(QObject* parent = Q_NULLPTR);

    // Input text format
    enum SAKEnumTextInputFormat {
        InputFormatBin,
        InputFormatOct,
        InputFormatDec,
        InputFormatHex,
        InputFormatUtf8,
        InputFormatAscii,
        InputFormatLocal
    };
    Q_ENUM(SAKEnumTextInputFormat);

    // Output text format
    enum SAKEnumTextOutputFormat {
        OutputFormatBin,
        OutputFormatOct,
        OutputFormatDec,
        OutputFormatHex,
        OutputFormatUcs4,
        OutputFormatUtf8,
        OutputFormatUtf16,
        OutputFormatAscii,
        OutputFormatLocal,
    };
    Q_ENUM(SAKEnumTextOutputFormat);

    // Auto response options
    enum SAKEnumAutoResponseOption {
        AutoResponseOptionEqual, // Rx data is equal to reference data
        AutoResponseOptionContain, // Rx data contains reference data
        AutoResponseOptionDoNotContain  // Rx data does not contain reference data
    };
    Q_ENUM(SAKEnumAutoResponseOption);

    // Web socket sending type
    enum SAKEnumWebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBin,
    };
    Q_ENUM(SAKEnumWebSocketSendingType);
public:
    /**
     * @brief setComboBoxTextOutputFormat: Add output text format items to combo box.
     * @param comboBox: Targat combo box.
     */
    static void setComboBoxTextOutputFormat(QComboBox *comboBox);

    /**
     * @brief setComboBoxTextInputFormat: Add input text format items to combo Box.
     * @param comboBox: Target combo box.
     */
    static void setComboBoxTextInputFormat(QComboBox *comboBox);

    /**
     * @brief setComboBoxTextWebSocketSendingType: Add supported types to combo box.
     * @param comboBox: Target combo box.
     */
    static void setComboBoxTextWebSocketSendingType(QComboBox *comboBox);

    /**
     * @brief formattingString: Formatting input text of text edit.
     * @param textEdit: Target text edit.
     * @param format: See SAKEnumTextInputFormat for more information.
     */
    static QString formattingString(QString &origingString, SAKEnumTextInputFormat format);

    /**
     * @brief stringToByteArray: Transmit a QString to a QByteArray.
     * @param origingString: Origin string.
     * @param format: See SAKEnumTextInputFormat for more information.
     * @return A QByteArray.
     */
    static QByteArray stringToByteArray(QString &origingString, SAKEnumTextInputFormat format);

    /**
     * @brief byteArrayToString: Transmit a QByteArray to a QString.
     * @param origingString: Origin byte array.
     * @param format: See SAKEnumTextOutputFormat for more information.
     * @return A QString.
     */
    static QString byteArrayToString(QByteArray &origingData, SAKEnumTextOutputFormat format);
private:
    static void setComboBoxItems(QComboBox *comboBox, QMap<int, QString> &formatMap, int currentData);
};

#endif
