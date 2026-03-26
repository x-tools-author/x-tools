/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emitterpanel.h"

#include <QHBoxLayout>

#include "emitterview.h"

class EmitterPanelPrivate : public QObject
{
public:
    explicit EmitterPanelPrivate(EmitterPanel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~EmitterPanelPrivate() {}

public:
    EmitterPanel *q;
    EmitterView *m_view{nullptr};
};

EmitterPanel::EmitterPanel(QWidget *parent)
    : Panel(parent)

{
    d = new EmitterPanelPrivate(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    d->m_view = new EmitterView(this);
    layout->addWidget(d->m_view);

    connect(d->m_view, &EmitterView::outputBytes, this, &EmitterPanel::outputBytes);
}

EmitterPanel::~EmitterPanel()
{
    delete d;
}

QVariantMap EmitterPanel::save() const
{
    return d->m_view->save();
}

void EmitterPanel::load(const QVariantMap &parameters)
{
    d->m_view->load(parameters);
}
