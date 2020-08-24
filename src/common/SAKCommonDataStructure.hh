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

class SAKCommonDataStructure:public QObject
{
    Q_OBJECT
public:
    SAKCommonDataStructure(QObject* parent = Q_NULLPTR);

    // QSAK tools
    enum SAKEnumToolType {
#ifdef SAK_IMPORT_FILECHECKER_MODULE
        ToolTypeFileChecker,
#endif
#ifdef SAK_IMPORT_QRCODE_MODULE
        ToolTypeQRCodeCreator,
#endif
        ToolTypeCRCCalculator
    };
    Q_ENUM(SAKEnumToolType);

    // QSAK debugging page
    enum SAKEnumDebugPageType {
        DebugPageTypeTest,
#ifdef SAK_IMPORT_COM_MODULE
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
        DebugPageTypeSslSocketClient,
        DebugPageTypeSslSocketServer,
#ifdef SAK_IMPORT_SCTP_MODULE
        DebugPageTypeSCTPClient,
        DebugPageTypeSCTPServer,
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
        DebugPageTypeBluetoothClient,
        DebugPageTypeBluetoothServer,
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
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
        InputFormatAscii,
        InputFormatUtf8,
        InputFormatLocal
    };
    Q_ENUM(SAKEnumTextInputFormat);

    // Output text format
    enum SAKEnumTextOutputFormat {
        OutputFormatBin,
        OutputFormatOct,
        OutputFormatDec,
        OutputFormatHex,
        OutputFormatAscii,
        OutputFormatUtf8,
        OutputFormatLocal,
        OutputFormatUtf16,
        OutputFormatUcs4,
        OutputFormatStdwstring
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
};

#endif
