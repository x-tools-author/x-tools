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

#if X_ENABLE_X_FLOW
#include "x/flow/xflow.h"
#endif

#if X_ENABLE_X_MODBUS
#include "x/modbus/xmodbus.h"
#endif

#if X_ENABLE_X_MQTT
#include "x/mqtt/xmqtt.h"
#endif

#if X_ENABLE_X_CANBUS
#include "x/canbus/xcanbus.h"
#endif

struct LayoutManagerKeys
{
    const QString xIndex{"xIndex"};
    const QString xModbus{"xModbus"};
    const QString xFlow{"xFlow"};
};

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
    m_pageButtons.insert(page, button);
    connect(button, &QToolButton::clicked, this, [=]() { m_layout->setCurrentWidget(page); });

    m_controllerLayout->removeWidget(m_rightLabel);
    m_controllerLayout->addWidget(m_rightLabel);

    return button;
}

void LayoutManager::setupPages()
{
#if X_ENABLE_X_MODBUS
    m_modbus = new xModbus::xModbus(m_layout->parentWidget());
    addLayoutPage(QString("xModbus"), m_modbus);
#endif
#if X_ENABLE_X_MQTT
    m_mqtt = new xMqtt::xMqtt(m_layout->parentWidget());
    addLayoutPage(QString("xMQTT"), m_mqtt);
#endif
#if X_ENABLE_X_CANBUS
    m_canbus = new xCanBus::xCanBus(m_layout->parentWidget());
    addLayoutPage(QString("xCANBus"), m_canbus);
#endif
#if X_ENABLE_X_FLOW
    m_flow = new xFlow::xFlow(m_layout->parentWidget());
    addLayoutPage(QString("xFlow"), m_flow);
#endif

    if (m_layout->count() == 1) {
        m_controller->hide();
    }
}

int LayoutManager::currentIndex() const
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return 0;
    }

    return m_layout->currentIndex();
}

void LayoutManager::setCurrentIndex(int index)
{
    if (!m_layout) {
        qWarning("LayoutManager: m_layout is null");
        return;
    }

    if (index < 0 || index >= m_layout->count()) {
        index = 0;
    }

    m_layout->setCurrentIndex(index);
    QWidget* currentWidget = m_layout->currentWidget();
    QToolButton* currentButton = m_pageButtons.value(currentWidget, nullptr);
    if (currentButton) {
        currentButton->setChecked(true);
    }
}

QJsonObject LayoutManager::save()
{
    LayoutManagerKeys keys;
    QJsonObject obj;

#if X_ENABLE_X_MODBUS
    obj[keys.xIndex] = currentIndex();
    obj[keys.xModbus] = m_modbus ? m_modbus->save() : QJsonObject();
#endif
#if X_ENABLE_X_FLOW
    obj[keys.xFlow] = m_flow ? m_flow->save() : QJsonObject();
#endif

    return obj;
}

void LayoutManager::load(const QJsonObject& obj)
{
    LayoutManagerKeys keys;
    int index = obj.value(keys.xIndex).toInt(0);
    setCurrentIndex(index);

#if X_ENABLE_X_MODBUS
    if (m_modbus) {
        QJsonObject modbusObj = obj.value(keys.xModbus).toObject(QJsonObject());
        m_modbus->load(modbusObj);
    }
#endif
#if X_ENABLE_X_FLOW
    if (m_flow) {
        QJsonObject flowObj = obj.value(keys.xFlow).toObject(QJsonObject());
        m_flow->load(flowObj);
    }
#endif
}

void LayoutManager::outputBytes(const QString& txt, int channel)
{
#if X_ENABLE_X_FLOW
    if (m_flow) {
        m_flow->outputBytes(txt, channel);
    }
#endif
}

void LayoutManager::clearOutput(int channel)
{
#if X_ENABLE_X_FLOW
    if (m_flow) {
        m_flow->clearOutput(channel);
    }
#endif
}
