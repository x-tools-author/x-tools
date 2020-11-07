/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonHostSection.hh"
#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusClientControllerTcp.hh"

SAKModbusClientControllerTcp::SAKModbusClientControllerTcp(QWidget *parent)
    :SAKModbusClientController(parent)
{
    mHostSection = new SAKModbusCommonHostSection(this);
    appendSection(mHostSection);

    mClient = new QModbusTcpClient(this);
}

void SAKModbusClientControllerTcp::open()
{
//    if (isClient()){
//        m_client = createClient(m_type);
//        if (m_client){
//            if(!initModbusDevice(m_client)){
//                ui->connectPushButton->setEnabled(true);
//            }
//        }
//    }else{
//        m_server = createServer(m_type);
//        if (m_server){
//            connect(m_server, &QModbusServer::dataWritten, this, [](QModbusDataUnit::RegisterType type, int address, int size){
//                qDebug() << __FUNCTION__ << type << address << size;
//            });
//            if (!initModbusDevice(m_server)){
//                ui->connectPushButton->setEnabled(true);
//            }
//        }
//    }
}
