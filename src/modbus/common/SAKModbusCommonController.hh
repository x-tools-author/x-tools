/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONDEVICECONTROLLER_HH
#define SAKMODBUSCOMMONDEVICECONTROLLER_HH

#include <QWidget>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QModbusDevice>
#include <QModbusServer>
#include <QModbusDataUnit>

class SAKModbusCommonController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonController(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonController();

    virtual void open() = 0;
    virtual void setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value) = 0;
    virtual quint16 registerValue(QModbusDataUnit::RegisterType type, quint16 address) = 0;
    virtual void exportRegisterData() = 0;
    virtual void importRegisterData() = 0;

    void closeDevice();
    void appendSection(QWidget *section);
    QModbusDevice *device();
    // You must call the device in the constructor of sublcass.
    void init();
protected:
    virtual QWidget *bottomSection();
    // You should implement this function and do not calling it in the sub class.
    // You can use device() to get the device instance.
    virtual QModbusDevice *initModbusDevice();
    void setModbusServerMap(QModbusServer *server);
    QString getSaveFileName();
    QString getOpenFileName();
    void saveServerRegisterData(QModbusServer *server, QString fileName);
    // The file is the json file.
    void setServerRegisterData(QModbusServer *server, QString fileName);
private:
    QVBoxLayout *mSectionLayout;
    QWidget *mBottomSection;
    QModbusDevice *mDevice;
    quint16 mRegisterNumber;
    QMap<QModbusDataUnit::RegisterType, QString> mInfoMap;
    int mAllRegisterCount;
private:
    QProgressBar *progressBar();
signals:
    void deviceStateChanged();
    void modbusDataUnitRead(QModbusDataUnit mdu);
    void modbusDataUnitWritten(QModbusDataUnit mdu);
    // The signal is for QModbusServer device only
    void dataWritten(QModbusDataUnit::RegisterType table, int address, int size);
    void invokeOutputMessage(QString msg, bool isErrorMsg = false);
};

#endif // SAKMODBUSCOMMONDEVICECONTROLLER_HH
