/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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

#include "common/xtools.h"
#include "respondermodel.h"

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

        auto options = supportedResponseOptions();
        for (auto &option : options) {
            auto data = static_cast<ResponseOption>(option);
            auto text = responseOptionName(data);
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
    m_tableModel = new ResponderModel(this);
    setTableModel(m_tableModel);

    auto *tv = tableView();
    tv->setItemDelegateForColumn(2, new ResponserItemDelegate(tv));

    auto hHeader = tv->horizontalHeader();
    hHeader->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
    hHeader->setSectionResizeMode(5, QHeaderView::ResizeMode::Stretch);
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

        auto cookedOption = static_cast<ResponseOption>(option);
        TextItem cookedRef = loadTextItem(ref);
        TextItem cookedRes = loadTextItem(res);

        QByteArray refBytes = textItem2array(cookedRef);
        QByteArray resBytes = textItem2array(cookedRes);

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
    return QList<int>{4, 5};
}
