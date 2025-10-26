/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsdictionary.h"

TsDictionary::TsDictionary(QObject *parent)
    : QAbstractTableModel(parent)
{}

TsDictionary::~TsDictionary() {}

int TsDictionary::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 0;
}

int TsDictionary::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 0;
}

QVariant TsDictionary::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

bool TsDictionary::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}