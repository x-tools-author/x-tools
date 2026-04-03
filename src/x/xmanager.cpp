/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xmanager.h"

#include <QAction>
#include <QDebug>

#include "common/xapp.h"
#include "log/log.h"
#include "x/common/xpage.h"
#include "x/tools/xtools.h"

#if X_ENABLE_X_BLE
#include "x/ble/xble.h"
#endif
#if X_ENABLE_X_MODBUS
#include "x/modbus/xmodbus.h"
#endif
#if X_ENABLE_X_CANBUS
#include "x/canbus/xcanbus.h"
#endif
#if X_ENABLE_X_MQTT
#include "x/mqtt/xmqtt.h"
#endif
#if X_ENABLE_X_COAP
#include "x/coap/xcoap.h"
#endif
#if X_ENABLE_X_OPCUA
#include "x/opcua/xopcua.h"
#endif
#if X_ENABLE_X_HTTP
#include "x/http/xhttp.h"
#endif
#if X_ENABLE_X_FLOW
#include "x/flow/xflow.h"
#endif

struct LayoutManagerKeys
{
    const QString xIndex{"xIndex"};

    const QString xTools{"xTools"};
    const QString xBle{"xBle"};
    const QString xModbus{"xModbus"};
    const QString xCanbus{"xCanbus"};
    const QString xMqtt{"xMqtt"};
    const QString xCoap{"xCoap"};
    const QString xOpcUa{"xOpcUa"};
    const QString xHttp{"xHttp"};

    const QString xFlow{"xFlow"};
    const QString xLog{"xLog"};
};

