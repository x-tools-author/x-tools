/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistview.h"
#include "ui_framelistview.h"

#include <QMessageBox>

#include "common/xtools.h"
#include "utilities/iconengine.h"

#include "framelistfilter.h"
#include "framelistmodel.h"

namespace Ui {
class FrameListView;
} // namespace Ui

namespace xCanBus {

struct FrameListViewParameterKeys
{
    const QString importFilePath{"importFilePath"};
    const QString exportFilePath{"exportFilePath"};
    const QString frameIdFilter{"frameIdFilter"};
    const QString payloadFilter{"payloadFilter"};
};

class FrameListViewPrivate : public QObject
{
public:
    FrameListViewPrivate(FrameListView *parent)
        : QObject(parent)
        , q(parent)
    {}

public:
    Ui::FrameListView *ui{nullptr};
    FrameListModel *m_model{nullptr};
    FrameListFilter *m_filter{nullptr};

private:
    FrameListView *q;
};

FrameListView::FrameListView(QWidget *parent)
    : CanBusPanel(parent)
{
    d = new FrameListViewPrivate(this);
    d->ui = new Ui::FrameListView;
    d->ui->setupUi(this);

    QHeaderView *hHeader = d->ui->tableView->horizontalHeader();
    hHeader->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    hHeader->setStretchLastSection(true);

    d->m_model = new FrameListModel(this);
    d->m_filter = new FrameListFilter(this);
    d->m_filter->setSourceModel(d->m_model);
    d->ui->tableView->setModel(d->m_filter);
}

FrameListView::~FrameListView()
{
    delete d->ui;
}

QJsonObject FrameListView::save() const
{
    QJsonObject obj;

    return obj;
}

void FrameListView::load(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}

} // namespace xCanBus