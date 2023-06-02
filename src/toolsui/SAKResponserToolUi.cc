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
#include <QTimer>
#include <QMutex>
#include <QVariant>
#include <QJsonArray>
#include <QJsonDocument>
#include <QAbstractTableModel>

#include "SAKResponserTool.hh"
#include "SAKResponserToolUi.hh"
#include "SAKResponserToolUiEditor.hh"

SAKResponserToolUi::SAKResponserToolUi(QWidget *parent)
    : SAKTableViewWithController{"SAK.ResponserToolUi", parent}
{
    mEditor = new SAKResponserToolUiEditor(this);
}

SAKResponserToolUi::~SAKResponserToolUi()
{

}

void SAKResponserToolUi::initialize(SAKResponserTool *tool,
                                    const QString &settingsGroup)
{
    if (tool == nullptr) {
        return;
    }

    if (!tool->inherits("SAKResponserTool")) {
        return;
    }

    mTool = qobject_cast<SAKResponserTool*>(tool);
    auto dataModel = mTool->tableModel().value<QAbstractTableModel*>();
    SAKTableViewWithController::initialize(dataModel,
                                           settingsGroup + "/responser");

    QList<int> columns;
    columns << 11 << 21;
    setStretchSections(columns);
}


void SAKResponserToolUi::edit(const QModelIndex &index)
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

void SAKResponserToolUi::clear()
{
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        mTableModel->removeRow(i);
    }
}

void SAKResponserToolUi::remove(const QModelIndex &index)
{
    if (index.isValid()) {
        mTableModel->removeRow(index.row());
    }
}

void SAKResponserToolUi::importFromJson(const QByteArray &json)
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

QByteArray SAKResponserToolUi::exportAsJson()
{
    auto items = mTool->itemsContext();

    QJsonArray jsonArray = items.toJsonArray();
    QJsonDocument jsonDoc;

    jsonDoc.setArray(jsonArray);

    QByteArray json = jsonDoc.toJson();
    return json;
}

bool SAKResponserToolUi::append()
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
    mTool->addItem(str, -1);
    return true;
}
