/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourceview.h"
#include "ui_coapresourceview.h"

namespace xCoAP {

class CoAPResourceViewPrivate : public QObject
{
public:
    CoAPResourceViewPrivate(CoAPResourceView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPResourceView();
        ui->setupUi(q);
    }
    ~CoAPResourceViewPrivate() override {}

public:
    Ui::CoAPResourceView* ui{nullptr};

private:
    CoAPResourceView* q{nullptr};
};

CoAPResourceView::CoAPResourceView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPResourceViewPrivate(this);
}

CoAPResourceView::~CoAPResourceView() {}

} // namespace xCoAP