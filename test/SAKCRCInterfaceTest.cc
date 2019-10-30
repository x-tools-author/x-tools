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
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */

#include <QtTest>
#include "SAKCRCInterface.hh"

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
    SAKCRCInterface sakCRCInterface;
private slots:
    void crc8();
};

SAKCRCInterfaceTest::SAKCRCInterfaceTest()
{

}

SAKCRCInterfaceTest::~SAKCRCInterfaceTest()
{

}

void SAKCRCInterfaceTest::crc8()
{
    quint8 crc = sakCRCInterface.crcCalulate2<quint8>(nullptr, 0, SAKCRCInterface::CRC_8);
    QCOMPARE(crc, 0);
}













QTEST_APPLESS_MAIN(SAKCRCInterfaceTest)

#include "SAKCRCInterfaceTest.moc"
