/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "panel.h"

Panel::Panel(QWidget *parent)
    : QWidget(parent)
{}

Panel::~Panel() {}

QVariantMap Panel::save() const
{
    return QVariantMap();
}

void Panel::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void Panel::onBytesRead(const QByteArray &bytes, const QString &flag)
{
    Q_UNUSED(bytes);
    Q_UNUSED(flag);
}

void Panel::onBytesWritten(const QByteArray &bytes, const QString &flag)
{
    Q_UNUSED(bytes);
    Q_UNUSED(flag);
}