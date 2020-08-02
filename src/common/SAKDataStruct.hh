/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDATASTRUCT_HH
#define SAKDATASTRUCT_HH

#include <QObject>

class SAKDataStruct:public QObject
{
    Q_OBJECT
public:
    SAKDataStruct(QObject* parent = Q_NULLPTR);

    /// @brief 支持的调试工具类型
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

    /// @brief 支持调试的设备类型
    enum SAKEnumDebugPageType {
#ifdef SAK_IMPORT_COM_MODULE
        DebugPageTypeCOM,
#endif
#ifdef SAK_IMPORT_HID_MODULE
        DebugPageTypeHID,
#endif
        DebugPageTypeUdpClient,
#ifdef SAK_IMPORT_USB_MODULE
        DebugPageTypeUSB,
#endif
        DebugPageTypeTCPClient,
        DebugPageTypeTCPServer,
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

    /// @brief 文本输出格式
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

    /// @brief 文本输出格式
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

    /// @brief 调试页面，自动回复选项
    enum SAKEnumAutoResponseOption {
        AutoResponseOptionEqual,        // 接收数据与参考数据相等时回复
        AutoResponseOptionContain,      // 接收数据包含参考数据时回复
        AutoResponseOptionDoNotContain  // 接收数据不包含参考数据时回复
    };
    Q_ENUM(SAKEnumAutoResponseOption);

    /// @brief web socket 发送接口类型
    enum SAKEnumWebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBin,
    };
    Q_ENUM(SAKEnumWebSocketSendingType);

    /// @brief 调试页面自动回复数据的数据结构
    struct SAKStructAutoResponseItem {
        quint64 id;                 /// 自动回复条目ID,回复条目唯一标识符（取值为数据产生时的时间戳）
        QString name;               /// 该自动回复数据名称
        QString referenceData;      /// 参考数据
        QString responseData;       /// 回复数据
        bool enable;                /// 该自动回复是否生效
        quint32 referenceFormat;    /// 参考数据格式，详情查看SAKEnumTextInputFormat
        quint32 responseFormat;     /// 回复数据格式，详情查看SAKEnumTextInputFormat
        quint32 option;             /// 回复选项
        bool delay; // true-response delayly
        quint32 interval; // delay interval
    };

    /// @brief 调试页面定时发送数的据结构
    struct SAKStructTimingSentItem {
        quint64 id;
        quint32 interval;   /// 定时间隔
        quint32 format;     /// 文本格式
        QString comment;    /// 备注
        QString data;       /// 定时发送数据
    };

    /// @brief 调试页面预设数据的数据结构
    struct SAKStructPresettingDataItem{
        quint64 id;
        quint32 format;     /// 预设数据格式
        QString description;    /// 预设数据备注
        quint32 classify;   /// 预设数据分类
        QString text;       /// 预设数据
    };
public:
    /**
     * @brief autoResponseTableName 获取自动数据库中自动应答数据表的表名称
     * @param type 调试类型，详情查看SAKEnumDebugPageType
     * @return 数据表名称
     */
    static QString autoResponseTableName(int type);

    /**
     * @brief timingSendingTableName 获取定时发送数据表的表名称
     * @param type 调试类型，详情查看SAKEnumDebugPageType
     * @return 数据表名称
     */
    static QString timingSendingTableName(int type);

    /**
     * @brief presettingDataTableName 获取数据预设数据表的表名称
     * @param type 调试类型，详情查看SAKEnumDebugPageType
     * @return 数据表名称
     */
    static QString dataPresetTableName(int type);
};

#endif
