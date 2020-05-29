/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDATASTRUCT_HH
#define SAKDATASTRUCT_HH

#include <QObject>

class SAKDataStruct:public QObject
{
    Q_OBJECT
public:
    SAKDataStruct(QObject* parent = Q_NULLPTR);

    /// @brief 支持调试的设备类型
    enum SAKEnumDebugPageType {
#ifdef SAK_IMPORT_COM_MODULE
        DebugPageTypeCOM,
#endif
#ifdef SAK_IMPORT_HID_MODULE
        DebugPageTypeHID,
#endif
        DebugPageTypeUDP,
#ifdef SAK_IMPORT_USB_MODULE
        DebugPageTypeUSB,
#endif
        DebugPageTypeTCPClient,
        DebugPageTypeTCPServer
    };
    Q_ENUM(SAKEnumDebugPageType)

    /// @brief 文本输出格式
    enum SAKTextInputFormat {
        InputFormatBin,
        InputFormatOct,
        InputFormatDec,
        InputFormatHex,
        InputFormatAscii,
        InputFormatUtf8,
        InputFormatLocal
    };
    Q_ENUM(SAKTextInputFormat)

    /// @brief 文本输出格式
    enum SAKTextOutputFormat {
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
};

#endif
