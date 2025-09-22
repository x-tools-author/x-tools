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

#if X_ENABLE_X_NODE_EDITOR
#include "nodeeditor/nodeeditor.h"
#endif

#if X_ENABLE_X_MODBUS
#include "modbus/xmodbus.h"
#endif

#if X_ENABLE_X_MQTT
#include "mqtt/xmqtt.h"
#endif

#if X_ENABLE_X_CANBUS
#include "canbus/xcanbus.h"
#endif

LayoutManager::LayoutManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent)
    : QObject(parent)
    , m_layout(layout)
    , m_mainMenuBar(menuBar)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return;
    }

    if (!menuBar) {
        qWarning("LayoutManager: menuBar is null");
        return;
    }

    m_controller = new QWidget();
    m_controllerLayout = new QHBoxLayout(m_controller);
    m_controller->setLayout(m_controllerLayout);
    m_leftLabel = new QLabel(m_controller);
    m_rightLabel = new QLabel(m_controller);
    m_controllerLayout->addWidget(m_leftLabel);
    m_controllerLayout->addWidget(m_rightLabel);

    m_group = new QButtonGroup(this);
    menuBar->setCornerWidget(m_controller, Qt::TopRightCorner);
}

LayoutManager::~LayoutManager() {}

QToolButton* LayoutManager::addLayoutPage(const QString& name, QWidget* page)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return nullptr;
    }

    QToolButton* button = new QToolButton();
    button->setText(name);
    button->setCheckable(true);
    m_group->addButton(button);
    m_layout->addWidget(page);
    m_controllerLayout->addWidget(button);
    connect(button, &QToolButton::clicked, this, [=]() { m_layout->setCurrentWidget(page); });

    m_controllerLayout->removeWidget(m_rightLabel);
    m_controllerLayout->addWidget(m_rightLabel);

    return button;
}

void LayoutManager::setupPages()
{
#if X_ENABLE_X_NODE_EDITOR
    m_nodeEditor = new xFlow::NodeEditor(m_layout->parentWidget());
    addLayoutPage(tr("Node Editor"), m_nodeEditor);
#endif
#if X_ENABLE_X_MODBUS
    m_modbus = new xModbus::xModbus(m_layout->parentWidget());
    addLayoutPage(QString("Modbus"), m_modbus);
#endif
#if X_ENABLE_X_MQTT
    m_mqtt = new xMqtt::xMqtt(m_layout->parentWidget());
    addLayoutPage(QString("MQTT"), m_mqtt);
#endif
#if X_ENABLE_X_CANBUS
    m_canbus = new xCanBus::xCanBus(m_layout->parentWidget());
    addLayoutPage(QString("CAN Bus"), m_canbus);
#endif

    if (m_layout->count() == 1) {
        m_controller->hide();
    }
}
