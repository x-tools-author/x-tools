/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "presetpanel.h"

#include <QHBoxLayout>

#include "presetview.h"

class PresetPanelPrivate : public QObject
{
public:
    PresetPanelPrivate(PresetPanel *q_ptr)
        : q(q_ptr)
    {}
    ~PresetPanelPrivate() {}

public:
    PresetView *m_view{nullptr};

private:
    PresetPanel *q;
};

PresetPanel::PresetPanel(QWidget *parent)
    : Panel(parent)
{
    d = new PresetPanelPrivate(this);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    auto *view = new PresetView(this);
    layout->addWidget(view);
    d->m_view = view;
}

PresetPanel::~PresetPanel() {}

QVariantMap PresetPanel::save() const
{
    return d->m_view->save();
}

void PresetPanel::load(const QVariantMap &parameters)
{
    d->m_view->load(parameters);
}

PresetView *PresetPanel::presetView() const
{
    return d->m_view;
}