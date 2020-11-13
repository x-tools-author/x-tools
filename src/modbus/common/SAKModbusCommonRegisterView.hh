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
#include <QModbusDataUnit>

namespace Ui {
    class SAKModbusCommonReigsterView;
}

class SAKModbusCommonFlowLayout;
class SAKModbusCommonRegisterView : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonRegisterView(QModbusDataUnit::RegisterType registerType, QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonRegisterView();

    /**
     * @brief addWidget: Add a register widgt to the layout
     * @param registerWisget: Register widget
     */
    void addWidget(QWidget *registerWisget);

    /**
     * @brief updateRegister: Update the register widgets
     * @param startAddress: --
     * @param registerNumber: --
     */
    void updateRegister(int startAddress, int registerNumber);
private:
    SAKModbusCommonFlowLayout *mFlowLayout;
    QModbusDataUnit::RegisterType mRegisterType;
private:
    Ui::SAKModbusCommonReigsterView *ui;
};

#endif // SAKMODBUSCOMMONREIGSTERVIEW_HH
