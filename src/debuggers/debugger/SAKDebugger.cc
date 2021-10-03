/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QSqlQuery>
#include <QDateTime>
#include <QSettings>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QScrollBar>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QIntValidator>
#include <QLoggingCategory>
#include <QStandardItemModel>

#include "SAKDebugger.hh"
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKDebuggerInput.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerOutput.hh"
#include "SAKDebuggerPlugins.hh"
#include "SAKDebuggerStatistics.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerController.hh"

#ifdef SAK_IMPORT_MODULE_CHARTS
#include "SAKPluginCharts.hh"
#endif

#include "ui_SAKDebugger.h"

SAKDebugger::SAKDebugger(QSettings *settings,
                         const QString settingsGroup,
                         QSqlDatabase *sqlDatabase,
                         QWidget *parent)
    :QWidget(parent)
    ,mModuleDevice(Q_NULLPTR)
    ,mModuleController(Q_NULLPTR)
    ,mUi(new Ui::SAKDebugger)
{
    mUi->setupUi(this);
    auto html = mUi->outputTextBroswer->toHtml();
    html = html.replace(QString("1970"), sakApp->buildDate()->toString("yyyy"));
    html = html.replace(QString("Author"), QString(SAK_AUTHOR));
    html = html.replace(QString("Email"), QString(SAK_AUTHOR_EMAIL));
    mUi->outputTextBroswer->setHtml(html);


    mModulePlugins = new SAKDebuggerPlugins(
                mUi->managerPushButton,
                mUi->pluginsPushButton,
                settings,
                settingsGroup,
                mUi->pluginPanelLabel,
                sqlDatabase,
                mUi->pluginPanelWidget,
                this
                );
    mModuleOutput = new SAKDebuggerOutput(
                mUi->moreOutputSettingsPushButton,
                mUi->outputTextFormatComboBox,
                settings,
                settingsGroup,
                mUi->outputTextBroswer,
                this,
                this
                );
    mModuleStatistics = new SAKDebuggerStatistics(
                mUi->txSpeedLabel,
                mUi->rxSpeedLabel,
                mUi->txFramesLabel,
                mUi->rxFramesLabel,
                mUi->txBytesLabel,
                mUi->rxBytesLabel,
                this
                );
    mModuleInput = new SAKDebuggerInput(
                mUi->cyclingTimeComboBox,
                mUi->inputFormatComboBox,
                mUi->moreInputSettingsPushButton,
                mUi->sendPushButton,
                mUi->crcLabel,
                mUi->inputComboBox,
                settings,
                settingsGroup,
                sqlDatabase,
                this,
                this
                );
    mModuleInput->updateUiState(false);

    // Initialize timer.
    mClearInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    mClearInfoTimer.setSingleShot(true);
    connect(&mClearInfoTimer, &QTimer::timeout,
            mUi->infoLabel, &QLabel::clear);


    // Initialize menu
    mDeviceMenu = new QMenu(mUi->deviceMorePushButton);
    mUi->deviceMorePushButton->setMenu(mDeviceMenu);
    mRefreshAction = mDeviceMenu->addAction(tr("Refresh"), this, [=](){
        if (mModuleController){
            mModuleController->refreshDevice();
        }else{
            Q_ASSERT_X(false, __FUNCTION__, "A null pointer!");
        }
    });


    // Open or close device.
    connect(mUi->switchPushButton, &QPushButton::clicked,
            this, [=](){
        mUi->switchPushButton->setEnabled(false);
        if (this->device()){
            if (!this->device()->isRunning()){
                mModuleDevice->start();
            }else{
                mModuleDevice->exit();
                mModuleDevice->wait();
            }
        }
    });
}

SAKDebugger::~SAKDebugger()
{
    delete mUi;
    if (mModuleDevice) {
        if (mModuleDevice->isRunning()){
            mModuleDevice->exit();
            mModuleDevice->wait();
        }
        mModuleDevice->deleteLater();
    }
}

void SAKDebugger::initDebugger()
{
    initDebuggerDevice();
    initDebuggerController();
    initDebuggerStatistics();
    initDebuggerOutout();
    initDebuggerInput();
    initDebuggerPlugin();
}

