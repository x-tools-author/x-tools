/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QJsonArray>
#include <QTableView>
#include <QHeaderView>
#include <QJsonDocument>
#include <QStandardItemModel>

#include "SAKEmitterTool.hh"
#include "SAKEmitterToolUi.hh"

SAKEmitterToolUi::SAKEmitterToolUi(QWidget *parent)
    : SAKTableViewWithController{"SAK.EmitterToolUi", parent}
{
    mEditor = new SAKEmitterToolUiEditor(this);
}

SAKEmitterToolUi::~SAKEmitterToolUi()
{

}

void SAKEmitterToolUi::initialize(SAKEmitterTool *tool,
                                  const QString &settingsGroup)
{
    if (tool == nullptr) {
        return;
    }

    if (!tool->inherits("SAKEmitterTool")) {
        return;
    }

    mTool = qobject_cast<SAKEmitterTool*>(tool);
    QVariant var = mTool->tableModel();
    QAbstractTableModel *dataModel = var.value<QAbstractTableModel*>();
    SAKTableViewWithController::initialize(dataModel,
                                           settingsGroup + "/emitter");
}

void SAKEmitterToolUi::edit(const QModelIndex &index)
{
    QVariant var = mTool->itemContext(index.row());
    QJsonObject jsonObj = var.toJsonObject();
    mEditor->setParameters(jsonObj);
    mEditor->show();

    if (QDialog::Accepted == mEditor->exec()) {
        QJsonObject params = mEditor->parameters();
        QJsonDocument jsonDoc;
        jsonDoc.setObject(params);
        QString str = QString::fromUtf8(jsonDoc.toJson());
        mTool->addItem(str, index.row());
    }
}

void SAKEmitterToolUi::clear()
{
    QVariant var = mTool->tableModel();
    QAbstractTableModel *tableModel = var.value<QAbstractTableModel*>();
    int rowCount = tableModel->rowCount();
    tableModel->removeRows(0, rowCount);
}

void SAKEmitterToolUi::remove(const QModelIndex &index)
{
    if (index.isValid()) {
        QVariant var = mTool->tableModel();
        QAbstractTableModel *tableModel = var.value<QAbstractTableModel*>();
        tableModel->removeRow(index.row());
    }
}

void SAKEmitterToolUi::importFromJson(const QByteArray &json)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    QJsonArray jsonArray = jsonDoc.array();
    for (int i = 0; i < jsonArray.count(); i++) {
        QJsonObject jsonObj = jsonArray.at(i).toObject();
        QJsonDocument jd;
        jd.setObject(jsonObj);
        QString item = QString::fromUtf8(jd.toJson());
        mTool->addItem(item);
    }
}

QByteArray SAKEmitterToolUi::exportAsJson()
{
    auto items = mTool->itemsContext();

    QJsonArray jsonArray = items.toJsonArray();
    QJsonDocument jsonDoc;

    jsonDoc.setArray(jsonArray);

    QByteArray json = jsonDoc.toJson();
    return json;
}

bool SAKEmitterToolUi::append()
{
    QJsonObject jsonObj = mTool->itemContext(-1).toJsonObject();
    mEditor->setParameters(jsonObj);
    mEditor->show();
    int ret = mEditor->exec();
    if (!(ret == QDialog::Accepted)) {
        return false;
    }

    jsonObj = mEditor->parameters();
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QString str = QString::fromUtf8(jsonDoc.toJson());
    mTool->addItem(str);

    return true;
}
