/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QDateTime>
#include <QLineEdit>
#include <QTabWidget>
#include <QModbusDataUnit>
#include <QStandardItemModel>

#include "SAKModbusDebugger.hh"
#include "SAKModbusCommonRegister.hh"
#include "SAKModbusCommonFlowLayout.hh"
#include "SAKModbusCommonController.hh"
#include "SAKModbusCommonRegisterView.hh"
#include "SAKModbusClientControllerTcp.hh"
#include "SAKModbusServerControllerTcp.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"
#include "SAKModbusServerControllerSerialPort.hh"
#include "SAKModbusCommonRegisterViewController.hh"

#include "ui_SAKModbusDebugPage.h"

SAKModbusDebugger::SAKModbusDebugger(QSettings *settings,
                                     const QString settingsGroup,
                                     QSqlDatabase *sqlDatabase,
                                     QWidget *parent)
    :QWidget(parent)
    ,mType(0)
    ,mName("")
    ,mSettings(settings)
    ,mSplashScreen(Q_NULLPTR)
    ,mSqlDatabase(sqlDatabase)
    ,ui(new Ui::SAKModbusDebugPage)
{
    Q_UNUSED(settingsGroup)
    ui->setupUi(this);
    ui->deviceControllerWidget->setContentsMargins(0, 0, 0, 0);
    ui->deviceControllerWidget->setLayout(new QVBoxLayout);
    ui->deviceControllerWidget->layout()->setContentsMargins(0, 0, 0, 0);
    ui->disconnectionPushButton->setEnabled(false);

    // Initialize the settings keys
    const QString group = QString("ModBus");
    mSettingsKeyContext.pageIndex = QString("%1/pageIndex").arg(group);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index){
       mSettings.setValue(mSettingsKeyContext.pageIndex, index) ;
    });

#if 0
    // Add a button to tab bar
    mMenuPushButton = new QPushButton(QString("..."), this);
    ui->tabWidget->setCornerWidget(mMenuPushButton);
#endif
    // Tab page information
    struct PageInfo {QWidget *widget; QModbusDataUnit::RegisterType type;};
    QList<PageInfo> pageInfoList;
    pageInfoList << PageInfo{ui->coilsWidget, QModbusDataUnit::Coils}
                 << PageInfo{ui->discreteInputsWidget, QModbusDataUnit::DiscreteInputs}
                 << PageInfo{ui->holdingRegistersWidget, QModbusDataUnit::HoldingRegisters}
                 << PageInfo{ui->inputRegistersWidget, QModbusDataUnit::InputRegisters};

    // Add page to tab widget
    for (auto &var : pageInfoList){
        var.widget->setContentsMargins(0, 0, 0, 0);
        if (!var.widget->layout()){
            QVBoxLayout *vLayout = new QVBoxLayout;
            var.widget->setLayout(vLayout);
        }
        auto registerView = new SAKModbusCommonRegisterView(var.type);
        mRegisterViewList.append(registerView);
        registerView->setContentsMargins(0, 0, 0, 0);
        auto registerViewController = new SAKModbusCommonRegisterViewController(var.type);
        mRegisterViewControllerList.append(registerViewController);
        connect(registerViewController, &SAKModbusCommonRegisterViewController::invokeUpdateRegister, registerView, &SAKModbusCommonRegisterView::updateRegister);
        connect(registerView, &SAKModbusCommonRegisterView::registerValueChanged, this, &SAKModbusDebugger::setData);
        connect(registerView, &SAKModbusCommonRegisterView::invokeUpdateRegisterValue, this, &SAKModbusDebugger::updateRegisterValue);
        var.widget->layout()->addWidget(registerView);
        var.widget->layout()->addWidget(registerViewController);
    }

    // Combo box items
    struct DeviceInfo {int type; QString name;};
    QList<DeviceInfo> deviceInfoList;
    deviceInfoList << DeviceInfo{TcpClient, tr("Tcp Client")}
                   << DeviceInfo{TcpServer, tr("Tcp Server")}
                   << DeviceInfo{SerialPortClient, tr("SerialPort Client")}
                   << DeviceInfo{SerialPortServer, tr("SerialPort Server")};

    // Add items to combo box
    for (auto &var : deviceInfoList){
        ui->deviceTypeComboBox->addItem(var.name, QVariant::fromValue(var.type));
    }

    // Readin settings
    auto currentPageIndex = mSettings.value(mSettingsKeyContext.pageIndex).toInt();
    ui->tabWidget->setCurrentIndex(currentPageIndex > ui->tabWidget->count() - 1 ? 0 : currentPageIndex);


    struct Info {
        QString name;
        int registerType;
    };
    QList<Info> infoList;
    infoList << Info{tr("Read Coils(0x01)"),
                QModbusDataUnit::Coils}
             << Info{tr("Read Discrete Inputs(0x02)"),
                QModbusDataUnit::DiscreteInputs}
             << Info{tr("Read Holding Registers(0x03)"),
                QModbusDataUnit::HoldingRegisters}
             << Info{tr("Read Input Registers(0x04)"),
                QModbusDataUnit::InputRegisters}
             << Info{tr("Write Coils(0x0F)"),
                QModbusDataUnit::Coils}
             << Info{tr("Write Registers(0x10)"),
                QModbusDataUnit::HoldingRegisters};

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


    connect(ui->functionCodeComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](){
        updateTableWidget();
        updateReadWriteButtonsState();
    });
    connect(ui->startAddressSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](){
        updateTableWidget();
    });
    connect(ui->registerNumberSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](){
        updateTableWidget();
    });
    connect(ui->sendReadRequestPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::sendReadRequest);
    connect(ui->sendWriteRequestPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::sendWriteRequest);
    connect(ui->deviceTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusDebugger::updateController);
    connect(ui->connectionPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::connecteToDevice);
    connect(ui->disconnectionPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::disconnecteDevice);


    updateTableWidget();
    updateController(0);
    updateReadWriteButtonsState();
}

