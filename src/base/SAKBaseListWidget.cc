/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QJsonArray>
#include <QMessageBox>
#include <QListWidget>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKBaseListWidget.hh"
#include "ui_SAKBaseListWidget.h"
#include "SAKBaseListWidgetItemWidget.hh"

SAKBaseListWidget::SAKBaseListWidget(QSqlDatabase *sqlDatabase,
                                     QSettings *settings,
                                     QString settingsGroup,
                                     QString tableNameSuffix,
                                     QWidget *parent)
    :QWidget(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mTableNameSuffix(tableNameSuffix)
    ,mTableName(settingsGroup + tableNameSuffix)
    ,mUi(new Ui::SAKBaseListWidget)
{
    mSqlQuery = QSqlQuery(*sqlDatabase);
    mUi->setupUi(this);
    mListWidget = mUi->itemListWidget;


    mClearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearMessageInfoTimer, &QTimer::timeout, this, [&](){
        mClearMessageInfoTimer.stop();
        mUi->infoLabel->clear();
    });


    connect(mUi->clearPushButton, &QPushButton::clicked,
            this, &SAKBaseListWidget::clearItems);
    connect(mUi->exportPushButton, &QPushButton::clicked,
            this, &SAKBaseListWidget::exportItems);
    connect(mUi->importPushButton, &QPushButton::clicked,
            this, &SAKBaseListWidget::importItems);
    connect(mUi->deletePushButton, &QPushButton::clicked,
            this, [=](){
        QListWidgetItem *item = mListWidget->currentItem();
        deleteItem(item);
    });
    connect(mUi->addPushButton, &QPushButton::clicked,
            this, &SAKBaseListWidget::addItem);
    setContentsMargins(0, 0, 0, 0);


    mForbidAllItemsSettingsKey =
            settingsGroup + "/" + tableNameSuffix + "/forbidAllItems";
    if (mSettings->value(mForbidAllItemsSettingsKey).isNull()) {
        mUi->forbidAllItemsCheckBox->setChecked(true);
    } else {
        bool ret = mSettings->value(mForbidAllItemsSettingsKey).toBool();
        mUi->forbidAllItemsCheckBox->setChecked(ret);
    }
    connect(mUi->forbidAllItemsCheckBox, &QCheckBox::clicked,
            this, [&](){
        mSettings->setValue(mForbidAllItemsSettingsKey,
                            mUi->forbidAllItemsCheckBox->isChecked());
    });
}

SAKBaseListWidget::~SAKBaseListWidget()
{
    delete mUi;
}

void SAKBaseListWidget::onBytesRead(QByteArray bytes)
{
    if (mUi->forbidAllItemsCheckBox->isChecked()) {
        return;
    } else {
        emit bytesRead(bytes);
    }
}

bool SAKBaseListWidget::forbidAllItems()
{
    return mUi->forbidAllItemsCheckBox->isChecked();
}

void SAKBaseListWidget::updateRecord(quint64 id, QString columnName, QVariant value)
{
    QString queryString;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (value.type() == QVariant::String) {
#else
    if (value.metaType() == QMetaType(QMetaType::QString)) {
#endif
        queryString = QString("UPDATE %1 SET %2='%3' WHERE ID=%4")
                .arg(mTableName,
                     columnName,
                     value.toString(),
                     QString::number(id));
    } else {
        queryString = QString("UPDATE %1 SET %2=%3 WHERE ID=%4")
                .arg(mTableName,
                     columnName,
                     value.toString(),
                     QString::number(id));
    }

    if(!mSqlQuery.exec(queryString)) {
        qInfo() << queryString;
        qWarning() << QString("Can not update record(%1):%2")
                      .arg(columnName, mSqlQuery.lastError().text());
    }
}

void SAKBaseListWidget::outputMessage(QString msg, bool isError)
{
    QString color = "black";
    if (isError){
        color = "red";
        QApplication::beep();
    }
    mUi->infoLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    mUi->infoLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    mClearMessageInfoTimer.start();
}

void SAKBaseListWidget::clearItems()
{
    QString title = tr("Clear Data");
    QString text = tr("All data items will be deleted!")
            + " "
            + tr("Are you sure you want to do this?");
    auto ret = QMessageBox::warning(Q_NULLPTR,
                                    title,
                                    text,
                                    QMessageBox::Ok|QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        while (mUi->itemListWidget->count()) {
            QListWidgetItem *item = mUi->itemListWidget->item(0);
            deleteItem(item);
        }
    }
}

void SAKBaseListWidget::importItems()
{
    auto writableLocation = QStandardPaths::DesktopLocation;
    QString defaultPath = QStandardPaths::writableLocation(writableLocation);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import data"),
                                                    defaultPath,
                                                    QString("json (*.json)"));
    QFile file(fileName);
    QByteArray jsonArray;
    if (file.open(QFile::ReadWrite)) {
        jsonArray = file.readAll();
        file.close();
    } else {
        outputMessage(file.errorString(), false);
        return;
    }

    QJsonDocument jsc = QJsonDocument::fromJson(jsonArray);
    QJsonArray jsa = jsc.array();
    for (int i = 0; i < jsa.count(); i++) {
        QJsonObject parameters = jsa.at(i).toObject();
        auto *itemWidget = createItemFromParameters(parameters);
        QListWidgetItem *item = new QListWidgetItem();
        if (setupItemWidget(item, itemWidget)) {
            insertRecord(sqlInsert(mTableName, itemWidget));
        } else {
            itemWidget->deleteLater();
            delete item;
        }
    }
}

