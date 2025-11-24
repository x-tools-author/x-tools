/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcanbus.h"
#include "ui_xcanbus.h"

namespace xCanBus {

struct xCanBusParameterKeys
{
    const QString leftWidth{"leftWidth"};
};

xCanBus::xCanBus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xCanBus)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    connect(ui->splitter, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        this->m_leftWidth = ui->splitter->sizes().first();
    });
}

xCanBus::~xCanBus()
{
    delete ui;
}

QJsonObject xCanBus::save()
{
    QJsonObject obj;
    xCanBusParameterKeys keys;
    obj.insert(keys.leftWidth, ui->splitter->sizes().first());
    return obj;
}

void xCanBus::load(const QJsonObject& obj)
{
    xCanBusParameterKeys keys;
    int leftWidth = obj.value(keys.leftWidth).toInt(168);
    ui->splitter->setSizes({leftWidth, width() - leftWidth});
}

bool xCanBus::event(QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    }

    return QWidget::event(event);
}

} // namespace xCanBus