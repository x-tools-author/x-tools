/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONREIGSTERVIEWCONTROLLER_HH
#define SAKMODBUSCOMMONREIGSTERVIEWCONTROLLER_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonRegisterViewController;
}

class SAKModbusCommonRegisterViewController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonRegisterViewController(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonRegisterViewController();
private:
    Ui::SAKModbusCommonRegisterViewController *ui;
};

#endif // SAKMODBUSCOMMONREIGSTERVIEWCONTROLLER_HH
