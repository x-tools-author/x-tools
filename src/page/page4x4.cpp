/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "page4x4.h"

#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>

#include "page.h"

Page4x4::Page4x4(QSettings* settings, QWidget* parent)
    : QWidget(parent)
    , m_settings(settings)
{
    m_iopage00 = new Page(Page::Left, settings, this);
    m_iopage01 = new Page(Page::Right, settings, this);
    m_iopage10 = new Page(Page::Left, settings, this);
    m_iopage11 = new Page(Page::Right, settings, this);

    auto* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_iopage00, 0, 0);
    layout->addWidget(m_iopage01, 0, 1);
    layout->addWidget(m_iopage10, 1, 0);
    layout->addWidget(m_iopage11, 1, 1);

    const int defaultGrid = static_cast<int>(WindowGrid::Grid1x1);
    int rawGrid = settings->value(m_keys.windowGrid, defaultGrid).toInt();
    m_windowGrid = static_cast<WindowGrid>(rawGrid);
    updateGrid(m_windowGrid);
    qInfo() << "The value of window grid is:" << static_cast<int>(rawGrid);

    initActions();
}

Page4x4::~Page4x4() {}

void Page4x4::updateGrid(WindowGrid grid)
{
    if (grid == WindowGrid::Grid1x2) {
        m_iopage00->show();
        m_iopage01->show();
        m_iopage10->hide();
        m_iopage11->hide();
    } else if (grid == WindowGrid::Grid2x1) {
        m_iopage00->show();
        m_iopage01->hide();
        m_iopage10->show();
        m_iopage11->hide();
    } else if (grid == WindowGrid::Grid2x2) {
        m_iopage00->show();
        m_iopage01->show();
        m_iopage10->show();
        m_iopage11->show();
    } else {
        m_iopage00->show();
        m_iopage01->hide();
        m_iopage10->hide();
        m_iopage11->hide();
    }

    m_windowGrid = grid;
    m_settings->setValue(m_keys.windowGrid, static_cast<int>(grid));
}

void Page4x4::load(const QVariantMap& parameters) const
{
    m_iopage00->load(parameters.value(m_keys.page00).toMap());
    m_iopage01->load(parameters.value(m_keys.page01).toMap());
    m_iopage10->load(parameters.value(m_keys.page10).toMap());
    m_iopage11->load(parameters.value(m_keys.page11).toMap());
}

QVariantMap Page4x4::save() const
{
    QVariantMap obj;
    obj.insert(m_keys.page00, m_iopage00->save());
    obj.insert(m_keys.page01, m_iopage01->save());
    obj.insert(m_keys.page10, m_iopage10->save());
    obj.insert(m_keys.page11, m_iopage11->save());
    return obj;
}

int Page4x4::windowGrid() const
{
    return static_cast<int>(m_windowGrid);
}

QList<QAction*> Page4x4::actions() const
{
    return m_group->actions();
}

void Page4x4::initActions()
{
    if (!m_group) {
        m_group = new QActionGroup(this);
    }

    int g1x1 = static_cast<int>(Page4x4::WindowGrid::Grid1x1);
    int g1x2 = static_cast<int>(Page4x4::WindowGrid::Grid1x2);
    int g2x1 = static_cast<int>(Page4x4::WindowGrid::Grid2x1);
    int g2x2 = static_cast<int>(Page4x4::WindowGrid::Grid2x2);
    QAction* a1x1 = new QAction("1x1", this);
    QAction* a1x2 = new QAction("1x2", this);
    QAction* a2x1 = new QAction("2x1", this);
    QAction* a2x2 = new QAction("2x2", this);

    connect(a1x1, &QAction::triggered, this, [=]() { updateGrid(static_cast<WindowGrid>(g1x1)); });
    connect(a1x2, &QAction::triggered, this, [=]() { updateGrid(static_cast<WindowGrid>(g1x2)); });
    connect(a2x1, &QAction::triggered, this, [=]() { updateGrid(static_cast<WindowGrid>(g2x1)); });
    connect(a2x2, &QAction::triggered, this, [=]() { updateGrid(static_cast<WindowGrid>(g2x2)); });

    a1x1->setObjectName("PageViewAction");
    a1x2->setObjectName("PageViewAction");
    a2x1->setObjectName("PageViewAction");
    a2x2->setObjectName("PageViewAction");

    a1x1->setCheckable(true);
    a1x2->setCheckable(true);
    a2x1->setCheckable(true);
    a2x2->setCheckable(true);

    m_group->addAction(a1x1);
    m_group->addAction(a1x2);
    m_group->addAction(a2x1);
    m_group->addAction(a2x2);

    int windowGrid = m_settings->value(m_keys.windowGrid, g1x1).toInt();
    updateGrid(static_cast<WindowGrid>(windowGrid));
    if (windowGrid == g1x2) {
        a1x2->setChecked(true);
    } else if (windowGrid == g2x1) {
        a2x1->setChecked(true);
    } else if (windowGrid == g2x2) {
        a2x2->setChecked(true);
    } else {
        a1x1->setChecked(true);
    }
}