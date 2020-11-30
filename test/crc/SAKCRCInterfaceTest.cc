/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 */

#include <QtTest>
#include "SAKCommonCrcInterface.hh"

/**
 * @brief crc计算接口测试
 */
class SAKCRCInterfaceTest:public QObject
{
    Q_OBJECT
public:
    SAKCRCInterfaceTest();
    ~SAKCRCInterfaceTest();
private:
    SAKCommonCrcInterface sakCRCInterface;

    QByteArray crcData;
private slots:
    void crc8();
    void crc8itu();
    void crc8rohc();
    void crc8maxim();

    void crc16ibm();
    void crc16maxim();
    void crc16usb();
    void crc16modbus();
    void crc16ccitt();
    void crc16ccittfalse();
    void crc16x25();
    void crc16xmodem();
    void crc16dnp();

    void crc32();
    void crc32mpeg2();
};

SAKCRCInterfaceTest::SAKCRCInterfaceTest()
{
    crcData = QByteArray("0123456789qwertyuiopasdfghjklzxcvbnm");
}

SAKCRCInterfaceTest::~SAKCRCInterfaceTest()
{

}

void SAKCRCInterfaceTest::crc8()
{
    quint8 crc = sakCRCInterface.crcCalculate<quint8>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_8);
    QCOMPARE(crc, 0x92);
}

void SAKCRCInterfaceTest::crc8itu()
{
    quint8 crc = sakCRCInterface.crcCalculate<quint8>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_8_ITU);
    QCOMPARE(crc, 0xC7);
}

void SAKCRCInterfaceTest::crc8rohc()
{
    quint8 crc = sakCRCInterface.crcCalculate<quint8>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_8_ROHC);
    QCOMPARE(crc, 0x07);
}

void SAKCRCInterfaceTest::crc8maxim()
{
    quint8 crc = sakCRCInterface.crcCalculate<quint8>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_8_MAXIM);
    QCOMPARE(crc, 0x54);
}

void SAKCRCInterfaceTest::crc16ibm()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_IBM);
    QCOMPARE(crc, 0x3631);
}

void SAKCRCInterfaceTest::crc16maxim()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_MAXIM);
    QCOMPARE(crc, 0xC9CE);
}

void SAKCRCInterfaceTest::crc16usb()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_USB);
    QCOMPARE(crc, 0xD98E);
}

void SAKCRCInterfaceTest::crc16modbus()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_MODBUS);
    QCOMPARE(crc, 0x2671);
}

void SAKCRCInterfaceTest::crc16ccitt()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_CCITT);
    QCOMPARE(crc, 0xEE13);
}

void SAKCRCInterfaceTest::crc16ccittfalse()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_CCITT_FALSE);
    QCOMPARE(crc, 0x2574);
}

void SAKCRCInterfaceTest::crc16x25()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_x25);
    QCOMPARE(crc, 0xBD6D);
}

void SAKCRCInterfaceTest::crc16xmodem()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_XMODEM);
    QCOMPARE(crc, 0xA441);
}

void SAKCRCInterfaceTest::crc16dnp()
{
    quint16 crc = sakCRCInterface.crcCalculate<quint16>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_16_DNP);
    QCOMPARE(crc, 0x7470);
}

void SAKCRCInterfaceTest::crc32()
{
    quint32 crc = sakCRCInterface.crcCalculate<quint32>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_32);
    QCOMPARE(crc, 0x71F94319);
}

void SAKCRCInterfaceTest::crc32mpeg2()
{
    quint32 crc = sakCRCInterface.crcCalculate<quint32>(reinterpret_cast<uint8_t*>(crcData.data()), uint64_t(crcData.length()), SAKCommonCrcInterface::CRC_32_MPEG2);
    QCOMPARE(crc, 0x44EF8D9D);
}












QTEST_MAIN(SAKCRCInterfaceTest)

#include "SAKCRCInterfaceTest.moc"
