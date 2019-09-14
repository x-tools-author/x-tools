/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QFile>
#include <QTabBar>
#include <QAction>
#include <QMetaEnum>
#include <QSettings>
#include <QSpacerItem>
#include <QMessageBox>
#include <QStyleFactory>

#include "nslookup.h"
#include "SAKGlobal.hh"
#include "SAKVersion.hh"
#include "SAKConsole.hh"
#include "UpdateManager.h"
#include "CRCCalculator.hh"
#include "SAKMainWindow.hh"
#include "QtAppStyleApi.hh"
#include "SAKApplication.hh"
#include "SAKUdpDebugPage.hh"
#include "MoreInformation.hh"
#include "QtStyleSheetApi.hh"
#include "GetPublicIPWidget.h"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTabPageSerialportAssistant.hh"
#include "QtCryptographicHashController.hh"
#ifndef SAK_NO_SERIALPORT_ASSISTANT
#include "SAKTabPageSerialportAssistant.hh"
#endif

#include "ui_SAKMainWindow.h"

const static char* configureFile = "http://wuhai.pro/software/QtSwissArmyKnife/update.json";
const char* SAKMainWindow::appStyleKey = "Universal/appStyle";
SAKMainWindow::SAKMainWindow(QWidget *parent)
    :QMainWindow (parent)
    ,mpTabWidget (new QTabWidget)
    ,ui (new Ui::SAKMainWindow)
    ,versionDialog (new SAKVersion)
    ,moreInformation (new MoreInformation)
{
    ui->setupUi(this);
    updateManager = new UpdateManager(QUrl(configureFile), SAKVersion::instance()->getVersion());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);
    setWindowTitle(tr("瑞士军刀--开发调试工具集")
                   + " v" + versionDialog->getVersion()
                   + QString(" (QQ交流群%1 )").arg(versionDialog->getQQNum())
                   + QString(" 作者邮箱 %1").arg(versionDialog->getEmail()));

    this->resize(1280, 720);
    this->setMinimumWidth(1024);

    AddTab();
    InitMenu();
    AddTool();

    mpTabWidget->setTabsClosable(true);
    connect(mpTabWidget, &QTabWidget::tabCloseRequested, this, &SAKMainWindow::closeDebugPage);
}

SAKMainWindow::~SAKMainWindow()
{
    delete ui;
}

void SAKMainWindow::AddTab()
{
    // 串口助手
    this->mpTabWidget->addTab( new SAKTabPageSerialportAssistant, tr("串口调试"));
    // dup调试
    this->mpTabWidget->addTab(new SAKUdpDebugPage, tr("UDP调试"));
    // tcp客户单
    this->mpTabWidget->addTab(new SAKTcpClientDebugPage, tr("TCP客户端"));
    // tcp服务器
    this->mpTabWidget->addTab(new SAKTcpServerDebugPage, tr("TCP服务器"));
    // 终端输出
    this->mpTabWidget->addTab(new SAKConsole, tr("打印终端"));
}

void SAKMainWindow::AddTool()
{
    addTool(tr("域名转IP"),        new ToolsNsLookup);
    addTool(tr("公网IP获取工具"),   new GetPublicIPWidget);
    addTool(tr("文件校验工具"),     new QtCryptographicHashController);

    QAction *action = nullptr;
    action = new QAction("CRC计算器", this);
    toolsMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKMainWindow::createCRCCalculator);
}

void SAKMainWindow::InitMenu()
{
    // 文件菜单
    QMenu *pFileMenu = new QMenu(tr("文件"));
    menuBar()->addMenu(pFileMenu);

    QMenu *tabMenu = new QMenu(tr("新建页面"), this);
    pFileMenu->addMenu(tabMenu);
    QMetaEnum enums = QMetaEnum::fromType<SAKGlobal::SAKEnumIODeviceType>();
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        a->setData(var);
        connect(a, &QAction::triggered, this, &SAKMainWindow::addRemovablePage);
        tabMenu->addAction(a);
    }

    QMenu *windowMenu = new QMenu(tr("新建窗口"), this);
    pFileMenu->addMenu(windowMenu);
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        connect(a, &QAction::triggered, this, &SAKMainWindow::openIODeviceWindow);
        a->setData(var);
        windowMenu->addAction(a);
    }

    pFileMenu->addSeparator();

    QAction *pExitAction = new QAction(tr("退出"));
    pFileMenu->addAction(pExitAction);
    connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    // 工具菜单
    QMenu *pToolMenu = new QMenu(tr("工具"));
    menuBar()->addMenu(pToolMenu);
    toolsMenu = pToolMenu;

    // 选项
    QMenu *optionMenu = new QMenu(tr("选项"));
    menuBar()->addMenu(optionMenu);

    // 选项菜单--皮肤切换菜单
    initSkinMenu(optionMenu);

