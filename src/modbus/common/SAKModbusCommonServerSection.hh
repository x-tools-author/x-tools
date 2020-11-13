/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONSERVERSECTION_HH
#define SAKMODBUSCOMMONSERVERSECTION_HH

#include <QWidget>
#include <QModbusServer>

namespace Ui {
    class SAKModbusCommonServerSection;
}

class SAKModbusCommonServerSection : public QWidget
{
    Q_OBJECT
public:
    SAKModbusCommonServerSection(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonServerSection();

    void initModbusServerParameters(QModbusServer *server);
private:
    Ui::SAKModbusCommonServerSection *ui;
};

#endif // SAKMODBUSCOMMONSERVERSECTION_HH
