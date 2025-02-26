/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "responserui.h"

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

#include "IO/xIO.h"

namespace xTools {

class ResponserItemDelefate : public QStyledItemDelegate
{
public:
    ResponserItemDelefate(QObject *parent = nullptr)
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

        auto options = xIO::supportedResponseOptions();
        for (auto &option : options) {
            auto data = static_cast<xIO::ResponseOption>(option);
            auto text = xIO::responseOptionName(data);
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

ResponserUi::ResponserUi(QWidget *parent)
    : AbstractModelUi(parent)
{

}

ResponserUi::~ResponserUi() {}

void ResponserUi::setupIO(AbstractIO *io)
{
    AbstractModelUi::setupIO(io);

    auto *tv = tableView();
    tv->setItemDelegateForColumn(2, new ResponserItemDelefate(tv));

    auto hHeader = tv->horizontalHeader();
    hHeader->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
    hHeader->setSectionResizeMode(5, QHeaderView::ResizeMode::Stretch);
}

QList<int> ResponserUi::textItemColumns() const
{
    return QList<int>{4, 5};
}

} // namespace xTools
