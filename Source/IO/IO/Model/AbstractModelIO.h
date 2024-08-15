/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#include "../AbstractIO.h"

namespace xTools {

class AbstractModelIO : public AbstractIO
{
    Q_OBJECT
public:
    explicit AbstractModelIO(QObject *parent = nullptr);

    virtual QVariant tableModel() const = 0;
    virtual QVariantMap saveItem(const int row) const = 0;
    virtual void loadItem(const int row, const QVariantMap &item) = 0;
};

} // namespace xTools
