/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKSETTINGS_H
#define SAKSETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QApplication>

#define SAKSETTINGFILE (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/SAKApplication.ini")

#define APPLICATIONSTYLE            "Application/style"

#define UNKNOWINPUTMODE             "Unknow/inputMode"
#define UNKNOWOUTPUTMODE            "Unknow/outputMode"
#define UNKNOWCYCLETIME             "Unknow/cycleTime"
#define UNKNOWDELAYTIME             "Unknow/delayTime"

#define SERIALPORTINPUTMODE         "Serialport/inputMode"
#define SERIALPORTOUTPUTMODE        "Serialport/outputMode"
#define SERIALPORTCYCLETIME         "Serialport/cycleTime"
#define SERIALPORTDELAYTIME         "Serialport/delayTime"

#define UDPCLIENTINPUTMODE          "UdpClient/inputMode"
#define UDPCLIENTOUTPUTMODE         "UdpClient/outputMode"
#define UDPCLIENTCYCLETIME          "UdpClient/cycleTime"
#define UDPCLIENTDELAYTIME          "UdpClient/delayTime"
#define UDPCLIENTLOCALPORT          "UdpClient/localPort"
#define UDPCLIENTPEERPORT           "UdpClient/peerPort"
#define UDPCLIENTPEERADDRESS        "UdpClient/peerAddress"

#define TCPCLIENTINPUTMODE          "TcpClient/inputMode"
#define TCPCLIENTOUTPUTMODE         "TcpClient/outputMode"
#define TCPCLIENTCYCLETIME          "TcpClient/cycleTime"
#define TCPCLIENTDELAYTIME          "TcpClient/delayTime"
#define TCPCLIENTLOCALPORT          "TcpClient/localPort"
#define TCPCLIENTPEERPORT           "TcpClient/peerPort"
#define TCPCLIENTPEERADDRESS        "TcpClient/peerAddress"

#define TCPSERVERINPUTMODE          "TcpServer/inputMode"
#define TCPSERVEROUTPUTMODE         "TcpServer/outputMode"
#define TCPSERVERCYCLETIME          "TcpServer/cycleTime"
#define TCPSERVERDELAYTIME          "TcpServer/delayTime"
#define TCPSERVERADDRESS            "TcpServer/serverAddress"
#define TCPSERVERPORT               "TcpServer/serverPort"

#define MODEBIN                     "bin"
#define MODEOCT                     "oct"
#define MODEDEC                     "dec"
#define MODEHEX                     "hex"
#define MODEASCII                   "ascii"
#define MODEUTF8                    "utf8"

class SAKSettings:public QSettings
{
    Q_OBJECT
public:
    SAKSettings(const QString &fileName = SAKSETTINGFILE, Format format = QSettings::IniFormat, QObject *parent = Q_NULLPTR);
    ~SAKSettings();

    static SAKSettings *_sakSettings;

    static QString valueApplicationStyle();
    void setValueApplicationStyle(QString value);

    /// Unknow
    QString valueUnknowInputMode()                          {return value(UNKNOWINPUTMODE).toString();}
    void setValueUnknowInputMode(QString mode)              {setValue(UNKNOWINPUTMODE, mode);}
    QString valueUnknowOutputMode()                         {return value(UNKNOWOUTPUTMODE).toString();}
    void setValueUnknowOutputMode(QString mode)             {setValue(UNKNOWOUTPUTMODE, mode);}
    QString valueUnknowCycleTime()                          {return value(UNKNOWCYCLETIME).toString();}
    void setValueUnknowCycleTime(QString time)              {setValue(UNKNOWCYCLETIME, time);}
    QString valueUnknowDelayTime()                          {return value(UNKNOWDELAYTIME).toString();}
    void setValueUnknowDelayTime(QString time)              {setValue(UNKNOWDELAYTIME, time);}

    /// 串口配置选项
    QString valueSerialportInputMode()                      {return value(SERIALPORTINPUTMODE).toString();}
    void setValueSerialportInputMode(QString mode)          {setValue(SERIALPORTINPUTMODE, mode);}
    QString valueSerialportOutputMode()                     {return value(SERIALPORTOUTPUTMODE).toString();}
    void setValueSerialportOutputMode(QString mode)         {setValue(SERIALPORTOUTPUTMODE, mode);}
    QString valueSerialportCycleTime()                      {return value(SERIALPORTCYCLETIME).toString();}
    void setValueSerialportCycleTime(QString time)          {setValue(SERIALPORTCYCLETIME, time);}
    QString valueSerialportDelayTime()                      {return value(SERIALPORTDELAYTIME).toString();}
    void setValueSerialportDelayTime(QString time)          {setValue(SERIALPORTDELAYTIME, time);}

