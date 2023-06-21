/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMenu>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QWidgetAction>

#include "SAKSpinBox.hh"
#include "SAKLineEdit.hh"
#include "SAKBleCentralToolUi.hh"
#include "ui_SAKBleCentralToolUi.h"

#define SAK_CB_I_C &QComboBox::currentIndexChanged

SAKBleCentralToolUi::SAKBleCentralToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKBleCentralToolUi)
    , mBleTool(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->pushButtonScan, &QPushButton::clicked,
            this, &SAKBleCentralToolUi::onPushButtonScanClicked);
    connect(ui->comboBoxDevices,
            static_cast<void(QComboBox::*)(int)>(SAK_CB_I_C),
            this,
            &SAKBleCentralToolUi::onComboBoxDevicesActived);
    connect(ui->comboBoxServices,
            static_cast<void(QComboBox::*)(int)>(SAK_CB_I_C),
            this, &SAKBleCentralToolUi::onComboBoxServicesCurrentIndexChanged);
    connect(ui->comboBoxServices,
            static_cast<void(QComboBox::*)(int)>(SAK_CB_I_C),
            this, &SAKBleCentralToolUi::onComboBoxCharacteristicsActived);
}

SAKBleCentralToolUi::~SAKBleCentralToolUi()
{
    delete ui;
}

void SAKBleCentralToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                  const QString &settingsGroup)
{
    SAKCommunicationToolUi::onBaseToolUiInitialized(tool, settingsGroup);
    initSettingsMenu(settingsGroup);
}

void SAKBleCentralToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
    // Nothing to do yet...
}

void SAKBleCentralToolUi::initSettingsMenu(const QString &settingsGroup)
{
    QWidget *w = new QWidget(this);
    QGridLayout *gl = new QGridLayout();
    w->setLayout(gl);

    int rowIndex = 0;
    gl->addWidget(new QLabel(tr("Timeout interval(S)"), w), rowIndex, 0, 1, 1);
    SAKSpinBox *sp = new SAKSpinBox(w);
    sp->setMinimum(10);
    sp->setMaximum(120);
    sp->setValue(120);
    sp->setGroupKey(settingsGroup, "timeoutInterval");
    gl->addWidget(sp, rowIndex, 1, 1, 1);
    connect(sp, &SAKSpinBox::valueChanged, this, [=](int v){
        ui->comboBoxDevices->setTimeoutInterval(v);
    });

    rowIndex += 1;
    gl->addWidget(new QLabel(tr("Name filtter"), w), rowIndex, 0, 1, 1);
    SAKLineEdit *le = new SAKLineEdit(w);
    le->setGroupKey(settingsGroup, "nameFiltter");
    gl->addWidget(le, rowIndex, 1, 1, 1);
    connect(le, &SAKLineEdit::textChanged, this, [=](const QString &text){
        ui->comboBoxDevices->setNameFiltter(text);
    });

    rowIndex += 1;
    QMenu *menu = new QMenu(this);
    QPushButton *bt = new QPushButton(tr("OK"));
    connect(bt, &QPushButton::clicked, menu, &QMenu::close);
    gl->addWidget(bt, rowIndex, 1, 1, 1);

    QWidgetAction *a = new QWidgetAction(this);
    a->setDefaultWidget(w);;
    menu->addAction(a);
    ui->pushButtonSettings->setMenu(menu);

    mBleTool = qobject_cast<SAKBleCentralTool*>(mTool);
    if (!mBleTool) {
        qCWarning((*mLoggingCategory)) << "invalid SAKBleCentralTool tool";
        return;
    }
    connect(mBleTool, &SAKBleCentralTool::serviceDiscoveryStarted,
            this, &SAKBleCentralToolUi::onServiceDiscoveryStarted);
    connect(mBleTool, &SAKBleCentralTool::serviceDiscoveryFinished,
            this, &SAKBleCentralToolUi::onServiceDiscoveryFinished);

    int timeoutInterval = sp->value();
    QString nameFiltter = le->text().trimmed();
    ui->comboBoxDevices->setTimeoutInterval(timeoutInterval);
    ui->comboBoxDevices->setNameFiltter(nameFiltter);

    onComboBoxDevicesActived();
}

void SAKBleCentralToolUi::onServiceDiscoveryStarted()
{
    ui->progressBar->show();
}

void SAKBleCentralToolUi::onServiceDiscoveryFinished()
{
    ui->comboBoxServices->clear();
    auto services = mBleTool->services();
    for (auto &service : services) {
        auto cookedSerivce = service.value<QLowEnergyService*>();
        ui->comboBoxServices->addItem(cookedSerivce->serviceName(), service);
        connect(cookedSerivce, &QLowEnergyService::stateChanged,
                this, [=](QLowEnergyService::ServiceState state){
            qDebug() << state;
            if (state == QLowEnergyService::RemoteServiceDiscovered) {
                onComboBoxServicesCurrentIndexChanged();
            }
        });
    }
    ui->progressBar->hide();
}

void SAKBleCentralToolUi::onPushButtonScanClicked()
{
    if (ui->comboBoxDevices->isActive()) {
        ui->comboBoxDevices->stopDiscover();
        ui->pushButtonScan->setText(tr("Scan"));
    } else {
        ui->comboBoxDevices->startDiscover();
        ui->pushButtonScan->setText(tr("Stop"));
    }
}

void SAKBleCentralToolUi::onComboBoxDevicesActived()
{
    QVariant data = ui->comboBoxDevices->currentData();
    mBleTool->setInfo(data);
}

void SAKBleCentralToolUi::onComboBoxServicesCurrentIndexChanged()
{
    mBleTool->setServiceIndex(ui->comboBoxServices->currentIndex());
    auto service = ui->comboBoxServices->currentData();
    auto cookedSerivce = service.value<QLowEnergyService*>();
    auto chs = cookedSerivce->characteristics();
    ui->comboBoxCharacteristics->clear();
    for (auto &ch : chs) {
        QVariant var = QVariant::fromValue<QLowEnergyCharacteristic>(ch);
        QString name = ch.name();
        name = name.isEmpty() ? "(null)" : name;
        ui->comboBoxCharacteristics->addItem(name, var);
    }
}

void SAKBleCentralToolUi::onComboBoxCharacteristicsActived()
{
    int index = ui->comboBoxCharacteristics->currentIndex();
    mBleTool->setCharacteristicIndex(index);
}
