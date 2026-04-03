/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xtools.h"

#include <QGridLayout>

#include "common/xapp.h"
#include "page/page.h"

namespace xTools {

struct xToolsKeys
{
    const QString windowGrid{"windowGrid"};
    const QString page00{"page00"};
    const QString page01{"page01"};
    const QString page10{"page10"};
    const QString page11{"page11"};
};

class xToolsPrivate : public QObject
{
public:
    xToolsPrivate(xTools *parent = nullptr)
        : QObject(parent)
        , q(parent)
    {}

public:
    Page *m_ioPage00{nullptr};
    Page *m_ioPage01{nullptr};
    Page *m_ioPage10{nullptr};
    Page *m_ioPage11{nullptr};
    int m_windowGrid{int(xTools::WindowGrid::Grid1x1)};

private:
    xTools *q{nullptr};
};

xTools::xTools(QWidget *parent)
    : xPage(parent)
{
    d = new xToolsPrivate(this);
    d->m_ioPage00 = new Page(Page::Left, xAPP->settings(), this);
    d->m_ioPage01 = new Page(Page::Right, xAPP->settings(), this);
    d->m_ioPage10 = new Page(Page::Left, xAPP->settings(), this);
    d->m_ioPage11 = new Page(Page::Right, xAPP->settings(), this);

    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->m_ioPage00, 0, 0);
    layout->addWidget(d->m_ioPage01, 0, 1);
    layout->addWidget(d->m_ioPage10, 1, 0);
    layout->addWidget(d->m_ioPage11, 1, 1);
    setLayout(layout);
    setWindowGrid(d->m_windowGrid);
}

xTools::~xTools() {}

QJsonObject xTools::save() const
{
    xToolsKeys keys;
    QJsonObject obj = xPage::save();
    obj.insert(keys.windowGrid, d->m_windowGrid);

    obj.insert(keys.page00, QJsonObject::fromVariantMap(d->m_ioPage00->save()));
    obj.insert(keys.page01, QJsonObject::fromVariantMap(d->m_ioPage01->save()));
    obj.insert(keys.page10, QJsonObject::fromVariantMap(d->m_ioPage10->save()));
    obj.insert(keys.page11, QJsonObject::fromVariantMap(d->m_ioPage11->save()));

    return obj;
}

void xTools::load(const QJsonObject &obj)
{
    xToolsKeys keys;
    int grid = obj.value(keys.windowGrid).toInt(int(WindowGrid::Grid1x1));
    setWindowGrid(grid);

    d->m_ioPage00->load(obj.value(keys.page00).toObject().toVariantMap());
    d->m_ioPage01->load(obj.value(keys.page01).toObject().toVariantMap());
    d->m_ioPage10->load(obj.value(keys.page10).toObject().toVariantMap());
    d->m_ioPage11->load(obj.value(keys.page11).toObject().toVariantMap());
}

int xTools::windowGrid() const
{
    return d->m_windowGrid;
}

void xTools::setWindowGrid(int grid)
{
    d->m_windowGrid = grid;
    if (grid == int(WindowGrid::Grid1x2)) {
        d->m_ioPage00->show();
        d->m_ioPage01->show();
        d->m_ioPage10->hide();
        d->m_ioPage11->hide();
    } else if (grid == int(WindowGrid::Grid2x1)) {
        d->m_ioPage00->show();
        d->m_ioPage01->hide();
        d->m_ioPage10->show();
        d->m_ioPage11->hide();
    } else if (grid == int(WindowGrid::Grid2x2)) {
        d->m_ioPage00->show();
        d->m_ioPage01->show();
        d->m_ioPage10->show();
        d->m_ioPage11->show();
    } else {
        d->m_ioPage00->show();
        d->m_ioPage01->hide();
        d->m_ioPage10->hide();
        d->m_ioPage11->hide();
    }
}

} // namespace xTools