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
#include "SAKHelpManager.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKConsoleMessage.hh"
#include "SAKConsoleManager.hh"
#include "SAKDebuggerManager.hh"
#include "SAKHelp3rdController.hh"
#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerTextOutput.hh"
#include "SAKDebuggerInputManager.hh"
#include "SAKDebuggerInputSettings.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKHelpAboutQtController.hh"
#include "SAKHelpHistoryController.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKHelpAboutSAKController.hh"
#include "SAKDebuggerDeviceSerialport.hh"
#include "SAKDebuggerOutputStatistics.hh"

SAKApplication::SAKApplication(int argc, char **argv)
#ifdef Q_OS_ANDROID
    :QGuiApplication(argc, argv)
#else
    :QApplication(argc, argv)
#endif
{
    /// @brief 取消FileDialog警告
    setOrganizationName("Qter");
    setOrganizationDomain("IT");

    SAKConsoleManager::instance();
    registerCustomType();
}

SAKApplication::~SAKApplication()
{

}

void SAKApplication::registerCustomType()
{
    qmlRegisterUncreatableType<SAKDebugger>("SAK.CustomType", 1, 0, "SAKDebugger", "Can not insance the calss(SAKDebugger) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerDevice>("SAK.CustomType", 1, 0, "SAKDebuggerDevice", "Can not insance the calss(SAKDebugger) on qml!");
    qmlRegisterUncreatableType<SAKConsoleMessage>("SAK.CustomType", 1, 0, "SAKConsoleMessage", "Can not insance the calss(SAKConsoleMessage) on qml!");
    qmlRegisterUncreatableType<SAKHelp3rdController>("SAK.CustomType", 1, 0, "SAKHelp3rdController", "Can not insance the calss(SAKHelp3rdController) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerTextInput>("SAK.CustomType", 1, 0, "SAKDebuggerTextInput", "Can not insance the calss(SAKDebuggerTextInput) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerTextOutput>("SAK.CustomType", 1, 0, "SAKDebuggerTextOutput", "Can not insance the calss(SAKDebuggerTextOutput) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerInputManager>("SAK.CustomType", 1, 0, "SAKDebuggerInputManager", "Can not insance the calss(SAKDebuggerInputManager) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerInputSettings>("SAK.CustomType", 1, 0, "SAKDebuggerInputSettings", "Can not insance the calss(SAKDebuggerInputSettings) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerOutputManager>("SAK.CustomType", 1, 0, "SAKDebuggerOutputManager", "Can not insance the calss(SAKDebuggerOutputManager) on qml!");
    qmlRegisterUncreatableType<SAKHelpHistoryController>("SAK.CustomType", 1, 0, "SAKHelpHistoryController", "Can not insance the calss(SAKHelpHistoryController) on qml!");
    qmlRegisterUncreatableType<SAKHelpAboutQtController>("SAK.CustomType", 1, 0, "SAKHelpAboutQtController", "Can not insance the calss(SAKHelpAboutQtController) on qml!");
    qmlRegisterUncreatableType<SAKHelpAboutSAKController>("SAK.CustomType", 1, 0, "SAKHelpAboutSAKController", "Can not insance the calss(SAKHelpAboutSAKController) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerOutputSettings>("SAK.CustomType", 1, 0, "SAKDebuggerOutputSettings", "Can not insance the calss(SAKDebuggerOutputSettings) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerDeviceSerialport>("SAK.CustomType", 1, 0, "SAKDebuggerDeviceSerialport", "Can not insance the calss(SAKDebuggerDeviceSerialport) on qml!");
    qmlRegisterUncreatableType<SAKDebuggerOutputStatistics>("SAK.CustomType", 1, 0, "SAKDebuggerOutputStatistics", "Can not insance the calss(SAKDebuggerOutputStatistics) on qml!");

    qmlRegisterSingletonType<SAKHelpManager>("SAK.CustomType", 1, 0, "SAKHelpManager", [](QQmlEngine *, QJSEngine *)->QObject*{return SAKHelpManager::instance();});
    qmlRegisterSingletonType<SAKConsoleManager>("SAK.CustomType", 1, 0, "SAKConsoleManager", [](QQmlEngine *, QJSEngine *)->QObject*{return SAKConsoleManager::instance();});
    qmlRegisterSingletonType<SAKDebuggerManager>("SAK.CustomType", 1, 0, "SAKDebuggerManager", [](QQmlEngine *, QJSEngine *)->QObject*{return SAKDebuggerManager::instance();});
}

void SAKApplication::setupUi()
{
    qmlAppEngine.load(QUrl(QString("qrc:/qml/MainWindow.qml")));
}
