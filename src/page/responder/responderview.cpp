/***************************************************************************************************
 * Copyright 2023-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "responderview.h"

#include <QComboBox>
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

#include "common/x.h"
#include "respondermodel.h"

class ResponderViewPrivate : public QObject
{
public:
    explicit ResponderViewPrivate(ResponderView *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~ResponderViewPrivate() {}

public:
    ResponderModel *m_tableModel{nullptr};

private:
    ResponderView *q;
};

class ResponserItemDelegate : public QStyledItemDelegate
{
public:
    ResponserItemDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        if (index.column() != 2) {
            return nullptr;
        }

        auto *editor = new QComboBox(parent);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        if (index.column() != 2) {
            return;
        }

        auto *comboBox = qobject_cast<QComboBox *>(editor);
        if (!comboBox) {
            return;
        }

        auto options = xSupportedResponseOptions();
        for (auto &option : options) {
            auto data = static_cast<ResponseOption>(option);
            auto text = xResponseOptionName(data);
            comboBox->addItem(text, option);
        }

        int i = comboBox->findData(index.data().toInt());
        comboBox->setCurrentIndex(i);
    }

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override
    {
        if (index.column() != 2) {
            return;
        }

        auto *comboBox = qobject_cast<QComboBox *>(editor);
        if (!comboBox) {
            return;
        }

        auto data = comboBox->currentData().toInt();
        model->setData(index, data);
    }
};

ResponderView::ResponderView(QWidget *parent)
    : TableView(parent)
{
    d = new ResponderViewPrivate(this);
    d->m_tableModel = new ResponderModel(this);
    setTableModel(d->m_tableModel);

    auto *tv = tableView();
    tv->setItemDelegateForColumn(RESPONSE_MODEL_COLUMN_OPTION, new ResponserItemDelegate(tv));

    auto hHeader = tv->horizontalHeader();
    hHeader->setSectionResizeMode(RESPONSE_MODEL_COLUMN_REFERENCE, QHeaderView::ResizeMode::Stretch);
    hHeader->setSectionResizeMode(RESPONSE_MODEL_COLUMN_RESPONSE, QHeaderView::ResizeMode::Stretch);
}

ResponderView::~ResponderView() {}

void ResponderView::inputBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        return;
    }

    if (isDisableAll()) {
        return;
    }

    int rows = d->m_tableModel->rowCount(QModelIndex());
    for (int i = 0; i < rows; i++) {
        QModelIndex index = d->m_tableModel->index(i, RESPONSE_MODEL_COLUMN_ENABLE);
        bool enable = d->m_tableModel->data(index, Qt::EditRole).toBool();
        if (!enable) {
            continue;
        }

        index = d->m_tableModel->index(i, RESPONSE_MODEL_COLUMN_OPTION);
        int option = d->m_tableModel->data(index, Qt::EditRole).toInt();
        index = d->m_tableModel->index(i, RESPONSE_MODEL_COLUMN_DELAY);
        int delay = d->m_tableModel->data(index, Qt::EditRole).toInt();
        index = d->m_tableModel->index(i, RESPONSE_MODEL_COLUMN_REFERENCE);
        QJsonObject ref = d->m_tableModel->data(index, Qt::EditRole).toJsonObject();
        index = d->m_tableModel->index(i, RESPONSE_MODEL_COLUMN_RESPONSE);
        QJsonObject res = d->m_tableModel->data(index, Qt::EditRole).toJsonObject();

        auto cookedOption = static_cast<ResponseOption>(option);
        TextItem cookedRef = xLoadTextItem(ref);
        TextItem cookedRes = xLoadTextItem(res);

        QByteArray refBytes = xTextItem2array(cookedRef);
        QByteArray resBytes = xTextItem2array(cookedRes);

        if (cookedOption == ResponseOption::Echo) {
            QTimer::singleShot(delay, this, [=] { emit outputBytes(bytes); });
            continue;
        }

        if (resBytes.isEmpty()) {
            continue;
        }

        if (cookedOption == ResponseOption::InputEqualReference) {
            if (bytes != refBytes) {
                continue;
            }
        } else if (cookedOption == ResponseOption::InputContainReference) {
            if (!bytes.contains(refBytes)) {
                continue;
            }
        } else if (cookedOption == ResponseOption::InputDoesNotContainReference) {
            if (bytes.contains(refBytes)) {
                continue;
            }
        }

        QTimer::singleShot(delay, this, [=] { emit outputBytes(resBytes); });
    }
}

QList<int> ResponderView::textItemColumns() const
{
    return QList<int>{RESPONSE_MODEL_COLUMN_REFERENCE, RESPONSE_MODEL_COLUMN_RESPONSE};
}
