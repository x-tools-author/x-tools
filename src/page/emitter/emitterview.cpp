/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emitterview.h"

#include <QCheckBox>
#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTimer>

#include "emittermodel.h"

class EmitterViewPrivate : public QObject
{
public:
    EmitterViewPrivate(EmitterView *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~EmitterViewPrivate() {}

public:
    void try2Output()
    {
        int rows = m_tableModel->rowCount(QModelIndex());
        for (int i = 0; i < rows; ++i) {
            m_tableModel->increaseElapsedTime(i, 10);
            if (!m_tableModel->isTimeout(i)) {
                continue;
            }

            m_tableModel->resetElapsedTime(i);

            if (!m_tableModel->data(m_tableModel->index(i, 0), Qt::EditRole).toBool()) {
                continue;
            }

            QVariant var = m_tableModel->data(m_tableModel->index(i, 3), Qt::EditRole);
            QJsonObject json = var.toJsonObject();
            TextItem textItem = xLoadTextItem(json);
            QByteArray bytes = xTextItem2array(textItem);

            if (!q->isDisableAll()) {
                emit q->outputBytes(bytes);
            }
        }
    }

public:
    EmitterModel *m_tableModel;

private:
    EmitterView *q;
};

EmitterView::EmitterView(QWidget *parent)
    : TableView(parent)
{
    d = new EmitterViewPrivate(this);
    d->m_tableModel = new EmitterModel(this);
    setTableModel(d->m_tableModel);

    QTableView *tv = tableView();
    QHeaderView *hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);

    QTimer *timer = new QTimer(this);
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        d->try2Output();
        timer->start();
    });
    timer->start();
}

EmitterView::~EmitterView() {}

QList<int> EmitterView::textItemColumns() const
{
    return QList<int>{3};
}
