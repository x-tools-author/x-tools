/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "AbstractModelUi.h"
#include <QMenu>

class AbstractIO;

namespace xTools {

class PresetUi : public AbstractModelUi
{
    Q_OBJECT
public:
    explicit PresetUi(QWidget *parent = nullptr);
    ~PresetUi();
    QMenu *menu();

protected:
    virtual QList<int> defaultHideColumns() override;
    virtual void afterRowEdited(int row) override;
    virtual QDialog *itemEditor() override;
};

} // namespace xTools
