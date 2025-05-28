/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QItemDelegate>

class SvgItemDelegate : public QItemDelegate
{
public:
    explicit SvgItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    static void setColor(const QString &color) { s_color = color; }
    static void setSize(int size) { s_size = size < 16 ? 16 : size; }

private:
    static QString s_color;
    static int s_size;
};
