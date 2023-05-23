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
    int rowCount = mTool->getModel()->rowCount();
    for (int i = 0; i < rowCount; i++) {
        mTool->getModel()->removeRow(i);
    }
}

void SAKEmitterToolUi::remove(const QModelIndex &index)
{
    if (index.isValid()) {
        auto model = mTool->getModel();
        model->removeRow(index.row());
    }
}

void SAKEmitterToolUi::importFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QByteArray json = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
        QJsonArray jsonArray = jsonDoc.array();
        for (int i = 0; i < jsonArray.count(); i++) {
            QJsonObject jsonObj = jsonArray.at(i).toObject();
            QJsonDocument jd;
            jd.setObject(jsonObj);
            QString item = QString::fromUtf8(jd.toJson());
            mTool->addItem(item);
        }
    } else {
        qCWarning(mLoggingCategory) <<  "Can not open file:"
                                    << file.errorString();
    }
}

void SAKEmitterToolUi::exportToFile(const QString &fileName)
{
    auto items = mTool->itemsContext();

    QJsonArray jsonArray = items.toJsonArray();
    QJsonDocument jsonDoc;

    jsonDoc.setArray(jsonArray);

    QByteArray json = jsonDoc.toJson();
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        out << json;
        file.close();
    } else {
        qCWarning(mLoggingCategory) <<  "Can not open file:"
                                    << file.errorString();
    }
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
