/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "canbuspanel.h"

namespace xCanBus {

CanBusPanel::CanBusPanel(QWidget* parent)
    : QWidget(parent)
{}

CanBusPanel::~CanBusPanel() {}

QJsonObject CanBusPanel::save() const
{
    return QJsonObject();
}

void CanBusPanel::load(const QJsonObject& obj)
{
    Q_UNUSED(obj);
}

void CanBusPanel::onFrameRx(const QCanBusFrame& frame)
{
    Q_UNUSED(frame);
}

void CanBusPanel::onFrameTx(const QCanBusFrame& frame)
{
    Q_UNUSED(frame);
}

} // namespace xCanBus