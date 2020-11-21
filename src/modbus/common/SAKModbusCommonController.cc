/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QLabel>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QFileDialog>
#include <QSizePolicy>
#include <QMessageBox>
#include <QApplication>
#include <QJsonDocument>
#include <QModbusServer>

#include "SAKModbusCommonController.hh"

SAKModbusCommonController::SAKModbusCommonController(QWidget *parent)
    :QWidget(parent)
    ,mBottomSection(Q_NULLPTR)
    ,mDevice(Q_NULLPTR)
    ,mRegisterNumber(65535)
{
    mSectionLayout = new QVBoxLayout(this);
    mSectionLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
    setLayout(mSectionLayout);

    mInfoMap.insert(QModbusDataUnit::Coils, "Coils");
    mInfoMap.insert(QModbusDataUnit::DiscreteInputs, "DiscreteInputs");
    mInfoMap.insert(QModbusDataUnit::InputRegisters, "InputRegisters");
    mInfoMap.insert(QModbusDataUnit::HoldingRegisters, "HoldingRegisters");
    mAllRegisterCount = mInfoMap.count()*mRegisterNumber;
}

SAKModbusCommonController::~SAKModbusCommonController()
{
    closeDevice();
    if (device()){
        delete device();
    }
}

void SAKModbusCommonController::closeDevice()
{
    if (device()){
        device()->disconnectDevice();
    }
}

void SAKModbusCommonController::appendSection(QWidget *section)
{
    Q_ASSERT_X(section, __FUNCTION__, "Parameter can not be null!");
    mSectionLayout->addWidget(section);
    if (bottomSection() && mSectionLayout){
        mSectionLayout->addWidget(bottomSection());
    }
}

QModbusDevice *SAKModbusCommonController::device()
{
    return mDevice;
}

void SAKModbusCommonController::init()
{
    mDevice = initModbusDevice();
    mDevice->setParent(this);
    Q_ASSERT_X(mDevice, __FUNCTION__, "The value returned can not be null!");

    auto server = qobject_cast<QModbusServer*>(mDevice);
    if (server){
        connect(server, &QModbusServer::dataWritten, this, &SAKModbusCommonController::dataWritten);
        setModbusServerMap(server);
    }

    if (mDevice){
        connect(mDevice, &QModbusDevice::errorOccurred, this, [=](){
            QMessageBox::warning(this, tr("Error Occurred"), tr("Error Occured:%1 Please check the parameters and try again!").arg(mDevice->errorString()));
            emit invokeOutputMessage(tr("Error occurred:%1").arg(mDevice->errorString()), true);
        });

        connect(mDevice, &QModbusDevice::stateChanged, this, [=](QModbusDevice::State state){
            if (state == QModbusDevice::ConnectedState){
                emit invokeOutputMessage(tr("Modbus device is connected!"));
            }else if (state == QModbusDevice::ConnectingState){
                emit invokeOutputMessage(tr("Modbus device is connecting!"));
            }else if (state == QModbusDevice::ClosingState){
                emit invokeOutputMessage(tr("Modbus device is closing!"));
            }else if (state == QModbusDevice::UnconnectedState){
                emit invokeOutputMessage(tr("Modbus device is unconnected!"));
            }else{
                emit invokeOutputMessage(tr("Unknownn state(%1) of modbus device!").arg(mDevice->state()));
            }
        }, Qt::QueuedConnection);
    }
}

QWidget *SAKModbusCommonController::bottomSection()
{
    return Q_NULLPTR;
}

QModbusDevice *SAKModbusCommonController::initModbusDevice()
{
    return Q_NULLPTR;
}

void SAKModbusCommonController::setModbusServerMap(QModbusServer *server)
{
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils, {QModbusDataUnit::Coils, 0, mRegisterNumber});
    reg.insert(QModbusDataUnit::DiscreteInputs, {QModbusDataUnit::DiscreteInputs, 0, mRegisterNumber});
    reg.insert(QModbusDataUnit::InputRegisters, {QModbusDataUnit::InputRegisters, 0, mRegisterNumber});
    reg.insert(QModbusDataUnit::HoldingRegisters, {QModbusDataUnit::HoldingRegisters, 0, mRegisterNumber});
    server->setMap(reg);
    for (int i = 0; i < mRegisterNumber; i++){
        server->setData(QModbusDataUnit::Coils, i, false);
        server->setData(QModbusDataUnit::DiscreteInputs, i, false);
        server->setData(QModbusDataUnit::InputRegisters, i, 0xffff);
        server->setData(QModbusDataUnit::HoldingRegisters, i, 0xffff);
    }
}

