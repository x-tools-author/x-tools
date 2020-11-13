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
    class SAKModbusCommonReigsterViewController;
}

class SAKModbusCommonReigsterViewController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonReigsterViewController(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonReigsterViewController();
private:
    Ui::SAKModbusCommonReigsterViewController *ui;
};

#endif // SAKMODBUSCOMMONREIGSTERVIEWCONTROLLER_HH
