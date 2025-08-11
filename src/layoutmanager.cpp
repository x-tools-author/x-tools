/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "layoutmanager.h"

#include <QAction>

#if defined(X_ENABLE_XFLOW)
#include "nodeeditor/nodeeditor.h"
#endif

LayoutManager::LayoutManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent)
    : QObject(parent)
    , m_layout(layout)
    , m_mainMenuBar(menuBar)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
    }

    if (menuBar) {
        QLabel* label = new QLabel(tr("Layout:"), menuBar);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    m_group = new QActionGroup(this);
    m_group->setExclusive(true);
}

LayoutManager::~LayoutManager() {}

QAction* LayoutManager::addLayoutPage(const QString& name, QWidget* page)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return nullptr;
    }

    QAction* action = new QAction(name, this);
    action->setCheckable(true);
    m_group->addAction(action);
    m_layout->addWidget(page);
    m_mainMenuBar->addAction(action);
    connect(action, &QAction::triggered, this, [=]() { m_layout->setCurrentWidget(page); });

    return action;
}

void LayoutManager::setupPages()
{
#if defined(X_ENABLE_XFLOW)
    m_nodeEditor = new xFlow::NodeEditor(m_layout->parentWidget());
    addLayoutPage(tr("Node Editor"), m_nodeEditor);
#endif
}
