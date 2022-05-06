/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
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

#ifdef SAK_IMPORT_MODULE_TEST
    struct SAKStructTestParametersContext {
        bool openFailed;
        bool readCircularly;
        int readInterval;
        bool writeCircularly;
        int writtingInterval;
        QString errorString;
    };
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
    struct SAKStructSerialPortParametersContext {
        QString portName;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;

        int frameIntervel;
    };
#endif

#ifdef SAK_IMPORT_MODULE_UDP
#ifdef SAK_IMPORT_MODULE_UDP_CLIENT
    struct SAKStructUdpClientParametersContext {
        QString peerHost;
        quint16 peerPort;
        QString localHost;
        quint16 localPort;
        bool specifyLocalInfo;
        bool pauseSending;
    };
    struct SAKStructUdpServerParametersContext {
        QString serverHost;
        quint16 serverPort;

        QString currentClientHost;
        quint16 currentClientPort;
        QStringList clients;
    };
#endif
#endif

#ifdef SAK_IMPORT_MODULE_TCP
#ifdef SAK_IMPORT_MODULE_TCP_CLIENT
    struct SAKStructTcpClientParametersContext {
        QString localHost;
        quint16 localPort;
        QString serverHost;
        quint16 serverPort;
        bool specifyClientAddressAndPort;
        bool allowAutomaticConnection;
    };
#endif
#ifdef SAK_IMPORT_MODULE_TCP_SERVER
    struct SAKStructTcpServerParametersContext {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
    };
#endif
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
    struct SAKStructWSClientParametersContext {
        QString serverAddress;
        quint32 sendingType;
    };
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_SERVER
    struct SAKStructWSServerParametersContext {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
        quint32 sendingType;
    };
#endif
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
    static QString formattingString(QString &origingString,
                                    SAKEnumTextFormatInput format);

    /**
     * @brief stringToByteArray: Transmit a QString to a QByteArray.
     * @param origingString: Origin string.
     * @param format: See SAKEnumTextInputFormat for more information.
     * @return A QByteArray.
     */
    static QByteArray stringToByteArray(QString &origingString,
                                        SAKEnumTextFormatInput format);
    static QByteArray stringToByteArray(QString &origingString, int format);

    /**
     * @brief byteArrayToString: Transmit a QByteArray to a QString.
     * @param origingString: Origin byte array.
     * @param format: See SAKEnumTextOutputFormat for more information.
     * @return A QString.
     */
    static QString byteArrayToString(QByteArray &origingData,
                                     SAKEnumTextFormatOutput format);

    /**
     * @brief setLineEditTextFormat: Formating input
     * @param lineEdit: Target component
     * @param format: (SAKEnumTextInputFormat)
     */
    static void setLineEditTextFormat(QLineEdit *lineEdit,
                                      SAKEnumTextFormatInput format);
    static void setLineEditTextFormat(QLineEdit *lineEdit, int format);

    static QString suffix(SAKEmnuSuffixsType type);
    static QString suffix(int type);
    static QString friendlySuffix(SAKEmnuSuffixsType type);
    static void setupSuffix(QComboBox *comboBox);

    static void formattingInputText(QTextEdit *textEdit, int model);
private:
    static void setComboBoxItems(QComboBox *comboBox,
                                 QMap<int, QString> &formatMap,
                                 int currentData);
};


typedef SAKCommonDataStructure::SAKEnumTextFormatInput SAKTextFormatInput;
typedef SAKCommonDataStructure::SAKEnumTextFormatOutput SAKTextFormatOutput;
typedef SAKCommonDataStructure::SAKEnumWebSocketSendingType SAKWSSendingType;
typedef SAKCommonDataStructure::SAKEmnuSuffixsType SAKSuffixsType;
#ifdef SAK_IMPORT_MODULE_SERIALPORT
typedef SAKCommonDataStructure::SAKStructSerialPortParametersContext
SAKSerialPortParametersContext;
#endif
typedef SAKCommonDataStructure::SAKStructUdpClientParametersContext
SAKUdpClientParametersContext;
typedef SAKCommonDataStructure::SAKStructUdpServerParametersContext
SAKUdpServerParametersContext;
typedef SAKCommonDataStructure::SAKStructTcpClientParametersContext
SAKTcpClientParametersContext;
typedef SAKCommonDataStructure::SAKStructTcpServerParametersContext
SAKTcpServerParametersContext;
typedef SAKCommonDataStructure::SAKStructWSClientParametersContext
SAKWSClientParametersContext;
typedef SAKCommonDataStructure::SAKStructWSServerParametersContext
SAKWSServerParametersContext;


Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructTestParametersContext);
#ifdef SAK_IMPORT_MODULE_SERIALPORT
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructSerialPortParametersContext);
#endif
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructUdpClientParametersContext);
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructUdpServerParametersContext);
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructTcpClientParametersContext);
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructTcpServerParametersContext);
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructWSClientParametersContext);
Q_DECLARE_METATYPE(SAKCommonDataStructure::SAKStructWSServerParametersContext);

#endif
