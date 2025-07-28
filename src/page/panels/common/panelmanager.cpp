/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "panelmanager.h"

#include <QMetaObject>

#include "panel.h"

PanelManager::PanelManager(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QStackedLayout(this);
    setLayout(m_layout);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_panelButton = new QToolButton(this);
    m_panelButton->setText(tr("Panels"));
    m_panelButton->setIcon(QIcon(":/res/icons/dock_to_right.svg"));
    m_panelButton->setCheckable(true);
    m_panelButton->setToolTip(tr("Hide/Show Panels"));
    m_panelButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_panelButton->setChecked(false);
    connect(m_panelButton, &QToolButton::clicked, this, [this](bool checked) {
        this->setVisible(checked);
        emit visibleChanged(checked);
    });

    m_layout->setCurrentIndex(0);
    setVisible(false);
}

PanelManager::~PanelManager() {}

QVariantMap PanelManager::save() const
{
    QVariantMap map;
    for (const Panel *panel : std::as_const(m_panels)) {
        map[panel->metaObject()->className()] = panel->save();
    }
    return map;
}

void PanelManager::load(const QVariantMap &parameters)
{
    for (Panel *&panel : m_panels) {
        const QString className = panel->metaObject()->className();
        if (parameters.contains(className)) {
            panel->load(parameters.value(className).toMap());
        }
    }
}

QList<QToolButton *> PanelManager::buttons() const
{
    QList<QAbstractButton *> buttons = m_buttonGroup.buttons();
    QList<QToolButton *> buttonList;
    buttonList.append(m_panelButton); // Include the panel button
    for (const auto &button : m_buttonGroup.buttons()) {
        if (qobject_cast<QToolButton *>(button) == nullptr) {
            continue; // Skip if the button is not a QToolButton
        }

        buttonList.append(qobject_cast<QToolButton *>(button));
    }

    return buttonList;
}