/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketStyledItemDelegate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "../../xIO.h"

namespace xTools {

SocketStyledItemDelegate::SocketStyledItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

SocketStyledItemDelegate::~SocketStyledItemDelegate() {}

QWidget *SocketStyledItemDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(option);
    switch (index.column()) {
    case 6: // authentication
        return new QCheckBox(parent);
    case 0: // option
    case 1: // client address
    case 3: // server address
    case 5: // data channel
        return new QComboBox(parent);
    case 2: // client port
    case 4: // server port
        return new QSpinBox(parent);
    case 7: // username
    case 8: // password
    case 9: // description
        return new QLineEdit(parent);
    default:
        return nullptr;
    }
}

void SocketStyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //0:transfer type, 1:client address, 2:client port, 3:server address, 4:server port, 5:data channel
    //6:authentication, 7:username, 8:password, 9:description
    int column = index.column();
    if (column == 6) {
        qobject_cast<QCheckBox *>(editor)->setChecked(index.data(Qt::EditRole).toBool());
    } else if (column == 0 || column == 1 || column == 3 || column == 5 || column == 7) {
        auto cb = qobject_cast<QComboBox *>(editor);
        if (column == 0) {
            xIO::setupTransferType(cb);
            int transferType = index.data(Qt::EditRole).toInt();
            int index = cb->findData(transferType);
            if (index == -1) {
                cb->setCurrentIndex(0);
            } else {
                cb->setCurrentIndex(index);
            }
        } else if (column == 1 || column == 3) {
            xIO::setupSocketAddress(cb);
            QString text = index.data(Qt::EditRole).toString();
            cb->setCurrentText(text);
        } else if (column == 5) {
            xIO::setupWebSocketDataChannel(cb);
            int dataChannel = index.data(Qt::EditRole).toInt();
            int index = cb->findData(dataChannel);
            if (index == -1) {
                cb->setCurrentIndex(0);
            } else {
                cb->setCurrentIndex(index);
            }
        }
    } else if (column == 2 || column == 4) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        xIO::setupSocketPort(sb);
        sb->setValue(index.data(Qt::EditRole).toInt());
    } else if (column == 7 || column == 8 || column == 9) {
        auto le = qobject_cast<QLineEdit *>(editor);
        le->setText(index.data(Qt::EditRole).toString());
    }
}

void SocketStyledItemDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    //0:transfer type, 1:client address, 2:client port, 3:server address, 4:server port, 5:data channel
    //6:authentication, 7:username, 8:password, 9:description
    int column = index.column();
    if (column == 6) {
        model->setData(index, qobject_cast<QCheckBox *>(editor)->isChecked(), Qt::EditRole);
    } else if (column == 0 || column == 1 || column == 3 || column == 5) {
        auto cb = qobject_cast<QComboBox *>(editor);
        if (column == 0 || column == 5) {
            model->setData(index, cb->currentData(), Qt::EditRole);
        } else {
            model->setData(index, cb->currentText());
        }
    } else if (column == 2 || column == 4) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        model->setData(index, sb->value(), Qt::EditRole);
    } else if (column == 7 || column == 8 || column == 9) {
        auto le = qobject_cast<QLineEdit *>(editor);
        model->setData(index, le->text(), Qt::EditRole);
    }
}

} // namespace xTools
