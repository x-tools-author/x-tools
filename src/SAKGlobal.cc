/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMetaEnum>
#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#endif
#include <QHostAddress>
#include <QApplication>
#include <QStandardPaths>
#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPortInfo>
#endif
#include <QNetworkInterface>

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"

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
    path.insert(path.lastIndexOf("/"),  QString("/Qter"));

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

QString SAKGlobal::getNameOfDebugPage(int type)
{
    QString name;
    switch (type) {
    case SAKDataStruct::DebugPageTypeUDP:
        name = tr("UDP调试");
        break;
    case SAKDataStruct::DebugPageTypeTCPClient:
        name = tr("TCP客户端");
        break;
    case SAKDataStruct::DebugPageTypeTCPServer:
        name = tr("TCP服务器");
        break;
#ifdef SAK_IMPORT_COM_MODULE
    case SAKDataStruct::DebugPageTypeCOM:
        name = tr("串口调试");
        break;
#endif
#ifdef SAK_IMPORT_HID_MODULE
    case SAKDataStruct::DebugPageTypeHID:
        name = tr("HID调试");
        break;
#endif
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow debug page type");
        name = QString("Unknow");
        break;
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
        comboBox->addItem(tr("无"), QVariant::fromValue(QSerialPort::NoParity));
        comboBox->addItem(tr("偶校验"), QVariant::fromValue(QSerialPort::EvenParity));
        comboBox->addItem(tr("奇校验"), QVariant::fromValue(QSerialPort::OddParity));
        comboBox->addItem(tr("空格检验"), QVariant::fromValue(QSerialPort::SpaceParity));
        comboBox->addItem(tr("标记校验"), QVariant::fromValue(QSerialPort::MarkParity));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initFlowControlComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("无"), QVariant::fromValue(QSerialPort::NoFlowControl));
        comboBox->addItem(tr("硬件流控"), QVariant::fromValue(QSerialPort::HardwareControl));
        comboBox->addItem(tr("软件流控"), QVariant::fromValue(QSerialPort::SoftwareControl));
    }
}
#endif

void SAKGlobal::initIpComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
        for(auto var:addresses){
            if (var.protocol() == QAbstractSocket::IPv4Protocol) {
                comboBox->addItem(var.toString());
            }
        }
    }
}

void SAKGlobal::initInputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        comboBox->addItem(tr("二进制"), SAKDataStruct::InputFormatBin);
        comboBox->addItem(tr("八进制"), SAKDataStruct::InputFormatOct);
        comboBox->addItem(tr("十进制"), SAKDataStruct::InputFormatDec);
        comboBox->addItem(tr("十六进制"), SAKDataStruct::InputFormatHex);
        comboBox->addItem(QString("ASCII"), SAKDataStruct::InputFormatAscii);
        comboBox->addItem(QString("UTF8"), SAKDataStruct::InputFormatUtf8);
        comboBox->addItem(tr("系统编码"), SAKDataStruct::InputFormatLocal);

        comboBox->setCurrentIndex(4);
    }
}

void SAKGlobal::initOutputTextFormatComboBox(QComboBox *comboBox)
{
    comboBox->addItem(tr("二进制"), SAKDataStruct::OutputFormatBin);
    comboBox->addItem(tr("八进制"), SAKDataStruct::OutputFormatOct);
    comboBox->addItem(tr("十进制"), SAKDataStruct::OutputFormatDec);
    comboBox->addItem(tr("十六进制"), SAKDataStruct::OutputFormatHex);
    comboBox->addItem(QString("ASCII"), SAKDataStruct::OutputFormatAscii);
    comboBox->addItem(QString("UTF8"), SAKDataStruct::OutputFormatUtf8);
    comboBox->addItem(QString("UTF16"), SAKDataStruct::OutputFormatUtf16);
    comboBox->addItem(QString("UCS4"), SAKDataStruct::OutputFormatUcs4);
    comboBox->addItem(tr("宽字符"), SAKDataStruct::OutputFormatStdwstring);
    comboBox->addItem(tr("系统编码"), SAKDataStruct::OutputFormatLocal);
}

void SAKGlobal::initCRCComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QMetaEnum enums = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
        for (int i = 0; i < enums.keyCount(); i++){
            comboBox->addItem(QString(enums.key(i)), QVariant::fromValue(enums.value(i)));
        }
    }
}