void SAKDebugger::initDebuggerController()
{
    mModuleController = controller();
    if (mModuleController){
        QHBoxLayout *layout = new QHBoxLayout(mUi->controllerWidget);
        mUi->controllerWidget->setLayout(layout);
        layout->addWidget(mModuleController);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    connect(mModuleController, &SAKDebuggerController::parametersContextChanged,
            this, [=](){
        mModuleDevice->setParametersContext(mModuleController->parametersContext());
    });
    mModuleDevice->setParametersContext(mModuleController->parametersContext());
}

void SAKDebugger::initDebuggerDevice()
{
    mModuleDevice = device();
    Q_ASSERT_X(mModuleDevice,
               __FUNCTION__,
               "You must initialize the mDevice in the subcalss!");
    mModuleDevice->setupMenu(mDeviceMenu);
    auto updateUiState = [=](bool opened){
        mModuleInput->updateUiState(opened);
        mRefreshAction->setEnabled(!opened);
        mModuleController->updateUiState(opened);
        mUi->switchPushButton->setEnabled(true);
    };
    connect(mModuleDevice, &SAKDebuggerDevice::errorOccurred,
            this, [=](QString error){
        QString time = QDateTime::currentDateTime().toString("hh:mm:ss ");
        QString temp;
        temp.append(time);
        temp.append(error);
        time = QString("<font color=silver>%1</font>").arg(time);
        QApplication::beep();

        error.prepend(time);
        mUi->infoLabel->setText(error);
        mClearInfoTimer.start();
        mUi->switchPushButton->setEnabled(true);
        updateUiState(false);
    });
    connect(mModuleDevice, &SAKDebuggerDevice::started,
            this, [=](){
        mUi->switchPushButton->setText(tr("Close"));
        updateUiState(true);
    });
    connect(mModuleDevice, &SAKDebuggerDevice::finished,
            this, [=](){
        mUi->switchPushButton->setText(tr("Open"));
        updateUiState(false);
    });
}

void SAKDebugger::initDebuggerStatistics()
{
    connect(mModuleDevice, &SAKDebuggerDevice::bytesRead,
            mModuleStatistics, &SAKDebuggerStatistics::onBytesRead);
    connect(mModuleDevice, &SAKDebuggerDevice::bytesWritten,
            mModuleStatistics, &SAKDebuggerStatistics::onBytesWritten);
}

void SAKDebugger::initDebuggerOutout()
{
    connect(mModuleDevice, &SAKDebuggerDevice::bytesRead,
            mModuleOutput, &SAKDebuggerOutput::onBytesRead);
    connect(mModuleDevice, &SAKDebuggerDevice::bytesWritten,
            mModuleOutput, &SAKDebuggerOutput::onBytesWritten);
}

void SAKDebugger::initDebuggerInput()
{
    connect(mModuleInput, &SAKDebuggerInput::invokeWriteBytes,
            mModuleDevice, &SAKDebuggerDevice::writeBytes, Qt::QueuedConnection);
}

void SAKDebugger::initDebuggerPlugin()
{
    connect(mModuleDevice, &SAKDebuggerDevice::bytesRead,
            mModulePlugins, &SAKDebuggerPlugins::bytesRead);
    connect(mModuleDevice, &SAKDebuggerDevice::bytesWritten,
            mModulePlugins, &SAKDebuggerPlugins::bytesWritten);
    connect(mModulePlugins, &SAKDebuggerPlugins::invokeWriteCookedBytes,
            mModuleDevice, &SAKDebuggerDevice::writeBytes);
}

void SAKDebugger::commonSqlApiUpdateRecord(QSqlQuery *sqlQuery,
                                           QString tableName,
                                           QString columnName,
                                           QVariant value,
                                           quint64 recordID,
                                           bool valueIsString)
{
    QString queryString;
    if (valueIsString){
        queryString = QString("UPDATE %1 SET %2='%3' WHERE ID=%4")
                .arg(tableName,
                     columnName,
                     value.toString(),
                     QString::number(recordID));
    }else{
        queryString = QString("UPDATE %1 SET %2=%3 WHERE ID=%4")
                .arg(tableName,
                     columnName,
                     value.toString(),
                     QString::number(recordID));
    }

    if(!sqlQuery->exec(queryString)){
        qWarning() << QString("Can not update record(%1):%2")
                      .arg(columnName, sqlQuery->lastError().text());
    }
}

void SAKDebugger::commonSqlApiDeleteRecord(QSqlQuery *sqlQuery,
                                           QString tableName,
                                           quint64 recordID)
{
    QString queryString = QString("DELETE FROM %1 WHERE ID=%2")
            .arg(tableName)
            .arg(recordID);
    if (!sqlQuery->exec(queryString)) {
        qWarning() << "Can not delete recored form(" << tableName << ")"
                   << sqlQuery->lastError().text();
    }
}

int SAKDebugger::clearDataMessageBox()
{
    QString title = tr("Clear Data");
    QString text = tr("All Data Items Will Be Deleted!")
            + " "
            + tr("Are you sure you want to do this?");
    auto ret = QMessageBox::warning(sakMainWindow,
                                    title,
                                    text,
                                    QMessageBox::Ok|QMessageBox::Cancel);
    return ret;
}
