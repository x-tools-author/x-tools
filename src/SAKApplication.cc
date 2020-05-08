/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QQmlEngine>

#include "SAKDebugger.hh"
#include "SAKApplication.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerManager.hh"
#include "SAKDebuggerInputManager.hh"
#include "SAKDebuggerInputSettings.hh"
#include "SAKDebuggerDeviceSerialport.hh"

SAKApplication::SAKApplication(int argc, char **argv)
    : QGuiApplication (argc, argv)
{
    registerCustomType();
}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::registerCustomType()
{
    qmlRegisterUncreatableType<SAKDebugger>("SAK.CustomType", 1, 0, "SAKDebugger", "Can not insance the calss(SAKDebugger) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerDevice>("SAK.CustomType", 1, 0, "SAKDebuggerDevice", "Can not insance the calss(SAKDebugger) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerInputManager>("SAK.CustomType", 1, 0, "SAKDebuggerInputManager", "Can not insance the calss(SAKDebuggerInputManager) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerInputSettings>("SAK.CustomType", 1, 0, "SAKDebuggerInputSettings", "Can not insance the calss(SAKDebuggerInputSettings) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerDeviceSerialport>("SAK.CustomType", 1, 0, "SAKDebuggerDeviceSerialport", "Can not insance the calss(SAKDebuggerDeviceSerialport) on qml!");

    qmlRegisterSingletonType<SAKDebuggerManager>("SAK.CustomType", 1, 0, "SAKDebuggerManager", [](QQmlEngine *, QJSEngine *)->QObject*{return SAKDebuggerManager::instance();});
}

void SAKApplication::setupUi()
{
    qmlAppEngine.load(QUrl(QString("qrc:/qml/MainWindow.qml")));
}
