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

    // QSAK tools
    enum SAKEnumToolType {
#ifdef SAK_IMPORT_MODULE_FILECHECKER
        ToolTypeFileChecker,
#endif
#ifdef SAK_IMPORT_MODULE_QRCODE
        ToolTypeQRCodeCreator,
#endif
        ToolTypeCRCCalculator
    };
    Q_ENUM(SAKEnumToolType);

    // QSAK debugging page
    enum SAKEnumDebugPageType {
        DebugPageTypeTest,
#ifdef SAK_IMPORT_MODULE_SERIALPORT
        DebugPageTypeCOM,
#endif
#ifdef SAK_IMPORT_HID_MODULE
        DebugPageTypeHID,
#endif
#ifdef SAK_IMPORT_USB_MODULE
        DebugPageTypeUSB,
#endif
        DebugPageTypeUdpClient,
        DebugPageTypeUdpServer,
        DebugPageTypeTCPClient,
        DebugPageTypeTCPServer,
#ifdef SAK_IMPORT_MODULE_SSLSOCKET
        DebugPageTypeSslSocketClient,
        DebugPageTypeSslSocketServer,
#endif
#ifdef SAK_IMPORT_SCTP_MODULE
        DebugPageTypeSCTPClient,
        DebugPageTypeSCTPServer,
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
        DebugPageTypeBluetoothClient,
        DebugPageTypeBluetoothServer,
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
        DebugPageTypeWebSocketClient,
        DebugPageTypeWebSocketServer
#endif
    };
    Q_ENUM(SAKEnumDebugPageType);

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

    struct SAKStructAutoResponseItem {
        quint64 id; // The id of response item
        QString name; //  Response item name
        QString referenceData;
        QString responseData;
        bool enable; // true-enable the response item
        quint32 referenceFormat; // see the SAKEnumTextInputFormat enum
        quint32 responseFormat; // See the SAKEnumTextInputFormat enum
        quint32 option; // response option, see the SAKEnumAutoResponseOption enum
        bool delay; // true-response delayly
        quint32 interval; // delay interval
    };

    struct SAKStructTimingSentItem {
        quint64 id; // The id of timing sent item
        quint32 interval; // Timing interval
        quint32 format; // The format of data that will be sent later
        QString comment; // The item comment
        QString data; // The data that will be sent later
    };

    struct SAKStructPresettingDataItem{
        quint64 id; // The id of the item
        quint32 format; // Data format, see the SAKEnumTextInputFormat enum
        QString description; // The description of item
        quint32 classify; // (The parameter is reserved)
        QString text; // Raw data that will be sent later
    };
public:
    /**
     * @brief autoResponseTableName: Get the table name of database
     * @param type: See the SAKEnumDebugPageType enum
     * @return The table name of database
     */
    static QString autoResponseTableName(int type);

    /**
     * @brief timingSendingTableName: Get the table name of database
     * @param type: See the SAKEnumDebugPageType enum
     * @return The table name of database
     */
    static QString timingSendingTableName(int type);

    /**
     * @brief presettingDataTableName: Get the table name of database
     * @param type: See the SAKEnumDebugPageType enum
     * @return The table name of database
     */
    static QString dataPresetTableName(int type);

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
     * @brief formattingString: Formatting input text of text edit.
     * @param textEdit: Target text edit.
     * @param format: See SAKEnumTextInputFormat for more information.
     */
    static QString formattingString(QString &origingString, SAKEnumTextInputFormat format);

    /**
     * @brief stringToByteArray: Transmit a QString to a QByteArray.
     * @param origingString: Origin string.
     * @param format: See SAKEnumTextOutputFormat for more information.
     * @return A QByteArray.
     */
    static QByteArray stringToByteArray(QString &origingString, SAKEnumTextInputFormat format);
private:
    static void setComboBoxItems(QComboBox *comboBox, QMap<int, QString> &formatMap, int index = 0);
};

#endif
