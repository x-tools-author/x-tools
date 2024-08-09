/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Preset.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "PresetModel.h"

namespace xTools {

Preset::Preset(QObject *parent)
    : AbstractModelIO{parent}
    , m_tableModel{new PresetModel{this}}
{

}

QVariant Preset::tableModel()
{
    return QVariant::fromValue(m_tableModel);
}

QVariantMap Preset::saveItem(const int row) const
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = m_tableModel->data(m_tableModel->index(row, 0), Qt::DisplayRole);
    QString description = var.toString();

    var = m_tableModel->data(m_tableModel->index(row, 1), Qt::EditRole);
    QJsonObject item = var.toJsonObject();

    QVariantMap map = item.toVariantMap();
    map.insert("description", description);
    return map;
}

void Preset::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    QString description = item.value("description").toString();
    m_tableModel->setData(m_tableModel->index(row, 0), description, Qt::EditRole);

    QJsonObject json = QJsonObject::fromVariantMap(item);
    m_tableModel->setData(m_tableModel->index(row, 1), json, Qt::EditRole);
}

void Preset::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes)
}

void Preset::run()
{
    exec();
}

} // namespace xTools
