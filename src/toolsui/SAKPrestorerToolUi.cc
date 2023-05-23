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

#include "SAKPrestorerTool.hh"
#include "SAKPrestorerToolUi.hh"
#include "SAKPrestorerToolUiEditor.hh"

SAKPrestorerToolUi::SAKPrestorerToolUi(QWidget *parent)
    : SAKTableViewWithController{"SAK.PrestorerToolUi", parent}
{
    mEditor = new SAKPrestorerToolUiEditor(this);
}

SAKPrestorerToolUi::~SAKPrestorerToolUi()
{

}

void SAKPrestorerToolUi::setupSAKPrestorerTool(SAKPrestorerTool *tool)
{
    if (tool == nullptr) {
        return;
    }

    if (!tool->inherits("SAKPrestorerTool")) {
        return;
    }

    mTool = qobject_cast<SAKPrestorerTool*>(tool);
    SAKPrestorerTableModel *dataModel = mTool->getModel();
    setupTableModel(dataModel);
}

void SAKPrestorerToolUi::edit(const QModelIndex &index)
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

void SAKPrestorerToolUi::clear()
{
    int rowCount = mTool->getModel()->rowCount();
    for (int i = 0; i < rowCount; i++) {
        mTool->getModel()->removeRow(i);
    }
}

void SAKPrestorerToolUi::remove(const QModelIndex &index)
{
    if (index.isValid()) {
        auto model = mTool->getModel();
        model->removeRow(index.row());
    }
}

void SAKPrestorerToolUi::importFromFile(const QString &fileName)
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

void SAKPrestorerToolUi::exportToFile(const QString &fileName)
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

void SAKPrestorerToolUi::append()
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
