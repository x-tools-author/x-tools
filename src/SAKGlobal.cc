/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDir>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QMetaEnum>
#include <QSslSocket>
#include <QHostAddress>
#include <QApplication>
#include <QMapIterator>
#include <QStandardItem>
#include <QStandardPaths>
#include <QNetworkInterface>
#include <QStandardItemModel>

#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include "SAKGlobal.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKCommonCrcInterface.hh"

#ifdef SAK_IMPORT_COM_MODULE
Q_DECLARE_METATYPE(QSerialPortInfo)
#endif
SAKGlobal::SAKGlobal(QObject* parent)
    :QObject (parent)
{

}

QString SAKGlobal::logFile()
{
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir;
    if (!dir.exists(fileName)){
        SAKGlobal::mkMutiDir(fileName);
    }    

    fileName.append("/");
    fileName.append("QtSwissArmyKnife.txt");

    return fileName;
}

QString SAKGlobal::dataPath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
//    path.insert(path.lastIndexOf("/"),  QString("/Qter"));

    QDir dir;
    if (!dir.exists(path)){
        mkMutiDir(path);
    }

    return path;
}

QString SAKGlobal::mkMutiDir(const QString path){

    QDir dir(path);
    if (dir.exists(path)){
        return path;
    }

    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);

    if ( !dirname.isEmpty() ){
        parentPath.mkpath(dirname);
    }

    return parentDir + "/" + dirname;
}

QString SAKGlobal::debugPageNameFromType(int type)
{
    QString name;
    switch (type) {
    case SAKCommonDataStructure::DebugPageTypeTest:
        name = tr("Test");
        break;
#ifdef SAK_IMPORT_COM_MODULE
    case SAKCommonDataStructure::DebugPageTypeCOM:
        name = tr("COM");
        break;
#endif
#ifdef SAK_IMPORT_HID_MODULE
    case SAKDataStruct::DebugPageTypeHID:
        name = tr("HID");
        break;
#endif
#ifdef SAK_IMPORT_USB_MODULE
    case SAKDataStruct::DebugPageTypeUSB:
        name = tr("USB");
        break;
#endif
    case SAKCommonDataStructure::DebugPageTypeUdpClient:
        name = tr("UDP-C");
        break;
    case SAKCommonDataStructure::DebugPageTypeUdpServer:
        name = tr("UDP-S");
        break;
    case SAKCommonDataStructure::DebugPageTypeTCPClient:
        name = tr("TCP-C");
        break;
    case SAKCommonDataStructure::DebugPageTypeTCPServer:
        name = tr("TCP-S");
        break;
    case SAKCommonDataStructure::DebugPageTypeSslSocketClient:
        name = tr("SSL-C");
        break;
    case SAKCommonDataStructure::DebugPageTypeSslSocketServer:
        name = tr("SSL-S");
        break;
#ifdef SAK_IMPORT_SCTP_MODULE
    case SAKDataStruct::DebugPageTypeSCTPClient:
        name = tr("SCTP-C");
        break;
    case SAKDataStruct::DebugPageTypeSCTPServer:
        name = tr("SCTP-S");
        break;
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
    case SAKDataStruct::DebugPageTypeBluetoothClient:
        name = tr("Bluetooth-C");
        break;
    case SAKDataStruct::DebugPageTypeBluetoothServer:
        name = tr("Bluetooth-S");
        break;
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
    case SAKCommonDataStructure::DebugPageTypeWebSocketClient:
        name = tr("WS-C");
        break;
    case SAKCommonDataStructure::DebugPageTypeWebSocketServer:
        name = tr("WS-S");
        break;
#endif
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow debug page type");
        name = QString("Unknow");
        break;
    }

    return name;
}