    /// UDP 客户端配置选项
    QString valueUdpClientInputMode()                       {return value(UDPCLIENTINPUTMODE).toString();}
    void setValueUdpClientInputMode(QString mode)           {setValue(UDPCLIENTINPUTMODE, mode);}
    QString valueUdpClientOutputMode()                      {return value(UDPCLIENTOUTPUTMODE).toString();}
    void setValueUdpClientOutputMode(QString mode)          {setValue(UDPCLIENTOUTPUTMODE, mode);}
    QString valueUdpClientCycleTime()                       {return value(UDPCLIENTCYCLETIME).toString();}
    void setValueUdpClientCycleTime(QString time)           {setValue(UDPCLIENTCYCLETIME, time);}
    QString valueUdpClientDelayTime()                       {return value(UDPCLIENTDELAYTIME).toString();}
    void setValueUdpClientDelayTime(QString time)           {setValue(UDPCLIENTDELAYTIME, time);}
    QString valueUdpClientLocalPort()                       {return value(UDPCLIENTLOCALPORT).toString();}
    void setValueUdpClientLocalPort(QString port)           {setValue(UDPCLIENTLOCALPORT, port);}
    QString valueUdpClientPeerPort()                        {return value(UDPCLIENTPEERPORT).toString();}
    void setValueUdpClientPeerPort(QString port)            {setValue(UDPCLIENTPEERPORT, port);}
    QString valueUdpClientPeerAddress()                     {return value(UDPCLIENTPEERADDRESS).toString();}
    void setValueUdpClientPeerAddress(QString address)      {setValue(UDPCLIENTPEERADDRESS, address);}

    /// TCP 客户端
    QString valueTcpClientInputMode()                       {return value(TCPCLIENTINPUTMODE).toString();}
    void setValueTcpClientInputMode(QString mode)           {setValue(TCPCLIENTINPUTMODE, mode);}
    QString valueTcpClientOutputMode()                      {return value(TCPCLIENTOUTPUTMODE).toString();}
    void setValueTcpClientOutputMode(QString mode)          {setValue(TCPCLIENTOUTPUTMODE, mode);}
    QString valueTcpClientCycleTime()                       {return value(TCPCLIENTCYCLETIME).toString();}
    void setValueTcpClientCycleTime(QString time)           {setValue(TCPCLIENTCYCLETIME, time);}
    QString valueTcpClientDelayTime()                       {return value(TCPCLIENTDELAYTIME).toString();}
    void setValueTcpClientDelayTime(QString time)           {setValue(TCPCLIENTDELAYTIME, time);}
    QString valueTcpClientLocalPort()                       {return value(TCPCLIENTLOCALPORT).toString();}
    void setValueTcpClientLocalPort(QString port)           {setValue(TCPCLIENTLOCALPORT, port);}
    QString valueTcpClientPeerPort()                        {return value(TCPCLIENTPEERPORT).toString();}
    void setValueTcpClientPeerPort(QString port)            {setValue(TCPCLIENTPEERPORT, port);}
    QString valueTcpClientPeerAddress()                     {return value(TCPCLIENTPEERADDRESS).toString();}
    void setValueTcpClientPeerAddress(QString address)      {setValue(TCPCLIENTPEERADDRESS, address);}

    /// TCP 服务器
    QString valueTcpServerInputMode()                       {return value(TCPSERVERINPUTMODE).toString();}
    void setValueTcpServerInputMode(QString mode)           {setValue(TCPSERVERINPUTMODE, mode);}
    QString valueTcpServerOutputMode()                      {return value(TCPSERVEROUTPUTMODE).toString();}
    void setValueTcpServerOutputMode(QString mode)          {setValue(TCPSERVEROUTPUTMODE, mode);}
    QString valueTcpServerCycleTime()                       {return value(TCPSERVERCYCLETIME).toString();}
    void setValueTcpServerCycleTime(QString time)           {setValue(TCPSERVERCYCLETIME, time);}
    QString valueTcpServerDelayTime()                       {return value(TCPSERVERDELAYTIME).toString();}
    void setValueTcpServerDelayTime(QString time)           {setValue(TCPSERVERDELAYTIME, time);}
    QString valueTcpServerServerAddress()                   {return value(TCPSERVERINPUTMODE).toString();}
    void setValueTcpServerServerAddress(QString address)    {setValue(TCPSERVERINPUTMODE, address);}
    QString valueTcpServerServerPort()                      {return value(TCPSERVERPORT).toString();}
    void setValueTcpServerServerPort(QString port)          {setValue(TCPSERVERPORT, port);}

};

SAKSettings *sakSettings();

#endif
