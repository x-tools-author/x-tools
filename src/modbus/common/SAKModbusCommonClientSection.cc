/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QSpinBox>
#include <QComboBox>
#include <QModbusDevice>
#include <QModbusDataUnit>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

#include "SAKModbusCommonClientSection.hh"
#include "ui_SAKModbusCommonClientSection.h"

SAKModbusCommonClientSection::SAKModbusCommonClientSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonClientSection)
{
    ui->setupUi(this);
    mFunctionCodeSpinBox = ui->functionCodeComboBox;
    mStartAddressSpinBox = ui->startAddressSpinBox;
    mRegisterNumberLineEdit = ui->registerNumberSpinBox;
    mClientAddressSpinBox = ui->clientAddressSpinBox;
    mReadWriteValuesTableWidget = ui->readWriteValuesTableWidget;
    ui->readWriteValuesTableWidget->setColumnCount(2);
    ui->readWriteValuesTableWidget->verticalHeader()->setVisible(false);
    ui->readWriteValuesTableWidget->horizontalHeader()->setVisible(false);
    ui->readWriteValuesTableWidget->setContentsMargins(6, 6, 6, 6);
    ui->readWriteValuesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    struct Info {
        QString name;
        int registerType;
    };

    QList<Info> infoList;
    infoList << Info{tr("Read Coils(0x01)"), QModbusDataUnit::Coils}
             << Info{tr("Read Discrete Inputs(0x02)"), QModbusDataUnit::DiscreteInputs}
             << Info{tr("Read Holding Registers(0x03)"), QModbusDataUnit::HoldingRegisters}
             << Info{tr("Read Input Registers(0x04)"), QModbusDataUnit::InputRegisters}
             << Info{tr("Write Coils(0x0F)"), QModbusDataUnit::Coils}
             << Info{tr("Write Registers(0x10)"), QModbusDataUnit::HoldingRegisters};

    QStandardItemModel *itemModel = new QStandardItemModel(this);
    ui->functionCodeComboBox->clear();
    for (auto &var : infoList){
        auto item = new QStandardItem(var.name);
        item->setToolTip(var.name);
        itemModel->appendRow(item);
    }
    ui->functionCodeComboBox->setModel(itemModel);
    for (int i = 0; i < infoList.length(); i++){
        ui->functionCodeComboBox->setItemData(i, infoList.at(i).registerType);
    }
}

SAKModbusCommonClientSection::~SAKModbusCommonClientSection()
{
    delete ui;
}

SAKModbusCommonClientSection::ParametersContext SAKModbusCommonClientSection::parametersContext()
{
    ParametersContext parasCtx;
    parasCtx.numberOfRetries = ui->retriesSpinBox->value();
    parasCtx.timeout = ui->timeoutSpinBox->value();
    return parasCtx;
}

void SAKModbusCommonClientSection::setUiEnable(bool isUnconnected)
{
    ui->timeoutSpinBox->setEnabled(isUnconnected);
    ui->retriesSpinBox->setEnabled(isUnconnected);
}

int SAKModbusCommonClientSection::slaveAddress()
{
    return ui->clientAddressSpinBox->value();
}

void SAKModbusCommonClientSection::updateTableWidget(QModbusDataUnit mdu)
{
    QList<int> addressList;
    for (uint i = 0; i < mdu.valueCount(); i++) {
        addressList.append(mdu.startAddress() + i);
    }

    auto tableWidget = ui->readWriteValuesTableWidget;
    for (int i = 0; i < tableWidget->rowCount(); i++){
        auto address = qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 0))->text().toInt();
        if (addressList.contains(address)){
            auto value = mdu.value(i);
            auto valueLineEdit = qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 1));
            QString valueString;
            if (mdu.registerType() <= QModbusDataUnit::Coils){
                valueString = QString::number(value,  10);
            }else{
                valueString = QString("%1").arg(QString::number(value, 16) , 4, '0').toUpper();
            }
            valueLineEdit->setText(valueString);
        }
    }
}

