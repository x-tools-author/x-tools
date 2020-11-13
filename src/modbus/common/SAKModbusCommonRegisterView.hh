/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONREIGSTERVIEW_HH
#define SAKMODBUSCOMMONREIGSTERVIEW_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonReigsterView;
}

class SAKModbusCommonReigsterView : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonReigsterView(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonReigsterView();

    /**
     * @brief addWidget: Add a register widgt to the layout
     * @param registerWisget: Register widget
     */
    void addWidget(QWidget *registerWisget);
private:
    Ui::SAKModbusCommonReigsterView *ui;
};

#endif // SAKMODBUSCOMMONREIGSTERVIEW_HH
