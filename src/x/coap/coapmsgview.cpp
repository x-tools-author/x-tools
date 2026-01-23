/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapmsgview.h"
#include "ui_coapmsgview.h"

#include "coapmsgmodel.h"

namespace Ui {
class CoAPMsgView;
};

namespace xCoAP {

class CoAPMsgViewPrivate : public QObject
{
public:
    CoAPMsgViewPrivate(CoAPMsgView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPMsgView();
        ui->setupUi(q);

        m_model = new CoAPMsgModel(q);
        ui->tableView->setModel(m_model);
    }
    ~CoAPMsgViewPrivate() override {}

public:
    Ui::CoAPMsgView* ui{nullptr};
    CoAPMsgModel* m_model{nullptr};

private:
    CoAPMsgView* q{nullptr};
};

CoAPMsgView::CoAPMsgView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPMsgViewPrivate(this);
}

CoAPMsgView::~CoAPMsgView() {}

} // namespace xCoAP