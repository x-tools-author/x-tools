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

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

protected:
    QList<int> universalColumns() const override;

private:
    QMenu *m_menu{nullptr};
};

} // namespace xTools
