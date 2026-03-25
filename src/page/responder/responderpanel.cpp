/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "responderpanel.h"

#include <QHBoxLayout>

#include "responderview.h"

class ResponderPanelPrivate : public QObject
{
public:
    explicit ResponderPanelPrivate(ResponderPanel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    { }
    ~ResponderPanelPrivate() { }

public:
    ResponderPanel *q;
    ResponderView *m_view{nullptr};
};

ResponderPanel::ResponderPanel(QWidget *parent)
    : Panel(parent)
{
    d = new ResponderPanelPrivate(this);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    d->m_view = new ResponderView(this);
    layout->addWidget(d->m_view);
}

ResponderPanel::~ResponderPanel() { }

QVariantMap ResponderPanel::save() const
{
    return d->m_view->save();
}

void ResponderPanel::load(const QVariantMap &parameters)
{
    d->m_view->load(parameters);
}

void ResponderPanel::onBytesRead(const QByteArray &bytes, const QString &flag)
{
    Q_UNUSED(bytes);
    Q_UNUSED(flag);
}

void ResponderPanel::onBytesWritten(const QByteArray &bytes, const QString &flag)
{
    Q_UNUSED(bytes);
    Q_UNUSED(flag);
}

ResponderView *ResponderPanel::view() const
{
    return d->m_view;
}