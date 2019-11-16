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
 */
#include <QFile>

#include <QLocale>
#include <QTabBar>
#include <QAction>
#include <QVariant>
#include <QSysInfo>
#include <QMetaEnum>
#include <QSettings>
#include <QJsonArray>
#include <QJsonObject>
#include <QSpacerItem>
#include <QMessageBox>
#include <QStyleFactory>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDesktopServices>

#include "SAKGlobal.hh"
#include "SAKVersion.hh"
#include "SAKSettings.hh"
#include "SAKCRCCalculator.hh"
#include "SAKMainWindow.hh"
#include "QtAppStyleApi.hh"
#include "SAKApplication.hh"
#include "SAKUdpDebugPage.hh"
#include "SAKMoreInformation.hh"
#include "QtStyleSheetApi.hh"
#include "SAKUpdateManager.hh"
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

SAKMainWindow::SAKMainWindow(QWidget *parent)
    :QMainWindow (parent)
    ,mpTabWidget (new QTabWidget)
    ,ui (new Ui::SAKMainWindow)
    ,moreInformation (new SAKMoreInformation)
{
    ui->setupUi(this);
    updateManager = new SAKUpdateManager(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);
    setWindowTitle(tr("瑞士军刀--开发调试工具集") + " v" + reinterpret_cast<SAKApplication*>(qApp)->applicationVersion());

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
    this->mpTabWidget->addTab( new SAKSerialPortDebugPage,  tr("串口调试"));
#endif
    this->mpTabWidget->addTab(new SAKUdpDebugPage,          tr("UDP调试"));
    this->mpTabWidget->addTab(new SAKTcpClientDebugPage,    tr("TCP客户端"));
    this->mpTabWidget->addTab(new SAKTcpServerDebugPage,    tr("TCP服务器"));

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
    addTool(tr("文件校验工具"),     new QtCryptographicHashController);

    QAction *action = nullptr;
    action = new QAction(tr("CRC计算器"), this);
    toolsMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKMainWindow::createCRCCalculator);
}

void SAKMainWindow::About()
{
    SAKVersion verDialog;
    verDialog.show();
    verDialog.activateWindow();
    verDialog.exec();
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
    initLanguageMenu();
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
    QMenu *stylesheetMenu = new QMenu(tr("皮肤"));
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

void SAKMainWindow::initLanguageMenu()
{
    QMenu *languageMenu = new QMenu(tr("语言"), this);
    menuBar()->addMenu(languageMenu);

    QString language = SAKSettings::instance()->value(reinterpret_cast<SAKApplication*>(qApp)->settingStringLanguage()).toString();

    QFile file(":/translations/sak/Translations.json");
    file.open(QFile::ReadOnly);
    QByteArray jsonData = file.readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error == QJsonParseError::NoError){
        if (jsonDocument.isArray()){
            QJsonArray jsonArray = jsonDocument.array();
            struct info {
                QString locale;
                QString language;
                QString name;
            };
            QList<info> infoList;

            for (int i = 0; i < jsonArray.count(); i++){
                QJsonObject jsonObject = jsonArray.at(i).toObject();
                struct info languageInfo;
                languageInfo.locale = jsonObject.value("locale").toString();
                languageInfo.language = jsonObject.value("language").toString();
                languageInfo.name = jsonObject.value("name").toString();
                infoList.append(languageInfo);
            }

            QActionGroup *actionGroup = new QActionGroup(this);
            for(auto var:infoList){
                QAction *action = new QAction(var.name, languageMenu);
                languageMenu->addAction(action);
                action->setCheckable(true);
                actionGroup->addAction(action);
                action->setObjectName(var.language);
                action->setData(QVariant::fromValue<QString>(var.name));
                action->setIcon(QIcon(QString(":/translations/sak/%1").arg(var.locale).toLatin1()));
                connect(action, &QAction::triggered, this, &SAKMainWindow::installLanguage);

                if (var.language == language.split('-').first()){
                    action->setChecked(true);
                }
            }
        }
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Json file parsing failed!");
    }    
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
    QAction *visitGitHubAction = new QAction(QIcon(":/resources/images/GitHub.png"), tr("GitHub"), this);
    connect(visitGitHubAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/wuuhii/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGitHubAction);
    QAction *visitGiteeAction = new QAction(QIcon(":/resources/images/Gitee.png"), tr("Gitee"), this);
    connect(visitGiteeAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://gitee.com/wuuhii/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGiteeAction);

    QAction *updateAction = new QAction(tr("检查更新"), this);
    helpMenu->addAction(updateAction);
    connect(updateAction, &QAction::triggered, updateManager, &SAKUpdateManager::show);

    QAction *moreInformationAction = new QAction(tr("更多信息"), this);
    helpMenu->addAction(moreInformationAction);
    connect(moreInformationAction, &QAction::triggered, moreInformation, &SAKMoreInformation::show);
}

void SAKMainWindow::installLanguage()
{
    if (!sender()){
        return;
    }

    QAction *action = reinterpret_cast<QAction*>(sender());
    action->setChecked(true);

    QString language = action->objectName();
    QString name = action->data().toString();
    SAKSettings::instance()->setValue(reinterpret_cast<SAKApplication*>(qApp)->settingStringLanguage(), language+"-"+name);
    reinterpret_cast<SAKApplication*>(qApp)->installLanguage();
    QMessageBox::information(this, tr("重启生效"),
                             tr("软件语言包已更改，重启软件生效！"));
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

#ifdef SAK_IMPORT_HID_MODULE
    case SAKGlobal::SAKEnumDebugPageTypeHID:
        widget = new SAKSerialPortDebugPage;
        break;
#endif

#ifdef SAK_IMPORT_USB_MODULE
    case SAKGlobal::SAKEnumDebugPageTypeUSB:
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
    SAKCRCCalculator *cal = new SAKCRCCalculator;
    cal->show();
}