QString SAKModbusCommonController::getSaveFileName()
{
    QString defaultFileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").append(".json").prepend("./");
    auto fileName = QFileDialog::getSaveFileName(this, tr("Export Register Data"), defaultFileName, QString("Json (*.json *.txt)"));
    return fileName;
}

QString SAKModbusCommonController::getOpenFileName()
{
    QString defaultFileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss").append(".json").prepend("./");
    auto fileName = QFileDialog::getOpenFileName(this, tr("Export Register Data"), defaultFileName, QString("Json (*.json *.txt)"));
    return fileName;
}

void SAKModbusCommonController::saveServerRegisterData(QModbusServer *server, QString fileName)
{
    Q_ASSERT_X(server, __FUNCTION__, "The parameter can not be null!");
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)){
        QMapIterator<QModbusDataUnit::RegisterType, QString> mapIterator(mInfoMap);
        QJsonObject jsonObj;
        QProgressBar *pb = progressBar();
        int pValue = 0;
        int percent = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QModbusDataUnit::RegisterType type = mapIterator.key();
            QString typeName = mapIterator.value();
             QJsonArray jsonArr;
            for (int i = 0; i < mRegisterNumber; i++){
                quint16 value = 0;
                server->data(type, i, &value);
                jsonArr.append(value);
                pValue += 1;
                int pTemp = pValue*100/mAllRegisterCount;
                if (pTemp != percent){
                    pb->setValue(pValue);
                    qApp->processEvents();
                    percent = pTemp;
                }
            }
            jsonObj.insert(typeName, jsonArr);
        }
        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObj);
        QTextStream out(&file);
        out << jsonDoc.toJson();
        file.close();

        pb->close();
        delete pb->parent();
#if 0
        QMessageBox::information(this, tr("Export Data Successfully"), tr("The data was exported successfully!"));
#endif
    }else{
        qWarning() << "Can not open the file(" << fileName << ")" << file.errorString();
        QMessageBox::warning(this, tr("Export Data Failed"), tr("The data was exported failed:%1").arg(file.errorString()));
    }
}

void SAKModbusCommonController::setServerRegisterData(QModbusServer *server, QString fileName)
{
    Q_ASSERT_X(server, __FUNCTION__, "The parameter can not be null!");
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)){
        QByteArray json = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
        if (jsonDoc.isObject()){
            QJsonObject jsonObj = jsonDoc.object();
            QMapIterator<QModbusDataUnit::RegisterType, QString> mapIterator(mInfoMap);
            QProgressBar *pb = progressBar();
            int pValue = 0;
            int percent = 0;
            while (mapIterator.hasNext()) {
                mapIterator.next();
                QModbusDataUnit::RegisterType type = mapIterator.key();
                QString typeName = mapIterator.value();
                QJsonArray jsonArr = jsonObj.value(typeName).toArray();
                auto arrayCount = jsonArr.count();
                for (int i = 0; (i < arrayCount) && (i < mRegisterNumber); i++){
                    auto value = quint16(jsonArr.at(i).toInt());
                    server->setData(type, i, value);
                    pValue += 1;
                    int pTemp = pValue*100/mAllRegisterCount;
                    if (pTemp != percent){
                        pb->setValue(pValue);
                        qApp->processEvents();
                        percent = pTemp;
                    }
                }
                emit dataWritten(type, 0, arrayCount > mRegisterNumber ? mRegisterNumber : arrayCount);
            }
            pb->close();
            delete pb->parent();
#if 0
            QMessageBox::information(this, tr("Import Data Successfully"), tr("The data was imported successfully!"));
#endif
        }
    }else{
        QMessageBox::warning(this, tr("Import Data Failed"), tr("The data was imported failed:%1").arg(file.errorString()));
        qWarning() << "Can not open the file(" << fileName << ")" << file.errorString();
    }
}

QProgressBar *SAKModbusCommonController::progressBar()
{
    QDialog *dialog = new QDialog;;
    QVBoxLayout *vLayout = new QVBoxLayout(dialog);
    QProgressBar *progressBar = new QProgressBar(dialog);
    progressBar->setMinimum(0);
    progressBar->setMaximum(mAllRegisterCount);
    vLayout->addWidget(progressBar);
    dialog->setWindowFlags(Qt::FramelessWindowHint);
    dialog->setModal(true);
    dialog->show();
    return progressBar;
}
