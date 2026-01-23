/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTableView>

namespace xCoAP {

class CoAPMsgViewPrivate;
class CoAPMsgView : public QTableView
{
    Q_OBJECT
    CoAPMsgViewPrivate *d{nullptr};

public:
    explicit CoAPMsgView(QWidget *parent = nullptr);
    ~CoAPMsgView() override;
};

} // namespace xCoAP