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
#include <QVariant>
#include <QSysInfo>
#include <QMetaEnum>
#include <QSettings>
#include <QSpacerItem>
#include <QMessageBox>
#include <QStyleFactory>
#include <QDesktopServices>

#include "nslookup.h"
#include "SAKGlobal.hh"
#include "SAKVersion.hh"
#include "SAKConsole.hh"
#include "SAKSettings.hh"
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
#include "QtCryptographicHashController.hh"

#ifdef SAK_IMPORT_COM_MODULE
#include "SAKSerialPortDebugPage.hh"
#endif

#ifdef SAK_IMPORT_HID_MODULE
#include "SAKHidDebugPage.hh"
#endif

#ifdef SAK_IMPORT_USB_MODULE
#include "SAKUsbDebugPage.hh"
#endif

#include "ui_SAKMainWindow.h"

const static char* configureFile = "http://wuhai.pro/software/QtSwissArmyKnife/update.json";
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

    this->resize(800, 600);

    mpTabWidget->setTabsClosable(true);
    connect(mpTabWidget, &QTabWidget::tabCloseRequested, this, &SAKMainWindow::closeDebugPage);    

    AddTab();
    initMenu();
    AddTool();   

    connect(QtStyleSheetApi::instance(), &QtStyleSheetApi::styleSheetChanged, this, &SAKMainWindow::changeStylesheet);
    connect(QtAppStyleApi::instance(), &QtAppStyleApi::appStyleChanged, this, &SAKMainWindow::changeAppStyle);
}

SAKMainWindow::~SAKMainWindow()
{
    delete ui;
}

void SAKMainWindow::AddTab()
{
    /*
     * 添加调试页面
     */
#ifdef SAK_IMPORT_COM_MODULE
    this->mpTabWidget->addTab( new SAKSerialPortDebugPage, tr("串口调试"));
#endif
    this->mpTabWidget->addTab(new SAKUdpDebugPage, tr("UDP调试"));
    this->mpTabWidget->addTab(new SAKTcpClientDebugPage, tr("TCP客户端"));
    this->mpTabWidget->addTab(new SAKTcpServerDebugPage, tr("TCP服务器"));
#ifdef SAK_ENABLE_USB_DEBUG
    this->mpTabWidget->addTab(new SAKUsbDebugPage, tr("USB调试"));
#endif
#ifdef SAK_IMPORT_HID_MODULE
    this->mpTabWidget->addTab(new SAKHidDebugPage, tr("HID调试"));
#endif
    this->mpTabWidget->addTab(new SAKConsole, tr("打印终端"));

    /*
     * 隐藏关闭按钮（必须在调用setTabsClosable()函数后设置，否则不生效）
     */
    for (int i = 0; i < mpTabWidget->count(); i++){
        mpTabWidget->tabBar()->setTabButton(i, QTabBar::RightSide, nullptr);
        mpTabWidget->tabBar()->setTabButton(i, QTabBar::LeftSide, nullptr);
    }
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

void SAKMainWindow::About()
{
    versionDialog->show();
    versionDialog->activateWindow();
}

void SAKMainWindow::addTool(QString toolName, QWidget *toolWidget)
{
    QAction *action = new QAction(toolName);
    toolsMenu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), toolWidget, SLOT(show()));
}

void SAKMainWindow::changeStylesheet(QString styleSheetName)
{
    SAKSettings::instance()->setValue(appStylesheetKey, styleSheetName);
    if (!styleSheetName.isEmpty()){
        defaultStyleSheetAction->setChecked(false);
    }
}

void SAKMainWindow::changeAppStyle(QString appStyle)
{
    SAKSettings::instance()->setValue(appStyleKey, appStyle);
}

void SAKMainWindow::initMenu()
{
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initHelpMenu();
}

void SAKMainWindow::initFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("文件"), this);
    menuBar()->addMenu(fileMenu);

    QMenu *tabMenu = new QMenu(tr("新建页面"), this);
    fileMenu->addMenu(tabMenu);
    QMetaEnum enums = QMetaEnum::fromType<SAKGlobal::SAKEnumDebugPageType>();
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        a->setData(var);
        connect(a, &QAction::triggered, this, &SAKMainWindow::addRemovablePage);
        tabMenu->addAction(a);
    }

    QMenu *windowMenu = new QMenu(tr("新建窗口"), this);
    fileMenu->addMenu(windowMenu);
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        connect(a, &QAction::triggered, this, &SAKMainWindow::openIODeviceWindow);
        a->setData(var);
        windowMenu->addAction(a);
    }

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("退出"));
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void SAKMainWindow::initToolMenu()
{
    QMenu *toolMenu = new QMenu(tr("工具"));
    menuBar()->addMenu(toolMenu);
    toolsMenu = toolMenu;
}

