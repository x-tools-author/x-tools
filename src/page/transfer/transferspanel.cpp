/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "transferspanel.h"

#include <QHBoxLayout>

#include "transfersview.h"

class TransfersPanelPrivate : public QObject
{
public:
    explicit TransfersPanelPrivate(TransfersPanel *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~TransfersPanelPrivate() {}

public:
    TransfersPanel *q;
    TransfersView *m_transfersView{nullptr};
};

TransfersPanel::TransfersPanel(QWidget *parent)
    : Panel(parent)
    , d(new TransfersPanelPrivate(this))
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    d->m_transfersView = new TransfersView(this);
    layout->addWidget(d->m_transfersView);

    connect(d->m_transfersView, &TransfersView::bytesRead, this, &TransfersPanel::bytesRead);
    connect(d->m_transfersView, &TransfersView::bytesWritten, this, &TransfersPanel::bytesWritten);
    connect(this, &TransfersPanel::outputBytes, this, &TransfersPanel::outputBytes);
}

TransfersPanel::~TransfersPanel()
{
    delete d;
}

QVariantMap TransfersPanel::save() const
{
    return d->m_transfersView->save();
}

void TransfersPanel::load(const QVariantMap &parameters)
{
    d->m_transfersView->load(parameters);
}

void TransfersPanel::onBytesRead(const QByteArray &bytes, const QString &flag)
{
    d->m_transfersView->inputBytes(bytes);
}
