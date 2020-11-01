/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONREGISTER_HH
#define SAKMODBUSCOMMONREGISTER_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonRegister;
}

class SAKModbusCommonRegister : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonRegister(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonRegister();
private:
    Ui::SAKModbusCommonRegister *ui;
};

#endif // SAKMODBUSCOMMONREGISTER_HH
