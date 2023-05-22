/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTableView>
#include <QHeaderView>
#include <QJsonDocument>
#include <QStandardItemModel>

#include "SAKEmitterTool.hh"
#include "SAKEmitterToolUi.hh"

SAKEmitterToolUi::SAKEmitterToolUi(QWidget *parent)
    : SAKTableView{"SAK.EmitterToolUi", parent}
{
    mEditor = new SAKEmitterToolUiEditor(this);
}

SAKEmitterToolUi::~SAKEmitterToolUi()
{

}

void SAKEmitterToolUi::setupEmitterTool(SAKEmitterTool *tool)
{
    if (tool == nullptr) {
        return;
    }

    if (!tool->inherits("SAKEmitterTool")) {
        return;
    }

    mTool = qobject_cast<SAKEmitterTool*>(tool);
    SAKEmitterTableModel *dataModel = mTool->getModel();
    setupTableModel(dataModel);
}

void SAKEmitterToolUi::edit(const QModelIndex &index)
{
    QVariant var = mTool->itemContext(index.row());
    QJsonObject jsonObj = var.toJsonObject();
    mEditor->setParameters(jsonObj);
    mEditor->show();
    if (!(QDialog::Accepted ==mEditor->exec())) {
        return;
    }
}

void SAKEmitterToolUi::clear()
{

}

void SAKEmitterToolUi::remove(const QModelIndex &index)
{

}

void SAKEmitterToolUi::importFromFile(const QString &fileName)
{
    Q_UNUSED(fileName);
}

void SAKEmitterToolUi::exportToFile(const QString &fileName)
{
    Q_UNUSED(fileN)
}

void SAKEmitterToolUi::append()
{
    QJsonObject jsonObj = mTool->itemContext(-1).toJsonObject();
    mEditor->setParameters(jsonObj);
    mEditor->show();
    int ret = mEditor->exec();
    if (!(ret == QDialog::Accepted)) {
        return;
    }

    jsonObj = mEditor->parameters();
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QString str = QString::fromUtf8(jsonDoc.toJson());
    mTool->addItem(str, -1);
}
