/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfileitemdelegate.h"

TsFileItemDelegate::TsFileItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

TsFileItemDelegate::~TsFileItemDelegate() {}

QWidget *TsFileItemDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TsFileItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void TsFileItemDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}