#if 1
    QSettings settings;
    QString value = settings.value(appStyleKey).toString();
    // 默认使用Qt支持的软件风格的第一种软件风格
    if (value.isEmpty()){
        foreach (QString style,  QStyleFactory::keys()){
            value = style;
            break;
        }
    }

    QMenu *appStyleMenu = new QMenu(tr("软件风格"));
    optionMenu->addMenu(appStyleMenu);
    appStyleMenu->addActions(QtAppStyleApi::instance()->actions());
#endif

    // 帮助菜单
    QMenu *pHelpMenu = new QMenu(tr("帮助"));
    menuBar()->addMenu(pHelpMenu);

    QAction *pAboutQtAction = new QAction(tr("关于Qt"));
    pHelpMenu->addAction(pAboutQtAction);
    connect(pAboutQtAction, SIGNAL(triggered(bool)), this, SLOT(AboutQt()));

    QAction *pAboutAction = new QAction(tr("关于软件"));
    pHelpMenu->addAction(pAboutAction);
    connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(About()));

    QAction *pUpdateAction = new QAction(tr("检查更新"));
    pHelpMenu->addAction(pUpdateAction);
    connect(pUpdateAction, SIGNAL(triggered(bool)), updateManager, SLOT(checkForUpdate()));

    QAction *pMoreInformation = new QAction(tr("更多信息"));
    pHelpMenu->addAction(pMoreInformation);
    connect(pMoreInformation, SIGNAL(triggered(bool)), moreInformation, SLOT(show()));
}

void SAKMainWindow::About()
{
    versionDialog->show();
    versionDialog->activateWindow();
}

void SAKMainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void SAKMainWindow::addTool(QString toolName, QWidget *toolWidget)
{
    QAction *action = new QAction(toolName);
    toolsMenu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), toolWidget, SLOT(show()));
}

void SAKMainWindow::changeStylesheet()
{
    QFile file;
    QString skin = sender()->objectName();

    QSettings setting;
    setting.setValue(QString(appStylesheetKey), skin);

    if (skins.keyToValue(skin.toLatin1().data()) == QtDefault){
        qApp->setStyleSheet("");
        return;
    }else if (skins.keyToValue(skin.toLatin1().data()) == FlatWhite){
        file.setFileName(":/qss/flatwhite.css");
    }else if (skins.keyToValue(skin.toLatin1().data()) == LightBlue){
        file.setFileName(":/qss/lightblue.css");
    }else if (skins.keyToValue(skin.toLatin1().data()) == PSBlack){
        file.setFileName(":/qss/psblack.css");
    }else {

    }

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void SAKMainWindow::initSkinMenu(QMenu *optionMenu)
{
    skins = QMetaEnum::fromType<SAKStyleSheet>();
    QSettings settings;
    QString value = settings.value(appStylesheetKey).toString();
    if (value.isEmpty()){
        value = QString(skins.valueToKey(LightBlue));
    }

    QMenu *stylesheetMenu = new QMenu("皮肤");
    optionMenu->addMenu(stylesheetMenu);
    stylesheetMenu->addActions(QtStyleSheetApi::instance()->actions());
}

void SAKMainWindow::addRemovablePage()
{
    int type = qobject_cast<QAction*>(sender())->data().value<int>();

    QWidget *widget = getDebugPage(type);
    mpTabWidget->addTab(widget, sender()->objectName());
}

void SAKMainWindow::openIODeviceWindow()
{
    int type = qobject_cast<QAction*>(sender())->data().value<int>();

    QWidget *widget = getDebugPage(type);
    widget->show();
}

QWidget *SAKMainWindow::getDebugPage(int type)
{
    QWidget *widget = nullptr;
    switch (type) {
    case SAKGlobal::SAKEnumIODeviceTypeUDP:
        widget = new SAKUdpDebugPage;
        break;
    case SAKGlobal::SAKEnumIODeviceTypeTCPClient:
        widget = new SAKTcpClientDebugPage;
        break;
    case SAKGlobal::SAKEnumIODeviceTypeTCPServer:
        widget = new SAKTcpServerDebugPage;
        break;

#ifndef SAK_NO_SERIALPORT_ASSISTANT
    case SAKGlobal::SAKEnumIODeviceTypeSerialport:
        widget = new SAKTabPageSerialportAssistant;
        break;
#endif

    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow window type.");
        break;
    }

    widget->setAttribute(Qt::WA_DeleteOnClose, true);
    return widget;
}

void SAKMainWindow::closeDebugPage(int index)
{
    if (index < 5){
        return;
    }else{
        QWidget *w = mpTabWidget->widget(index);
        mpTabWidget->removeTab(index);
        w->close();
    }
}

void SAKMainWindow::createCRCCalculator()
{
    CRCCalculator *cal = new CRCCalculator;
    cal->show();
}
