/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKCOMMONDATASTRUCTURE_HH
#define SAKCOMMONDATASTRUCTURE_HH

#include <QMap>
#include <QObject>
#include <QTextEdit>
#include <QComboBox>

#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include <QSerialPort>
#endif

/// @brief The class define some data structure of the project.
/// Also, It provides some interface about these data structure.
class SAKCommonDataStructure:public QObject
{
    Q_OBJECT
public:
    SAKCommonDataStructure(QObject* parent = Q_NULLPTR);
    ~SAKCommonDataStructure();

    // Input text format
    enum SAKEnumTextFormatInput {
        InputFormatBin,
        InputFormatOct,
        InputFormatDec,
        InputFormatHex,
        InputFormatAscii,
        InputFormatLocal
    };
    Q_ENUM(SAKEnumTextFormatInput);

    // Output text format
    enum SAKEnumTextFormatOutput {
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
    Q_ENUM(SAKEnumTextFormatOutput);

//    // Auto response options
//    enum SAKEnumAutoResponseOption {
//        AutoResponseOptionEqual, // Rx data is equal to reference data
//        AutoResponseOptionContain, // Rx data contains reference data
//        AutoResponseOptionDoNotContain  // Rx data does not contain reference data
//    };
//    Q_ENUM(SAKEnumAutoResponseOption);

    // Web socket sending type
    enum SAKEnumWebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBin,
    };
    Q_ENUM(SAKEnumWebSocketSendingType);

    enum SAKEmnuSuffixsType {
        SuffixsTypeNone,
        SuffixsTypeR,
        SuffixsTypeN,
        SuffixsTypeRN,
        SuffixsTypeNR
    };
    Q_ENUM(SAKEmnuSuffixsType);

#ifdef SAK_IMPORT_MODULE_SERIALPORT
    struct SAKStructSerialPortParametersContext {
        QString portName;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;

        qint64 intervalNs;
    };
#endif
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
    static QString formattingString(QString &origingString, SAKEnumTextFormatInput format);

    /**
     * @brief stringToByteArray: Transmit a QString to a QByteArray.
     * @param origingString: Origin string.
     * @param format: See SAKEnumTextInputFormat for more information.
     * @return A QByteArray.
     */
    static QByteArray stringToByteArray(QString &origingString, SAKEnumTextFormatInput format);

    /**
     * @brief byteArrayToString: Transmit a QByteArray to a QString.
     * @param origingString: Origin byte array.
     * @param format: See SAKEnumTextOutputFormat for more information.
     * @return A QString.
     */
    static QString byteArrayToString(QByteArray &origingData, SAKEnumTextFormatOutput format);

    /**
     * @brief setLineEditTextFormat: Formating input
     * @param lineEdit: Target component
     * @param format: (SAKEnumTextInputFormat)
     */
    static void setLineEditTextFormat(QLineEdit *lineEdit,
                                      SAKEnumTextFormatInput format);

    static QString suffix(SAKEmnuSuffixsType type);
    static QString friendlySuffix(SAKEmnuSuffixsType type);
    static void formattingInputText(QTextEdit *textEdit, int model);
private:
    static void setComboBoxItems(QComboBox *comboBox, QMap<int, QString> &formatMap, int currentData);
private:
    static QMap<int, QRegExpValidator*> mRegExpMap;
};

#endif
