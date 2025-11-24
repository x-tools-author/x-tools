/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHash>
#include <QJsonObject>
#include <QLabel>
#include <QMenuBar>
#include <QObject>
#include <QStackedLayout>
#include <QToolButton>
#include <QWidget>

namespace xMqtt {
class xMqtt;
}

#if X_ENABLE_X_CANBUS
namespace xCanBus {
class xCanBus;
}
#endif
#if X_ENABLE_X_MODBUS
namespace xModbus {
class xModbus;
}
#endif
#if X_ENABLE_X_FLOW
namespace xFlow {
class xFlow;
}
#endif

class LayoutManager : public QObject
{
    Q_OBJECT
public:
    explicit LayoutManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent = nullptr);
    ~LayoutManager() override;

    QToolButton* addLayoutPage(const QString& name, QWidget* page);
    void setupPages();
    int currentIndex() const;
    void setCurrentIndex(int index);

    QJsonObject save();
    void load(const QJsonObject& obj);

    void outputBytes(const QString& txt, int channel);
    void clearOutput(int channel);

private:
    QStackedLayout* m_layout{nullptr};
    QButtonGroup* m_group{nullptr};
    QWidget* m_controller{nullptr};
    QHBoxLayout* m_controllerLayout{nullptr};
    QLabel* m_leftLabel{nullptr};
    QLabel* m_rightLabel{nullptr};
    QMenuBar* m_mainMenuBar{nullptr};
    QHash<QWidget*, QToolButton*> m_pageButtons;
#if X_ENABLE_X_FLOW
    xFlow::xFlow* m_flow{nullptr};
#endif
#if X_ENABLE_X_MODBUS
    xModbus::xModbus* m_modbus{nullptr};
#endif
    xMqtt::xMqtt* m_mqtt{nullptr};
#if X_ENABLE_X_CANBUS
    xCanBus::xCanBus* m_canbus{nullptr};
#endif
};
