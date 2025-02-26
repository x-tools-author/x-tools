/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "CommunicationUi.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class HidUi;
}
QT_END_NAMESPACE

namespace xTools {

class HidUi : public CommunicationUi
{
    Q_OBJECT
public:
    HidUi(CommunicationType type, QWidget *parent = nullptr);

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

private:
    Ui::HidUi *ui;

private:
    void refresh();
};

} // namespace xTools