SAKModbusDebugger::~SAKModbusDebugger()
{
    delete ui;
}

QWidget *SAKModbusDebugger::controllerFromType(int type)
{
    QWidget *w = Q_NULLPTR;
    switch (type) {
    case TcpClient:
        w = new SAKModbusClientControllerTcp(this);
        break;
    case TcpServer:
        w = new SAKModbusServerControllerTcp(this);
        break;
    case SerialPortClient:
        w = new SAKModbusClientControllerSerialPort(this);
        break;
    case SerialPortServer:
        w = new SAKModbusServerControllerSerialPort(this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknown device type!");
        break;
    }

    return w;
}

void SAKModbusDebugger::outputModbusDataUnit(QModbusDataUnit mdu)
{
    auto startAddress = mdu.startAddress();
    auto valueCount = mdu.valueCount();
    QList<qint16> valueList;
    for (unsigned int i = startAddress; i < valueCount; i++){
        auto value = mdu.value(i);
        valueList.append(value);
    }

    QByteArray data;
    for (auto &var : valueList){
        data.append(reinterpret_cast<char*>(&var)[1]);
        data.append(reinterpret_cast<char*>(&var)[0]);
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    QString dataStr = QString(data.toHex(' '));
#else
    auto str = QString(data.toHex());
    QString newStr;
    while (str.length() >= 2) {
        newStr.append(str.left(2));
        newStr.append(' ');
        str = str.remove(0, 2);
    }
    newStr = newStr.trimmed();
    QString dataStr = newStr;
#endif

    QString datetime = QDateTime::currentDateTime().toString("hh:mm:ss ");
    datetime = QString("<font color=silver>%1Sending mdu[<%font>").arg(datetime);
    dataStr.prepend(datetime);
    dataStr.append("]");
    ui->textBrowser->append(dataStr);
}

void SAKModbusDebugger::setData(QModbusDataUnit::RegisterType type,
                                quint16 address,
                                quint16 value)
{
    if (mController){
        mController->setData(type, address, value);
    }
}

void SAKModbusDebugger::updateRegisterValue(QModbusDataUnit::RegisterType registerTyp,
                                            quint16 startAddress,
                                            quint16 addressNumber)
{
    auto view = registerView(registerTyp);
    if (view){
        for (quint16 i = startAddress; i < addressNumber; i++){
            auto value = mController->registerValue(registerTyp, startAddress + i);
            view->updateRegisterValue(startAddress + i, value);
        }
    }
}

SAKModbusCommonRegisterView *
SAKModbusDebugger::registerView(QModbusDataUnit::RegisterType registerTyp)
{
    for (int i = 0; i < mRegisterViewList.count(); i++) {
        auto view = mRegisterViewList.at(i);
        if (view->registerType() == registerTyp){
            return view;
        }
    }

    return Q_NULLPTR;
}

void SAKModbusDebugger::dataWritten(QModbusDataUnit::RegisterType table,
                                    int address,
                                    int size)
{
    // The operation is for client only.
    auto server = qobject_cast<QModbusServer*>(mController->device());
    if (server){
        for (int i = address; i < size; i++){
            quint16 value = 0;
            server->data(table, address + i, &value);
            auto view = registerView(table);
            view->updateRegisterValue(address+i, value);
        }
    }

    // The operation is for client only.
    auto clientController = qobject_cast<SAKModbusClientController*>(mController);
    if (clientController){
        for (int i = address; i < size; i++){
            quint16 value = 0;
            clientController->tempData(table, address + i, &value);
            auto view = registerView(table);
            view->updateRegisterValue(address+i, value);
        }
    }
}

void SAKModbusDebugger::outputMessage(QString msg, bool isErrorMsg)
{
    QString datetime = QDateTime::currentDateTime().toString("hh:mm:ss ");
    datetime = QString("<font color=silver>%1<%font>").arg(datetime);
    msg = QString("<font color=%1>%2<%font>")
            .arg(isErrorMsg ? "red" : "black", msg);
    msg.prepend(datetime);
    ui->textBrowser->append(msg);
}

void SAKModbusDebugger::updateTableWidget()
{
    bool isCoilsRegisterType = true;
    if (ui->functionCodeComboBox->currentData().toInt() > QModbusDataUnit::Coils){
        isCoilsRegisterType = false;
    }

    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();

    QTableWidget *tableWidget = ui->tableWidget;
    tableWidget->clear();
    QStringList headerLabels;
    headerLabels << tr("Address")
                 << tr("Value(Bin)")
                 << tr("Value(Otc)")
                 << tr("Value(Dec)")
                 << tr("Value(Hex)");
    tableWidget->setRowCount(number);
    ui->tableWidget->setColumnCount(headerLabels.length());
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    for (int i = 0; i < number; i++){
        QLineEdit *address = new QLineEdit;
        address->setText(QString("%1").arg(QString::number(startAddress + i)));
        address->setReadOnly(true);
        address->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(i, 0, address);

        QLineEdit *valueBin = new QLineEdit(this);
        QLineEdit *valueOtc = new QLineEdit(this);
        QLineEdit *valueDec = new QLineEdit(this);
        QLineEdit *valueHex = new QLineEdit(this);
        valueBin->setReadOnly(true);
        valueOtc->setReadOnly(true);
        valueDec->setReadOnly(true);
        valueHex->setMaxLength(4);

        connect(valueHex, &QLineEdit::textChanged, this, [=](const QString &text){
            int value = text.toInt(Q_NULLPTR, 16);
            QString valueStringBin = QString("%1")
                    .arg(QString::number(value, 2) , 16, '0');
            QString valueStringOtc = QString("%1")
                    .arg(QString::number(value, 8) , 6, '0');
            QString valueStringDec = QString("%1")
                    .arg(QString::number(value, 10));
            valueBin->setText(valueStringBin);
            valueOtc->setText(valueStringOtc);
            valueDec->setText(valueStringDec);
        });

        QVector<QLineEdit*> cellWidgets;
        cellWidgets << valueBin << valueOtc << valueDec << valueHex;
        for (int j = 0; j < cellWidgets.count(); j++) {
            QLineEdit *value = cellWidgets.at(j);
            value->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if (ui->functionCodeComboBox->currentIndex() < 4) {
                if (j == cellWidgets.length() - 1) {
                    value->setReadOnly(true);
                }
                if (isCoilsRegisterType) {
                    value->setText("-");
                } else {
                    value->setText("----");
                }
            } else {
                if (isCoilsRegisterType) {
                    value->setText("0");
                } else {
                    value->setText("0000");
                }
            }

            if (isCoilsRegisterType) {
                if (j == cellWidgets.length() - 1) {
                    value->setMaxLength(1);
                }
                QRegularExpression regExp("[01]");
                value->setValidator(new QRegularExpressionValidator(regExp, this));
                tableWidget->setCellWidget(i, j+1, value);
            } else {
                if (j == cellWidgets.length() - 1) {
                    value->setMaxLength(4);
                }
                QRegularExpression regExp("[a-fA-F0-9]{4}");
                value->setValidator(new QRegularExpressionValidator(regExp, this));
                tableWidget->setCellWidget(i, j+1, value);
            }
        }
    }
}

void SAKModbusDebugger::updateTableWidgetData(QModbusDataUnit mdu)
{
    QList<int> addressList;
    for (uint i = 0; i < mdu.valueCount(); i++) {
        addressList.append(mdu.startAddress() + i);
    }

    auto tableWidget = ui->tableWidget;
    for (int i = 0; i < tableWidget->rowCount(); i++) {
        QWidget *w = tableWidget->cellWidget(i, 0);
        auto address = qobject_cast<QLineEdit*>(w)->text().toInt();
        if (addressList.contains(address)) {
            auto value = mdu.value(i);
            auto valueLineEditHex = qobject_cast<QLineEdit*>(
                        tableWidget->cellWidget(i, 4)
                        );

            if (mdu.registerType() <= QModbusDataUnit::Coils) {
                QString valueString = QString::number(value,  10);
                valueLineEditHex->setText(valueString);
            } else {
                QString valueStringHex = QString("%1")
                        .arg(QString::number(value, 16) , 4, '0').toUpper();
                valueLineEditHex->setText(valueStringHex);
            }
        }
    }
}

void SAKModbusDebugger::sendReadRequest()
{
    const auto registerType = static_cast<QModbusDataUnit::RegisterType>(
                ui->functionCodeComboBox->currentData().toInt());
    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();
    auto mdu = QModbusDataUnit(registerType, startAddress, number);
    auto controller = qobject_cast<SAKModbusClientController*>(mController);
    QModbusClient *device = qobject_cast<QModbusClient*>(controller->device());
    auto reply = device->sendReadRequest(mdu, ui->serverAddressSpinBox->value());
    if (reply){
        outputModbusDataUnit(mdu);
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished,
                    this, [=](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit mdu = reply->result();
                    updateTableWidgetData(mdu);
                    auto registerType = reply->result().registerType();
                    auto startAddress = reply->result().startAddress();
                    auto registerNumber = reply->result().valueCount();
                    outputMessage(QString("<font color=green>%1</font>")
                                  .arg(tr("Register(s) read:")));
                    for (uint i = 0; i < mdu.valueCount(); i++) {
                        auto value = reply->result().value(startAddress + i);
                        setData(registerType, startAddress + i, value);
                        int base = mdu.registerType() <= QModbusDataUnit::Coils ? 10 : 16;
                        const QString entry = tr("[Address: %1, Value: %2]")
                                .arg(mdu.startAddress() + i).
                                arg(QString::number(mdu.value(i), base));
                        outputMessage(entry);
                    }
                    // The signal is using to update ui value.
                    dataWritten(registerType, startAddress, registerNumber);
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    QString error = QString("%1 (Mobus exception: 0x%2)")
                            .arg(reply->errorString())
                            .arg(reply->rawResult().exceptionCode(), 2, 16, QChar('0'));
                    outputMessage(error, true);
                } else {
                    QString error = QString("%1 (code: 0x%2)")
                            .arg(reply->errorString())
                            .arg(reply->error(), 2, 16, QChar('0'));
                    outputMessage(error, true);
                }

                reply->deleteLater();
            });
        } else {
            delete reply; // broadcast replies return immediately
        }
    }else{
        QString devErr = device->errorString();
        QString err = tr("Send reading request error:%1").arg(devErr);
        outputMessage(err);
    }
}

