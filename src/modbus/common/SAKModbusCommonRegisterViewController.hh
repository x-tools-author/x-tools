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
#include <QModbusDataUnit>

namespace Ui {
    class SAKModbusCommonRegisterViewController;
}

class SAKModbusCommonRegisterViewController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonRegisterViewController(QModbusDataUnit::RegisterType type, QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonRegisterViewController();
    QModbusDataUnit::RegisterType registerType();
private:
    QModbusDataUnit::RegisterType mRegisterType;
signals:
    void invokeUpdateRegister(int startAddress, int registerNumber);
    void invokeExport();
    void invokeImport();
private:
    Ui::SAKModbusCommonRegisterViewController *ui;
private slots:
    void on_updatePushButton_clicked();
    void on_exportPushButton_clicked();
    void on_importPushButton_clicked();
};

#endif // SAKMODBUSCOMMONREIGSTERVIEWCONTROLLER_HH