void SAKModbusCommonClientSection::initModbusClientParameters(QModbusClient *client)
{
    if (client){
        auto clientParacCtx = parametersContext();
        client->setNumberOfRetries(clientParacCtx.numberOfRetries);
        client->setTimeout(clientParacCtx.timeout);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Parameter can not be null!");
    }
}

void SAKModbusCommonClientSection::updateTableWidget()
{
    bool isCoilsRegisterType = true;
    if (ui->functionCodeComboBox->currentData().toInt() > QModbusDataUnit::Coils){
        isCoilsRegisterType = false;
    }

    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();

    QTableWidget *tableWidget = ui->readWriteValuesTableWidget;
    tableWidget->clear();
    tableWidget->setRowCount(number);
    for (int i = 0; i < number; i++){
        QLineEdit *address = new QLineEdit;
        address->setText(QString("%1").arg(startAddress+i));
        address->setReadOnly(true);
        address->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(i, 0, address);

        QLineEdit *value = new QLineEdit;
        value->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        if (ui->functionCodeComboBox->currentIndex() < 4){
            value->setReadOnly(true);
            if (isCoilsRegisterType){
                value->setText("-");
            }else{
                value->setText("----");
            }
        }else{
            if (isCoilsRegisterType){
                value->setText("0");
            }else{
                value->setText("0000");
            }
        }

        if (isCoilsRegisterType){
            value->setMaxLength(1);
            QRegularExpression regExp("[01]");
            value->setValidator(new QRegularExpressionValidator(regExp, this));
            tableWidget->setCellWidget(i, 1, value);
        }else{
            value->setMaxLength(4);
            QRegularExpression regExp("[a-fA-F0-9]{4}");
            value->setValidator(new QRegularExpressionValidator(regExp, this));
            tableWidget->setCellWidget(i, 1, value);
        }
    }

    tableWidget->resizeRowsToContents();
    tableWidget->resizeColumnsToContents();
}

void SAKModbusCommonClientSection::on_functionCodeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateTableWidget();
    if (ui->functionCodeComboBox->currentIndex() < 4){
        ui->sendReadRequestPushButton->setEnabled(true);
        ui->sendWriteRequestPushButton->setEnabled(false);
    }else{
        ui->sendReadRequestPushButton->setEnabled(false);
        ui->sendWriteRequestPushButton->setEnabled(true);
    }
}

void SAKModbusCommonClientSection::on_clientAddressSpinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateTableWidget();
}

void SAKModbusCommonClientSection::on_startAddressSpinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateTableWidget();
}

void SAKModbusCommonClientSection::on_registerNumberSpinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateTableWidget();
}

void SAKModbusCommonClientSection::on_sendReadRequestPushButton_clicked()
{
    const auto registerType = static_cast<QModbusDataUnit::RegisterType>(ui->functionCodeComboBox->currentData().toInt());
    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();
    auto mdu = QModbusDataUnit(registerType, startAddress, number);
    emit invokeSendReadRequest(mdu);
}

void SAKModbusCommonClientSection::on_sendWriteRequestPushButton_clicked()
{
    const auto registerType = static_cast<QModbusDataUnit::RegisterType>(ui->functionCodeComboBox->currentData().toInt());
    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();

    auto mdu = QModbusDataUnit(registerType, startAddress, number);
    auto tableWidget = ui->readWriteValuesTableWidget;
    for (int i = 0; i < number; i++){
        if (registerType == QModbusDataUnit::Coils){
            mdu.setValue(i, qint16(qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 1))->text().toInt(Q_NULLPTR, 10)));
        }else{
            mdu.setValue(i, qint16( qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 1))->text().toInt(Q_NULLPTR, 16)));
        }
    }
    emit invokrSendWriteRequest(mdu);
}