QString SAKGlobal::toolNameFromType(int type)
{
    QString name("Unknow name of tool");
    switch (type) {
#ifdef SAK_IMPORT_FILECHECKER_MODULE
    case SAKCommonDataStructure::ToolTypeFileChecker:
        name = QString("File checker");
        break;
#endif
    case SAKCommonDataStructure::ToolTypeCRCCalculator:
        name = QString("CRC calculator");
        break;
#ifdef SAK_IMPORT_QRCODE_MODULE
    case SAKCommonDataStructure::ToolTypeQRCodeCreator:
        name = QString("QR code creator");
        break;
#endif
    }

    return name;
}

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initComComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
        for(auto var:coms){
            comboBox->addItem(var.portName());
        }
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initBaudRateComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<qint32> bd = QSerialPortInfo::standardBaudRates();
        for (auto var:bd) {
            comboBox->addItem(QString::number(var), QVariant::fromValue(var));
        }

        comboBox->setCurrentText("9600");
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initDataBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("8", QVariant::fromValue(QSerialPort::Data8));
        comboBox->addItem("7", QVariant::fromValue(QSerialPort::Data7));
        comboBox->addItem("6", QVariant::fromValue(QSerialPort::Data6));
        comboBox->addItem("5", QVariant::fromValue(QSerialPort::Data5));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initStopBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("1", QVariant::fromValue(QSerialPort::OneStop));
#ifdef Q_OS_WINDOWS
        comboBox->addItem("1.5", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif
        comboBox->addItem("2", QVariant::fromValue(QSerialPort::TwoStop));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initParityComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(QSerialPort::NoParity));
        comboBox->addItem(tr("Even"), QVariant::fromValue(QSerialPort::EvenParity));
        comboBox->addItem(tr("Odd"), QVariant::fromValue(QSerialPort::OddParity));
        comboBox->addItem(tr("Space"), QVariant::fromValue(QSerialPort::SpaceParity));
        comboBox->addItem(tr("Mark"), QVariant::fromValue(QSerialPort::MarkParity));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initFlowControlComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(QSerialPort::NoFlowControl));
        comboBox->addItem(tr("Hardware"), QVariant::fromValue(QSerialPort::HardwareControl));
        comboBox->addItem(tr("Software"), QVariant::fromValue(QSerialPort::SoftwareControl));
    }
}
#endif

void SAKGlobal::initIpComboBox(QComboBox *comboBox, bool appendHostAny)
{
    QString localHost("127.0.0.1");
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("::"));
        comboBox->addItem(QString("::1"));
        comboBox->addItem(QString("0.0.0.0"));
        comboBox->addItem(localHost);
        QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
        for(auto var:addresses){
            if (var.protocol() == QAbstractSocket::IPv4Protocol) {
                if (var.toString().compare(localHost) == 0){
                    continue;
                }
                comboBox->addItem(var.toString());
            }
        }

        if (appendHostAny){
            comboBox->addItem(QString(SAK_HOST_ADDRESS_ANY));
        }
        comboBox->setCurrentText(localHost);
    }
}

void SAKGlobal::initInputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKCommonDataStructure::InputFormatBin, tr("BIN"));
        formatMap.insert(SAKCommonDataStructure::InputFormatOct, tr("DEC"));
        formatMap.insert(SAKCommonDataStructure::InputFormatDec, tr("HEX"));
        formatMap.insert(SAKCommonDataStructure::InputFormatHex, tr("ASCII"));
        formatMap.insert(SAKCommonDataStructure::InputFormatAscii, tr("UTF8"));
        formatMap.insert(SAKCommonDataStructure::InputFormatUtf8, tr("UTF16"));
        formatMap.insert(SAKCommonDataStructure::InputFormatLocal, tr("SYSTEM"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKCommonDataStructure::InputFormatLocal));

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }
    }
}

void SAKGlobal::initOutputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKCommonDataStructure::OutputFormatBin, tr("BIN"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatDec, tr("DEC"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatHex, tr("HEX"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatAscii, tr("ASCII"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf8, tr("UTF8"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf16, tr("UTF16"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUcs4, tr("UCS4"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatStdwstring, tr("WChart"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatLocal, tr("SYSTEM"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKCommonDataStructure::OutputFormatHex));

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }
    }
}

void SAKGlobal::initCRCComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QMetaEnum enums = QMetaEnum::fromType<SAKCommonCrcInterface::CRCModel>();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        for (int i = 0; i < enums.keyCount(); i++){
            const QString key = enums.key(i);
            // There may be a bug, I do not know whether will the itemModel take ownership of the item
            // if not, a memory leak will occur after comboBox is destroyed.
            QStandardItem *item = new QStandardItem(key);
            item->setToolTip(key);
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
    }
}

void SAKGlobal::initWebSocketSendingTypeComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->addItem(tr("BIN"), SAKCommonDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), SAKCommonDataStructure::WebSocketSendingTypeText);
    }
}
