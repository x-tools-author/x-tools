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
     * @brief updateRegister: Update the register widgets
     * @param startAddress: --
     * @param registerNumber: --
     */
    void updateRegister(int startAddress, int registerNumber);
    void updateRegisterValue(quint16 address, quint16 value);
    QModbusDataUnit::RegisterType registerType();
private:
    SAKModbusCommonFlowLayout *mFlowLayout;
    QModbusDataUnit::RegisterType mRegisterType;
    int mReigsterCount;
signals:
    void registerValueChanged(QModbusDataUnit::RegisterType registerType, quint16 address, quint16 value);
    void invokeUpdateRegisterValue(QModbusDataUnit::RegisterType registerTyp, quint16 startAddress, quint16 addressNumber);
private:
    Ui::SAKModbusCommonReigsterView *ui;
};

#endif // SAKMODBUSCOMMONREIGSTERVIEW_HH
