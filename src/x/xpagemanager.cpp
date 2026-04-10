/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xpagemanager.h"

#include <QAction>
#include <QDebug>

#include "common/xapp.h"
#include "log/log.h"
#include "x/log/xlog.h"
#include "x/tools/xtools.h"
#include "x/xpage.h"

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

class xPageManagerPrivate : public QObject
{
public:
    explicit xPageManagerPrivate(xPageManager* parent = nullptr)
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
    struct PageInfo
    {
        QString name;
        xPage* page{nullptr};
    };
    QList<PageInfo> m_pages;

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
    xLog::xLog* m_log{nullptr};

public:
    QToolButton* addLayoutPage(const QString& name, QWidget* page)
    {
        if (!m_layout) {
            qWarning("xPageManager: m_layout is null");
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

    void appendPage(const QString& name, xPage* page)
    {
        if (!page) {
            qWarning("xPageManager: page is null");
            return;
        }

        addLayoutPage(name, page);

        PageInfo pageInfo;
        pageInfo.name = name;
        pageInfo.page = page;
        m_pages.append(pageInfo);
    }

private:
    xPageManager* q{nullptr};
};

xPageManager::xPageManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent)
    : QObject(parent)
{
    d = new xPageManagerPrivate(this);
    d->m_layout = layout;
    d->m_mainMenuBar = menuBar;

    if (!d->m_layout) {
        qWarning("xPageManager: m_layout is null");
        return;
    }

    if (!d->m_mainMenuBar) {
        qWarning("xPageManager: menuBar is null");
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
    d->appendPage(QString("xTools"), d->m_tools);
#if X_ENABLE_X_BLE
    d->m_ble = new xBle::xBle(d->m_layout->parentWidget());
    d->appendPage(QString("xBLE"), d->m_ble);
#endif
#if X_ENABLE_X_MODBUS
    d->m_modbus = new xModbus::xModbus(d->m_layout->parentWidget());
    d->appendPage(QString("xModbus"), d->m_modbus);
#endif
#if X_ENABLE_X_CANBUS
    d->m_canbus = new xCanBus::xCanBus(d->m_layout->parentWidget());
    d->appendPage(QString("xCANBus"), d->m_canbus);
#endif
#if X_ENABLE_X_MQTT
    d->m_mqtt = new xMqtt::xMqtt(d->m_layout->parentWidget());
    d->appendPage(QString("xMQTT"), d->m_mqtt);
#endif
#if X_ENABLE_X_COAP
    d->m_coap = new xCoAP::xCoAP(d->m_layout->parentWidget());
    d->m_coap->setupSettings(xAPP->settings());
    d->appendPage(QString("xCoAP"), d->m_coap);
#endif
#if X_ENABLE_X_OPCUA
    d->m_opcua = new xOpcUa::xOpcUa(d->m_layout->parentWidget());
    d->appendPage(QString("xOpcUa"), d->m_opcua);
#endif
#if X_ENABLE_X_HTTP
    d->m_http = new xHttp::xHttp(d->m_layout->parentWidget());
    d->appendPage(QString("xHTTP"), d->m_http);
#endif
#if X_ENABLE_X_FLOW
    d->m_flow = new xFlow::xFlow(d->m_layout->parentWidget());
    d->appendPage(QString("xFlow"), d->m_flow);
#endif
    d->m_log = new xLog::xLog(d->m_layout->parentWidget());
    d->appendPage(QString("xLog"), d->m_log);
}

xPageManager::~xPageManager() {}

QJsonObject xPageManager::save()
{
    LayoutManagerKeys keys;
    QJsonObject obj;
    obj.insert(keys.xIndex, currentIndex());

    for (const auto& pageInfo :
         const_cast<const QList<xPageManagerPrivate::PageInfo>&>(d->m_pages)) {
        if (!pageInfo.page) {
            continue;
        }

        QJsonObject pageObj = pageInfo.page->save();
        obj.insert(pageInfo.name, pageObj);
    }

    return obj;
}

void xPageManager::load(const QJsonObject& obj)
{
    LayoutManagerKeys keys;
    int index = obj.value(keys.xIndex).toInt(0);
    setCurrentIndex(index);

    for (const auto& pageInfo :
         const_cast<const QList<xPageManagerPrivate::PageInfo>&>(d->m_pages)) {
        if (!pageInfo.page) {
            continue;
        }

        QJsonObject pageObj = obj.value(pageInfo.name).toObject();
        pageInfo.page->load(pageObj);
    }
}

template<typename T>
QAction* createNewWindowAction(xPageManager* manager, const QString& name)
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

int xPageManager::currentIndex() const
{
    if (!d->m_layout) {
        qWarning("xPageManager: m_layout is null");
        return 0;
    }

    return d->m_layout->currentIndex();
}

void xPageManager::setCurrentIndex(int index)
{
    if (!d->m_layout) {
        qWarning("xPageManager: m_layout is null");
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

QList<QAction*> xPageManager::newWindowActions()
{
    if (d->m_newWindowActions.isEmpty()) {
        QAction* a = nullptr;
        a = createNewWindowAction<xTools::xTools>(this, QString("xTools"));
        d->m_newWindowActions.append(a);
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

xTools::xTools* xPageManager::xTools()
{
    return d->m_tools;
}

void xPageManager::showLiteMode()
{
    setCurrentIndex(0);
    d->m_tools->showLiteMode();
}