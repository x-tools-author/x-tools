/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>
#include <QWidget>

namespace xTools {

class AbstractIO;
class AbstractIOUi : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractIOUi(QWidget *parent = nullptr);
    ~AbstractIOUi();

    virtual QVariantMap save() const = 0;
    virtual void load(const QVariantMap &parameters) = 0;
    virtual void setupIO(AbstractIO *io);
    virtual void setUiEnabled(bool enabled);
};

} // namespace xTools