void SAKModbusDebugger::sendWriteRequest()
{
    int rawRegisterType = ui->functionCodeComboBox->currentData().toInt();
    auto registerType = static_cast<QModbusDataUnit::RegisterType>(rawRegisterType);
    int startAddress = ui->startAddressSpinBox->value();
    int number = ui->registerNumberSpinBox->value();

    auto mdu = QModbusDataUnit(registerType, startAddress, number);
    auto tableWidget = ui->tableWidget;
    for (int i = 0; i < number; i++){
        QWidget *w = tableWidget->cellWidget(i, 4);
        int value = qobject_cast<QLineEdit*>(w)->text().toInt(Q_NULLPTR, 16);
        mdu.setValue(i, qint16(value));
    }

    auto controller = qobject_cast<SAKModbusClientController*>(mController);
    QModbusClient *device = qobject_cast<QModbusClient*>(controller->device());
    auto reply = device->sendWriteRequest(mdu, ui->serverAddressSpinBox->value());
    if (reply) {
        outputModbusDataUnit(mdu);
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [=]() {
                if (reply->error() == QModbusDevice::NoError) {
                    outputMessage(QString("<font color=blue>%1</font>")
                                  .arg(tr("Register(s) Written:")));
                    for (uint i = 0; i < mdu.valueCount(); i++) {
                        auto value = reply->result().value(startAddress + i);
                        setData(registerType, startAddress + i, value);
                        int base = mdu.registerType() <= QModbusDataUnit::Coils ? 10 : 16;
                        const QString entry = tr("[Address: %1, Value: %2]")
                                .arg(mdu.startAddress() + i).
                                arg(QString::number(mdu.value(i), base));
                        outputMessage(entry);
                    }
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    QString err = QString("%1 (Mobus exception: 0x%2)")
                            .arg(reply->errorString())
                            .arg(reply->rawResult().exceptionCode(), 2, 16, QChar('0'));
                    outputMessage(err, true);
                } else {
                    QString err = QString("%1 (code: 0x%2)")
                            .arg(reply->errorString())
                            .arg(reply->error(), 2, 16, QChar('0'));
                    outputMessage(err, true);
                }

                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        QString err = tr("Write error: %1").arg(device->errorString());
        outputMessage(err);
    }
}

void SAKModbusDebugger::updateController(int index)
{
    mController = qobject_cast<SAKModbusCommonController*>(controllerFromType(index));
    mController->setContentsMargins(0, 0, 0 ,0);

    auto *dev = mController->device();
    connect(mController, &SAKModbusCommonController::dataWritten,
            this, &SAKModbusDebugger::dataWritten);
    connect(mController, &SAKModbusCommonController::invokeOutputMessage,
            this, &SAKModbusDebugger::outputMessage);
    for (auto &var : mRegisterViewControllerList){
        connect(var, &SAKModbusCommonRegisterViewController::invokeImport,
                mController, &SAKModbusCommonController::importRegisterData);
        connect(var, &SAKModbusCommonRegisterViewController::invokeExport,
                mController, &SAKModbusCommonController::exportRegisterData);
    }
    connect(dev, &QModbusDevice::stateChanged, this, [=](){
        ui->connectionPushButton->setEnabled(
                    dev->state() == QModbusDevice::UnconnectedState
                    );
        ui->disconnectionPushButton->setEnabled(
                    dev->state() == QModbusDevice::ConnectedState
                    );
        ui->deviceTypeComboBox->setEnabled(
                    dev->state() == QModbusDevice::UnconnectedState
                    );
    }, Qt::QueuedConnection);

    QLayout *hLayout = ui->deviceControllerWidget->layout();
    if (hLayout){
        // Remove the old pannel
        QLayoutItem *item = hLayout->takeAt(0);
        if (item){
            delete item->widget();
            delete item;
        }

        // Add a new pannel
        hLayout->addWidget(mController);
        ui->connectionPushButton->setEnabled(true);
    }

    // Update the value of registers.
    quint16 startAddress = 0;
    quint16 registerNumber = 16;
    for (auto &var : mRegisterViewList){
        var->updateRegister(startAddress, registerNumber);
        for (int i = startAddress; i < registerNumber; i++){
            var->updateRegisterValue(startAddress + i,
                                     mController->registerValue(var->registerType(),
                                                                startAddress + i));
        }
    }

    if (index == 0 || index == 2) {
        ui->operationPanelGroupBox->setHidden(false);
        for (int i = 0; i < ui->tabWidget->count(); i++) {
            if (i > 0) {
                ui->tabWidget->setTabEnabled(i, false);
            }
        }
    } else {
        ui->operationPanelGroupBox->setHidden(true);
        for (int i = 0; i < ui->tabWidget->count(); i++) {
            if (i > 0) {
                ui->tabWidget->setTabEnabled(i, true);
            }
        }
    }
}

void SAKModbusDebugger::connecteToDevice()
{
    ui->connectionPushButton->setEnabled(false);
    ui->disconnectionPushButton->setEnabled(false);
    mController->open();
}

void SAKModbusDebugger::disconnecteDevice()
{
    ui->connectionPushButton->setEnabled(false);
    ui->disconnectionPushButton->setEnabled(false);
    mController->closeDevice();
}

void SAKModbusDebugger::updateReadWriteButtonsState()
{
    if (ui->functionCodeComboBox->currentIndex() < 4) {
        ui->sendReadRequestPushButton->setEnabled(true);
        ui->sendWriteRequestPushButton->setEnabled(false);
    } else {
        ui->sendReadRequestPushButton->setEnabled(false);
        ui->sendWriteRequestPushButton->setEnabled(true);
    }
}
