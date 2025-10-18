import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "pages/about"
import "pages/canbus"
import "pages/flow"
import "pages/links"
import "pages/log"
import "pages/mqtt"
import "pages/settings"
import "pages/tools"
import "pages/modbus"

StackLayout {
    id: root

    LinksPage {}
    ModbusPage {}
    MqttPage {}
    CANBusPage {}
    FlowPage {}

    ToolsPage {}
    LogPage {}
    SettingsPage {}
    AboutPage {}
}
