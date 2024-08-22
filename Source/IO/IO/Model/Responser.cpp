/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Responser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QVariant>

#include "ResponserModel.h"

namespace xTools {

Responser::Responser(QObject *parent)
    : AbstractModelIO{parent}
    , m_tableModel{new ResponserModel(this)}
{

}

QVariant Responser::tableModel() const
{
    return QVariant::fromValue(m_tableModel);
}

QVariantMap Responser::saveItem(const int row) const
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = m_tableModel->data(m_tableModel->index(row, 0), Qt::DisplayRole);
    bool enable = var.toBool();

    var = m_tableModel->data(m_tableModel->index(row, 1), Qt::DisplayRole);
    QString description = var.toString();

    var = m_tableModel->data(m_tableModel->index(row, 2), Qt::EditRole);
    int option = var.toInt();

    var = m_tableModel->data(m_tableModel->index(row, 3), Qt::EditRole);
    int interval = var.toInt();

    var = m_tableModel->data(m_tableModel->index(row, 4), Qt::EditRole);
    QJsonObject referenceItem = var.toJsonObject();

    var = m_tableModel->data(m_tableModel->index(row, 4), Qt::EditRole);
    QJsonObject responseItem = var.toJsonObject();

    QVariantMap map;
    map.insert("enable", enable);
    map.insert("description", description);
    map.insert("option", option);
    map.insert("delay", interval);
    map.insert("reference", referenceItem);
    map.insert("response", responseItem);
    return map;
}

void Responser::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    bool enable = item.value("enable").toBool();
    m_tableModel->setData(m_tableModel->index(row, 0), enable, Qt::EditRole);

    QString description = item.value("description").toString();
    m_tableModel->setData(m_tableModel->index(row, 1), description, Qt::EditRole);

    int option = item.value("option").toInt();
    m_tableModel->setData(m_tableModel->index(row, 2), option, Qt::EditRole);

    int interval = item.value("delay").toInt();
    m_tableModel->setData(m_tableModel->index(row, 3), interval, Qt::EditRole);

    QJsonObject json = item.value("reference").toJsonObject();
    m_tableModel->setData(m_tableModel->index(row, 4), json, Qt::EditRole);

    json = item.value("response").toJsonObject();
    m_tableModel->setData(m_tableModel->index(row, 5), json, Qt::EditRole);
}

void Responser::inputBytes(const QByteArray &bytes)
{
    if (!(!bytes.isEmpty() && isRunning())) {
        return;
    }

    if (!m_enable) {
        return;
    }

    int rows = m_tableModel->rowCount(QModelIndex());
    for (int i = 0; i < rows; i++) {
        bool enable = m_tableModel->data(m_tableModel->index(i, 0), Qt::EditRole).toBool();
        if (!enable) {
            continue;
        }

        int option = m_tableModel->data(m_tableModel->index(i, 2), Qt::EditRole).toInt();
        int delay = m_tableModel->data(m_tableModel->index(i, 3), Qt::EditRole).toInt();
        QJsonObject ref = m_tableModel->data(m_tableModel->index(i, 4), Qt::EditRole).toJsonObject();
        QJsonObject res = m_tableModel->data(m_tableModel->index(i, 5), Qt::EditRole).toJsonObject();

        auto cookedOption = static_cast<xIO::ResponseOption>(option);
        xIO::TextItem cookedRef = xIO::loadTextItem(ref);
        xIO::TextItem cookedRes = xIO::loadTextItem(res);

        QByteArray refBytes = xIO::textItem2array(cookedRef);
        QByteArray resBytes = xIO::textItem2array(cookedRes);

        if (cookedOption == xIO::ResponseOption::Echo) {
            QTimer::singleShot(delay, this, [=] { emit outputBytes(bytes); });
            continue;
        }

        if (resBytes.isEmpty()) {
            continue;
        }

        if (cookedOption == xIO::ResponseOption::InputEqualReference) {
            if (bytes != refBytes) {
                continue;
            }
        } else if (cookedOption == xIO::ResponseOption::InputContainReference) {
            if (!bytes.contains(refBytes)) {
                continue;
            }
        } else if (cookedOption == xIO::ResponseOption::InputDoesNotContainReference) {
            if (bytes.contains(refBytes)) {
                continue;
            }
        }

        QTimer::singleShot(delay, this, [=] { emit outputBytes(resBytes); });
    }
}

void Responser::run()
{
    exec();
}

} // namespace xTools