void SAKMainWindow::initOptionMenu()
{
    QMenu *optionMenu = new QMenu(tr("选项"));
    menuBar()->addMenu(optionMenu);

    /*
     * 软件样式，设置默认样式需要重启软件
     */
    QMenu *stylesheetMenu = new QMenu("皮肤");
    optionMenu->addMenu(stylesheetMenu);
    defaultStyleSheetAction = new QAction(tr("Qt默认样式"), this);
    defaultStyleSheetAction->setCheckable(true);
    stylesheetMenu->addAction(defaultStyleSheetAction);
    connect(defaultStyleSheetAction, &QAction::triggered, [=](){
        for(auto var:QtStyleSheetApi::instance()->actions()){
            var->setChecked(false);
        }

        changeStylesheet(QString());
        defaultStyleSheetAction->setChecked(true);
        QMessageBox::information(this, tr("重启软件生效"), tr("软件样式已更改，重启软件后即可使用默认样式"));
    });

    stylesheetMenu->addSeparator();
    stylesheetMenu->addActions(QtStyleSheetApi::instance()->actions());
    QString styleSheetName = SAKSettings::instance()->value(appStylesheetKey).toString();
    if (!styleSheetName.isEmpty()){
        QtStyleSheetApi::instance()->setStyleSheet(styleSheetName);
    }else{
        defaultStyleSheetAction->setChecked(true);
    }

    /*
     * 软件风格，默认使用Qt支持的第一种软件风格
     */
    QMenu *appStyleMenu = new QMenu(tr("软件风格"));
    optionMenu->addMenu(appStyleMenu);
    appStyleMenu->addActions(QtAppStyleApi::instance()->actions());
    QString style = SAKSettings::instance()->value(appStyleKey).toString();
    QtAppStyleApi::instance()->setStyle(style);
}

void SAKMainWindow::initHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("帮助"), this);
    menuBar()->addMenu(helpMenu);

    QAction *aboutQtAction = new QAction(tr("关于Qt"), this);
    helpMenu->addAction(aboutQtAction);
    connect(aboutQtAction, &QAction::triggered, [=](){QMessageBox::aboutQt(this, tr("关于Qt"));});

    QAction *aboutAction = new QAction(tr("关于软件"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &SAKMainWindow::About);

    QMenu *srcMenu = new QMenu(tr("获取源码"), this);
    helpMenu->addMenu(srcMenu);
    QAction *visitGitHubAction = new QAction(QIcon(":/Resources/Images/GitHub.png"), tr("GitHub"), this);
    connect(visitGitHubAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/wuuhii/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGitHubAction);
    QAction *visitGiteeAction = new QAction(QIcon(":/Resources/Images/Gitee.png"), tr("Gitee"), this);
    connect(visitGiteeAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://gitee.com/wuuhii/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGiteeAction);

    QAction *updateAction = new QAction(tr("检查更新"), this);
    helpMenu->addAction(updateAction);
    connect(updateAction, &QAction::triggered, updateManager, &UpdateManager::checkForUpdate);

    QAction *moreInformationAction = new QAction(tr("更多信息"), this);
    helpMenu->addAction(moreInformationAction);
    connect(moreInformationAction, &QAction::triggered, moreInformation, &MoreInformation::show);
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
    case SAKGlobal::SAKEnumDebugPageTypeUDP:
        widget = new SAKUdpDebugPage;
        break;
    case SAKGlobal::SAKEnumDebugPageTypeTCPClient:
        widget = new SAKTcpClientDebugPage;
        break;
    case SAKGlobal::SAKEnumDebugPageTypeTCPServer:
        widget = new SAKTcpServerDebugPage;
        break;

#ifdef SAK_IMPORT_COM_MODULE
    case SAKGlobal::SAKEnumDebugPageTypeCOM:
        widget = new SAKSerialPortDebugPage;
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
    QWidget *w = mpTabWidget->widget(index);
    mpTabWidget->removeTab(index);
    w->close();
}

void SAKMainWindow::createCRCCalculator()
{
    CRCCalculator *cal = new CRCCalculator;
    cal->show();
}
