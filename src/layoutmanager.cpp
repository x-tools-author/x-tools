/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "layoutmanager.h"

#include <QToolButton>

#if defined(X_ENABLE_XFLOW)
#include "nodeeditor/nodeeditor.h"
#endif

LayoutManager::LayoutManager(QStackedLayout* layout, QWidget* mw, QObject* parent)
    : QObject(parent)
    , m_layout(layout)
    , m_mainWindow(mw)
{
    m_controller = new QWidget(m_mainWindow);
    m_leftLabel = new QLabel(m_controller);
    m_rightLabel = new QLabel(m_controller);
    m_rightLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_hLayout = new QHBoxLayout(m_controller);
    m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->addWidget(m_leftLabel);
    m_hLayout->addWidget(m_rightLabel);
    m_controller->setLayout(m_hLayout);
    m_controller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_buttonGroup = new QButtonGroup(m_controller);
    connect(m_buttonGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* button) {
        auto buttons = m_buttonGroup->buttons();
        int index = buttons.indexOf(button);
        if (index != -1 && index < m_layout->count()) {
            m_layout->setCurrentIndex(index);
        }
    });

    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
    }
}

LayoutManager::~LayoutManager()
{
    // Clean up resources if needed
    m_controller->deleteLater();
}

void LayoutManager::addLayoutPage(const QString& name, QWidget* page)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return;
    }

    QToolButton* button = new QToolButton(m_controller);
    button->setText(name);
    button->setCheckable(true);
    m_hLayout->addWidget(button);
    m_buttonGroup->addButton(button);
    m_hLayout->removeWidget(m_rightLabel);
    m_hLayout->addWidget(m_rightLabel);
    m_layout->addWidget(page);
    if (m_layout->count() == 1) {
        button->setChecked(true);
    }
}

QWidget* LayoutManager::controller()
{
    return m_controller;
}

void LayoutManager::setupPages()
{
#if defined(X_ENABLE_XFLOW)
    m_nodeEditor = new xFlow::NodeEditor(m_mainWindow);
    addLayoutPage(tr("Node Editor"), m_nodeEditor);
#endif
}