class xManagerPrivate : public QObject
{
public:
    explicit xManagerPrivate(xManager* parent = nullptr)
        : QObject(parent)
        , q(parent)
    {}

public:
    QStackedLayout* m_layout{nullptr};
    QButtonGroup* m_group{nullptr};
    QWidget* m_controller{nullptr};
    QHBoxLayout* m_controllerLayout{nullptr};
    QLabel* m_leftLabel{nullptr};
    QLabel* m_rightLabel{nullptr};
    QMenuBar* m_mainMenuBar{nullptr};
    QHash<QWidget*, QToolButton*> m_pageButtons;
    QList<QAction*> m_newWindowActions;

public:
    xTools::xTools* m_tools{nullptr};
#if X_ENABLE_X_BLE
    xBle::xBle* m_ble{nullptr};
#endif
#if X_ENABLE_X_MODBUS
    xModbus::xModbus* m_modbus{nullptr};
#endif
#if X_ENABLE_X_CANBUS
    xCanBus::xCanBus* m_canbus{nullptr};
#endif
#if X_ENABLE_X_MQTT
    xMqtt::xMqtt* m_mqtt{nullptr};
#endif
#if X_ENABLE_X_COAP
    xCoAP::xCoAP* m_coap{nullptr};
#endif
#if X_ENABLE_X_OPCUA
    xOpcUa::xOpcUa* m_opcua{nullptr};
#endif
#if X_ENABLE_X_HTTP
    xHttp::xHttp* m_http{nullptr};
#endif
#if X_ENABLE_X_FLOW
    xFlow::xFlow* m_flow{nullptr};
#endif

public:
    QToolButton* addLayoutPage(const QString& name, QWidget* page)
    {
        if (!m_layout) {
            qWarning("xManager: m_layout is null");
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

        if (dynamic_cast<xPage*>(page)) {
            xPage* layoutPage = dynamic_cast<xPage*>(page);
            QMenu* menu = layoutPage->toolButtonMenu();
            if (menu) {
                button->setMenu(menu);
                button->setPopupMode(QToolButton::MenuButtonPopup);
            }
        }

        return button;
    }

private:
    xManager* q{nullptr};
};

xManager::xManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent)
    : QObject(parent)
{
    d = new xManagerPrivate(this);
    d->m_layout = layout;
    d->m_mainMenuBar = menuBar;

    if (!d->m_layout) {
        qWarning("xManager: m_layout is null");
        return;
    }

    if (!d->m_mainMenuBar) {
        qWarning("xManager: menuBar is null");
        return;
    }

    d->m_controller = new QWidget();
    d->m_controllerLayout = new QHBoxLayout(d->m_controller);
    d->m_controllerLayout->setContentsMargins(2, 2, 2, 2);
    d->m_controller->setLayout(d->m_controllerLayout);
    d->m_leftLabel = new QLabel(d->m_controller);
    d->m_rightLabel = new QLabel(d->m_controller);
    d->m_controllerLayout->addWidget(d->m_leftLabel);
    d->m_controllerLayout->addWidget(d->m_rightLabel);

    d->m_group = new QButtonGroup(this);
#if defined(Q_OS_MACOS)
    menuBar->setNativeMenuBar(false);
#endif
    menuBar->setCornerWidget(d->m_controller, Qt::TopRightCorner);
    // ---------------------------------------------------------------------------------------------

    d->m_tools = new xTools::xTools(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xTools"), d->m_tools);
#if X_ENABLE_X_BLE
    d->m_ble = new xBle::xBle(d->m_layout->parentWidget());
    addLayoutPage(QString("xBLE"), d->m_ble);
#endif
#if X_ENABLE_X_MODBUS
    d->m_modbus = new xModbus::xModbus(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xModbus"), d->m_modbus);
#endif
#if X_ENABLE_X_CANBUS
    d->m_canbus = new xCanBus::xCanBus(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xCANBus"), d->m_canbus);
#endif
#if X_ENABLE_X_MQTT
    d->m_mqtt = new xMqtt::xMqtt(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xMQTT"), d->m_mqtt);
#endif
#if X_ENABLE_X_COAP
    d->m_coap = new xCoAP::xCoAP(d->m_layout->parentWidget());
    d->m_coap->setupSettings(xAPP->settings());
    d->addLayoutPage(QString("xCoAP"), d->m_coap);
#endif
#if X_ENABLE_X_OPCUA
    d->m_opcua = new xOpcUa::xOpcUa(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xOpcUa"), d->m_opcua);
#endif
#if X_ENABLE_X_HTTP
    d->m_http = new xHttp::xHttp(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xHTTP"), d->m_http);
#endif
#if X_ENABLE_X_FLOW
    d->m_flow = new xFlow::xFlow(d->m_layout->parentWidget());
    d->addLayoutPage(QString("xFlow"), d->m_flow);
#endif
    d->addLayoutPage(QString("xLog"), xLogMgr.logView());
    if (d->m_layout->count() == 1) {
        d->m_controller->hide();
    }
}

xManager::~xManager() {}

QJsonObject xManager::save()
{
    LayoutManagerKeys keys;
    QJsonObject obj;

    obj[keys.xIndex] = currentIndex();
    obj[keys.xTools] = d->m_tools ? d->m_tools->save() : QJsonObject();
#if X_ENABLE_X_MODBUS
    obj[keys.xModbus] = d->m_modbus ? d->m_modbus->save() : QJsonObject();
#endif
#if X_ENABLE_X_CANBUS
    obj[keys.xCanbus] = d->m_canbus ? d->m_canbus->save() : QJsonObject();
#endif
#if X_ENABLE_X_MQTT
    obj[keys.xMqtt] = d->m_mqtt ? d->m_mqtt->save() : QJsonObject();
#endif
#if X_ENABLE_X_COAP
    obj[keys.xCoap] = d->m_coap ? d->m_coap->save() : QJsonObject();
#endif
#if X_ENABLE_X_OPCUA
    obj[keys.xOpcUa] = d->m_opcua ? d->m_opcua->save() : QJsonObject();
#endif
#if X_ENABLE_X_HTTP
    obj[keys.xHttp] = d->m_http ? d->m_http->save() : QJsonObject();
#endif
#if X_ENABLE_X_FLOW
    obj[keys.xFlow] = d->m_flow ? d->m_flow->save() : QJsonObject();
#endif
    obj[keys.xLog] = xLogMgr.save();
    return obj;
}

void xManager::load(const QJsonObject& obj)
{
    LayoutManagerKeys keys;
    int index = obj.value(keys.xIndex).toInt(0);
    setCurrentIndex(index);
    qInfo() << "The current xApps layout index is:" << index;

    QJsonObject toolsObj = obj.value(keys.xTools).toObject(QJsonObject());
    if (d->m_tools) {
        d->m_tools->load(toolsObj);
    }
#if X_ENABLE_X_MODBUS
    if (d->m_modbus) {
        QJsonObject modbusObj = obj.value(keys.xModbus).toObject(QJsonObject());
        d->m_modbus->load(modbusObj);
    }
#endif
#if X_ENABLE_X_CANBUS
    if (d->m_canbus) {
        QJsonObject canbusObj = obj.value(keys.xCanbus).toObject(QJsonObject());
        d->m_canbus->load(canbusObj);
    }
#endif
#if X_ENABLE_X_MQTT
    if (d->m_mqtt) {
        QJsonObject mqttObj = obj.value(keys.xMqtt).toObject(QJsonObject());
        d->m_mqtt->load(mqttObj);
    }
#endif
#if X_ENABLE_X_COAP
    if (d->m_coap) {
        QJsonObject coapObj = obj.value(keys.xCoap).toObject(QJsonObject());
        d->m_coap->load(coapObj);
    }
#endif
#if X_ENABLE_X_OPCUA
    if (d->m_opcua) {
        QJsonObject opcuaObj = obj.value(keys.xOpcUa).toObject(QJsonObject());
        d->m_opcua->load(opcuaObj);
    }
#endif
#if X_ENABLE_X_HTTP
    if (d->m_http) {
        QJsonObject httpObj = obj.value(keys.xHttp).toObject(QJsonObject());
        d->m_http->load(httpObj);
    }
#endif
#if X_ENABLE_X_FLOW
    if (d->m_flow) {
        QJsonObject flowObj = obj.value(keys.xFlow).toObject(QJsonObject());
        d->m_flow->load(flowObj);
    }
#endif
    QJsonObject logObj = obj.value(keys.xLog).toObject(QJsonObject());
    xLogMgr.load(logObj);
}

template<typename T>
QAction* createNewWindowAction(xManager* manager, const QString& name)
{
    QAction* action = new QAction(name, manager);
    QObject::connect(action, &QAction::triggered, manager, [=]() {
        QWidget* w = new T();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->setWindowTitle(name);
        w->show();
    });
    return action;
}

int xManager::currentIndex() const
{
    if (!d->m_layout) {
        qWarning("xManager: m_layout is null");
        return 0;
    }

    return d->m_layout->currentIndex();
}

void xManager::setCurrentIndex(int index)
{
    if (!d->m_layout) {
        qWarning("xManager: m_layout is null");
        return;
    }

    if (index < 0 || index >= d->m_layout->count()) {
        index = 0;
    }

    d->m_layout->setCurrentIndex(index);
    QWidget* currentWidget = d->m_layout->currentWidget();
    QToolButton* currentButton = d->m_pageButtons.value(currentWidget, nullptr);
    if (currentButton) {
        currentButton->setChecked(true);
    }
}

QList<QAction*> xManager::newWindowActions()
{
    if (d->m_newWindowActions.isEmpty()) {
        QAction* a = nullptr;
#if X_ENABLE_X_MODBUS
        a = createNewWindowAction<xModbus::xModbus>(this, QString("xModbus"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_CANBUS
        a = createNewWindowAction<xCanBus::xCanBus>(this, QString("xCANBus"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_MQTT
        a = createNewWindowAction<xMqtt::xMqtt>(this, QString("xMQTT"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_COAP
        a = createNewWindowAction<xCoAP::xCoAP>(this, QString("xCoAP"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_OPCUA
        a = createNewWindowAction<xOpcUa::xOpcUa>(this, QString("xOpcUa"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_HTTP
        a = createNewWindowAction<xHttp::xHttp>(this, QString("xHTTP"));
        d->m_newWindowActions.append(a);
#endif
#if X_ENABLE_X_FLOW
        a = createNewWindowAction<xFlow::xFlow>(this, QString("xFlow"));
        d->m_newWindowActions.append(a);
#endif
    }

    return d->m_newWindowActions;
}
