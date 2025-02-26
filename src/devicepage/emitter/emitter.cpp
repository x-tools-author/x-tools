/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emitter.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QVariant>

#include "emittermodel.h"

namespace xTools {

Emitter::Emitter(QObject *parent)
    : AbstractModelIO{parent}
    , m_tableModel{new EmitterModel(this)}
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        try2Emit();
        timer->start();
    });

    connect(this, &Emitter::started, timer, [=]() { timer->start(); });
    connect(this, &Emitter::finished, timer, [=]() { timer->stop(); });
}

QVariant Emitter::tableModel() const
{
    return QVariant::fromValue(m_tableModel);
}

QVariantMap Emitter::saveItem(const int row) const
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
    int interval = var.toInt();

    var = m_tableModel->data(m_tableModel->index(row, 3), Qt::EditRole);
    QJsonObject item = var.toJsonObject();

    QVariantMap map = item.toVariantMap();
    map.insert("description", description);
    map.insert("interval", interval);
    map.insert("enable", enable);
    return map;
}

void Emitter::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_tableModel->rowCount(QModelIndex())) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    bool enable = item.value("enable").toBool();
    m_tableModel->setData(m_tableModel->index(row, 0), enable, Qt::EditRole);

    QString description = item.value("description").toString();
    m_tableModel->setData(m_tableModel->index(row, 1), description, Qt::EditRole);

    int interval = item.value("interval").toInt();
    m_tableModel->setData(m_tableModel->index(row, 2), interval, Qt::EditRole);

    QJsonObject json = QJsonObject::fromVariantMap(item);
    m_tableModel->setData(m_tableModel->index(row, 3), json, Qt::EditRole);
}

void Emitter::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes)
}

void Emitter::run()
{
    exec();
}

void Emitter::try2Emit()
{
    int rows = m_tableModel->rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i) {
        m_tableModel->increaseElapsedTime(i, 10);
        if (m_tableModel->isTimeout(i)) {
            m_tableModel->resetElapsedTime(i);

            if (!m_tableModel->data(m_tableModel->index(i, 0), Qt::EditRole).toBool()) {
                return;
            }

            QVariant var = m_tableModel->data(m_tableModel->index(i, 3), Qt::EditRole);
            QJsonObject json = var.toJsonObject();
            TextItem textItem = loadTextItem(json);
            QByteArray bytes = textItem2array(textItem);

            if (m_enable) {
                emit outputBytes(bytes);
            }
        }
    }
}

} // namespace xTools