void SAKBaseListWidget::exportItems()
{
    QJsonArray jsonArray;
    for (int i = 0; i < mUi->itemListWidget->count(); i++) {
        QListWidgetItem *item = mUi->itemListWidget->item(i);
        QWidget *itemWidget = mUi->itemListWidget->itemWidget(item);
        QJsonObject jsonObj = toJsonObject(itemWidget);
        jsonArray.append(jsonObj);
    }

    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);


    // Open file
    auto desktopLocation = QStandardPaths::DesktopLocation;
    QString defaultName = QStandardPaths::writableLocation(desktopLocation);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Outport data"),
                                                    defaultName,
                                                    QString("json (*.json)"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if (file.open(QFile::ReadWrite)){
            file.write(jsonDoc.toJson());
            file.close();
        }else{
            outputMessage(file.errorString(), false);
        }
    }
}

void SAKBaseListWidget::deleteItem(QListWidgetItem *item)
{
    if (item) {
        QWidget *itemWidget = mUi->itemListWidget->itemWidget(item);
        quint64 id = itemId(itemWidget);
        mUi->itemListWidget->removeItemWidget(item);
        delete item;

        // Delete record from database.
        QString queryString = QString("DELETE FROM %1 WHERE ID=%2")
                .arg(mTableName)
                .arg(id);
        if (!mSqlQuery.exec(queryString)) {
            qWarning() << "Can not delete recored form(" << mTableName << ")"
                       << mSqlQuery.lastError().text();
        }
    } else {
        outputMessage(tr("Plese select an item first."), true);
    }
}

void SAKBaseListWidget::addItem()
{
    QListWidgetItem *item = new QListWidgetItem();
    QWidget *itemWidget = createItemFromParameters(QJsonObject());
    if (setupItemWidget(item, itemWidget)) {
        insertRecord(sqlInsert(mTableName, itemWidget));
    } else {
        itemWidget->deleteLater();
        delete item;
    }
}

void SAKBaseListWidget::readinRecords()
{
    const QString queryString = QString("SELECT * FROM %1").arg(mTableName);
    if (mSqlQuery.exec(queryString)) {
        while (mSqlQuery.next()) {
            QJsonObject parameters = toJsonObject(mSqlQuery);
            auto *itemWidget = createItemFromParameters(parameters);
            QListWidgetItem *item = new QListWidgetItem();
            if (!setupItemWidget(item, itemWidget)) {
                itemWidget->deleteLater();
                delete item;
            }
        }
    } else {
        qWarning() << "Select record form "
                   << mTableName
                   << " table failed: "
                   << mSqlQuery.lastError().text();
    }
}

bool SAKBaseListWidget::setupItemWidget(QListWidgetItem *item,
                                        QWidget *itemWidget)
{
    int count = mListWidget->count();
    for (int i = 0; i < count; i++) {
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *w = mListWidget->itemWidget(item);
        if (itemId(w) == itemId(itemWidget)) {
            return false;
        }
    }

    item->setSizeHint(itemWidget->sizeHint());
    mListWidget->addItem(item);
    mListWidget->setItemWidget(item, itemWidget);
    connectSignalsToSlots(itemWidget);

    auto cookedItemWidget = qobject_cast<SAKBaseListWidgetItemWidget*>(itemWidget);
    if (cookedItemWidget) {
        connect(this, &SAKBaseListWidget::bytesRead,
                cookedItemWidget, &SAKBaseListWidgetItemWidget::onBytesReadPrivate);

        connect(cookedItemWidget, &SAKBaseListWidgetItemWidget::invokeWriteCookedBytes,
                this, [&](QByteArray bytes){
            if (!forbidAllItems()) {
                emit invokeWriteCookedBytes(bytes);
            }
        });
    }

    return true;
}

void SAKBaseListWidget::insertRecord(const QString &sql)
{
    if (!mSqlQuery.exec(sql)) {
        qInfo() << sql;
        qWarning() << "Insert record failed:" << mSqlQuery.lastError().text();
    }
}

void SAKBaseListWidget::createTable(const QString &sql)
{
    if (!mSqlDatabase->tables().contains(mTableName)) {
        if (!mSqlQuery.exec(sql)) {
            qInfo() << sql;
            qWarning() << "Create table failed:" << mSqlQuery.lastError().text();
        }
    }
}

void SAKBaseListWidget::initialize()
{
    createTable(sqlCreate(mTableName));
    readinRecords();
}
