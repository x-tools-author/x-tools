/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONSERIALPORTSECTION_HH
#define SAKMODBUSCOMMONSERIALPORTSECTION_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonSerialPortSection;
}

class SAKModbusCommonSerialPortSection : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonSerialPortSection(QWidget *parent = nullptr);
    ~SAKModbusCommonSerialPortSection();
private:
    Ui::SAKModbusCommonSerialPortSection *ui;
};

#endif // SAKMODBUSCOMMONSERIALPORTSECTION_HH
