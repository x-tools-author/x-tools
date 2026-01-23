/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace xCoAP {

class CoAPResourceViewPrivate;
class CoAPResourceView : public QWidget
{
    Q_OBJECT
    CoAPResourceViewPrivate *d{nullptr};

public:
    explicit CoAPResourceView(QWidget *parent = nullptr);
    ~CoAPResourceView() override;
};

} // namespace xCoAP