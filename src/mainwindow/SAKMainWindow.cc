/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDir>
#include <QFile>
#include <QRect>
#include <QLocale>
#include <QTabBar>
#include <QAction>
#include <QVariant>
#include <QSysInfo>
#include <QMetaEnum>
#include <QSettings>
#include <QStatusBar>
#include <QClipboard>
#include <QJsonArray>
#include <QScrollBar>
#include <QScrollArea>
#include <QJsonObject>
#include <QSpacerItem>
#include <QMessageBox>
#include <QStyleFactory>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDesktopServices>

#include "SAK.hh"
#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKSettings.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKMainWindow.hh"
#include "QtAppStyleApi.hh"
#include "SAKApplication.hh"
#include "QtStyleSheetApi.hh"
#include "SAKToolsManager.hh"
#include "SAKUpdateManager.hh"
#include "SAKTestDebugPage.hh"
#include "SAKToolCRCCalculator.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpServerDebugPage.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKMainWindowQrCodeView.hh"
#include "SAKSslSocketClientDebugPage.hh"
#include "SAKSslSocketServerDebugPage.hh"
#include "SAKMainWindowMoreInformationDialog.hh"
#include "SAKMainWindowTabPageNameEditDialog.hh"

#ifdef SAK_IMPORT_HID_MODULE
#include "SAKHidDebugPage.hh"
#endif
#ifdef SAK_IMPORT_USB_MODULE
#include "SAKUsbDebugPage.hh"
#endif
#ifdef SAK_IMPORT_QRCODE_MODULE
#include "SAKToolQRCodeCreator.hh"
#endif
#ifdef SAK_IMPORT_SCTP_MODULE
#include "SAKSctpClientDebugPage.hh"
#include "SAKSctpServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_COM_MODULE
#include "SAKSerialPortDebugPage.hh"
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
#include "SAKBluetoothClientDebugPage.hh"
#include "SAKBluetoothServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_FILECHECKER_MODULE
#include "SAKToolFileChecker.hh"
#endif

#include "ui_SAKMainWindow.h"

SAKMainWindow::SAKMainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,mToolsMenu(Q_NULLPTR)
    ,mDefaultStyleSheetAction(Q_NULLPTR)
    ,mUpdateManager(Q_NULLPTR)
    ,mMoreInformation(new SAKMainWindowMoreInformationDialog)
    ,mQrCodeDialog(Q_NULLPTR)
    ,mSettingKeyEnableTestPage(QString("enableTestPage"))
    ,mSettingKeyClearConfiguration(settingKeyClearConfiguration())
    ,mUi(new Ui::SAKMainWindow)
    ,mTabWidget(new QTabWidget)
{
    mUi->setupUi(this);
    mUpdateManager = new SAKUpdateManager(this);
    mUpdateManager->setSettings(SAKSettings::instance());
    mQrCodeDialog = new SAKMainWindowQrCodeView(this);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(mTabWidget);
#if 0
    setWindowFlags(Qt::FramelessWindowHint);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    scrollArea->setLayout(layout);
    scrollArea->layout()->setContentsMargins(6, 6, 6, 6);
    scrollArea->setWidget(tabWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect rect = desktop->screenGeometry(tabWidget);
    tabWidget->setFixedWidth(rect.width());
#else
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
    centralWidget->layout()->setContentsMargins(6, 6, 6, 6);
#endif
    QString title = QString(tr("Qt Swiss Army Knife"));
    title.append(QString(" "));
    title.append(QString("v") + SAK::instance()->version());
    setWindowTitle(title);

    mTabWidget->setTabsClosable(true);
    connect(mTabWidget, &QTabWidget::tabCloseRequested, this, &SAKMainWindow::removeRemovableDebugPage);

    // Initializing menu bar
    initMenuBar();

    // The default application style of Windows paltform and Linux platform is "Fusion".
    // Do not set applicaiton stype for android palrform.
    // For macOS platform, do not use the application style named "Windows"
#ifndef Q_OS_ANDROID
    QString settingStyle = SAKSettings::instance()->appStyle();
    if (settingStyle.isEmpty()){
        QStringList styleKeys = QStyleFactory::keys();
#ifdef Q_OS_MACOS
        QString uglyStyle("Windows");
        for(auto var:styleKeys){
            if (var != uglyStyle){
                changeAppStyle(var);
                break;
            }
    }
#else
        QString defaultStyle("Fusion");
        if (styleKeys.contains(defaultStyle)){
            changeAppStyle(defaultStyle);
        }
#endif
    }
#endif

    // Create debugging page
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        // Test page is selectable
        bool enableTestPage = SAKSettings::instance()->value(mSettingKeyEnableTestPage).toBool();
        if (!enableTestPage && (metaEnum.value(i) == SAKCommonDataStructure::DebugPageTypeTest)){
            continue;
        }

        QString name = SAKGlobal::debugPageNameFromType(metaEnum.value(i));
        QWidget *page = debugPageFromType(metaEnum.value(i));
        page->setWindowTitle(name);
        if (page){
            mTabWidget->addTab(page, name);
            appendWindowAction(page);
        }
    }
    mWindowsMenu->addSeparator();

    // Hide the close button, the step must be do after calling setTabsClosable() function.
    for (int i = 0; i < mTabWidget->count(); i++){
        mTabWidget->tabBar()->setTabButton(i, QTabBar::RightSide, Q_NULLPTR);
        mTabWidget->tabBar()->setTabButton(i, QTabBar::LeftSide, Q_NULLPTR);
    }

    // Initializing the tools menu
    QMetaEnum toolTypeMetaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumToolType>();
    for (int i = 0; i < toolTypeMetaEnum.keyCount(); i++){
        QString name = SAKGlobal::toolNameFromType(toolTypeMetaEnum.value(i));
        QAction *action = new QAction(name, this);
        action->setData(QVariant::fromValue(toolTypeMetaEnum.value(i)));
        mToolsMenu->addAction(action);
        connect(action, &QAction::triggered, this, &SAKMainWindow::showToolWidget);
    }

    // Do soemthing to make the application look like more beautiful.
    connect(QtStyleSheetApi::instance(), &QtStyleSheetApi::styleSheetChanged, this, &SAKMainWindow::changeStylesheet);
    connect(QtAppStyleApi::instance(), &QtAppStyleApi::appStyleChanged, this, &SAKMainWindow::changeAppStyle);
}

SAKMainWindow::~SAKMainWindow()
{
    delete mUi;
}

const QString SAKMainWindow::settingKeyClearConfiguration()
{
    return QString("clearConfiguration");
}

void SAKMainWindow::initMenuBar()
{
#if 0
    // The menu bar is not show on ubuntu 16.04
   QMenuBar *menuBar = new QMenuBar(Q_NULLPTR);
#else
    QMenuBar *menuBar = this->menuBar();
#endif
    setMenuBar(menuBar);
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initWindowMenu();
    initLanguageMenu();
    initLinksMenu();
    initHelpMenu();
}

void SAKMainWindow::initFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    QMenu *tabMenu = new QMenu(tr("New page"), this);
    fileMenu->addMenu(tabMenu);
    QMetaEnum enums = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::debugPageNameFromType(i), this);
        a->setObjectName(SAKGlobal::debugPageNameFromType(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        a->setData(var);
        connect(a, &QAction::triggered, this, &SAKMainWindow::appendRemovablePage);
        tabMenu->addAction(a);
    }

    QMenu *windowMenu = new QMenu(tr("New window"), this);
    fileMenu->addMenu(windowMenu);
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::debugPageNameFromType(i), this);
        a->setObjectName(SAKGlobal::debugPageNameFromType(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        connect(a, &QAction::triggered, this, &SAKMainWindow::openDebugPageWidget);
        a->setData(var);
        windowMenu->addAction(a);
    }

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("Exit"), this);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void SAKMainWindow::initToolMenu()
{
    QMenu *toolMenu = new QMenu(tr("&Tools"));
    menuBar()->addMenu(toolMenu);
    mToolsMenu = toolMenu;
}

void SAKMainWindow::initOptionMenu()
{
    QMenu *optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(optionMenu);

    // Initializing style sheet menu, the application need to be reboot after change the style sheet to Qt default.
    QMenu *stylesheetMenu = new QMenu(tr("Skin"), this);
    optionMenu->addMenu(stylesheetMenu);
    mDefaultStyleSheetAction = new QAction(tr("Qt default"), this);
    mDefaultStyleSheetAction->setCheckable(true);
    stylesheetMenu->addAction(mDefaultStyleSheetAction);
    connect(mDefaultStyleSheetAction, &QAction::triggered, [=](){
        for(auto var:QtStyleSheetApi::instance()->actions()){
            var->setChecked(false);
        }

        changeStylesheet(QString());
        mDefaultStyleSheetAction->setChecked(true);
        rebootRequestion();
    });

    stylesheetMenu->addSeparator();
    stylesheetMenu->addActions(QtStyleSheetApi::instance()->actions());
    QString styleSheetName = SAKSettings::instance()->appStylesheet();
    if (!styleSheetName.isEmpty()){
        QtStyleSheetApi::instance()->setStyleSheet(styleSheetName);
    }else{
        mDefaultStyleSheetAction->setChecked(true);
    }

    // Initializing application style menu.
    QMenu *appStyleMenu = new QMenu(tr("Application style"), this);
    optionMenu->addMenu(appStyleMenu);
    appStyleMenu->addActions(QtAppStyleApi::instance()->actions());
    QString style = SAKSettings::instance()->appStyle();
    QtAppStyleApi::instance()->setStyle(style);

    optionMenu->addSeparator();

    mTestPageAction = new QAction(tr("Enable test page"), this);
    optionMenu->addAction(mTestPageAction);
    mTestPageAction->setCheckable(true);
    connect(mTestPageAction, &QAction::triggered, this, &SAKMainWindow::testPageActionTriggered);
    bool enableTestPage = SAKSettings::instance()->value(mSettingKeyEnableTestPage).toBool();
    if (enableTestPage){
        mTestPageAction->setChecked(true);
    }else{
        mTestPageAction->setChecked(false);
    }

    QAction *action = new QAction(tr("Clear configuration"), this);
    optionMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKMainWindow::clearConfiguration);
}

void SAKMainWindow::initWindowMenu()
{
    mWindowsMenu = new QMenu(tr("&Windows"), this);
    menuBar()->addMenu(mWindowsMenu);
}

void SAKMainWindow::initLanguageMenu()
{
    QMenu *languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(languageMenu);

    QString language = SAKSettings::instance()->language();

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
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    QAction *aboutQtAction = new QAction(tr("About Qt"), this);
    helpMenu->addAction(aboutQtAction);
    connect(aboutQtAction, &QAction::triggered, [=](){QMessageBox::aboutQt(this, tr("About Qt"));});

    QAction *aboutAction = new QAction(tr("About application"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &SAKMainWindow::about);

    QMenu *srcMenu = new QMenu(tr("Get source"), this);
    helpMenu->addMenu(srcMenu);
    QAction *visitGitHubAction = new QAction(QIcon(":/resources/images/GitHub.png"), tr("GitHub"), this);
    connect(visitGitHubAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/qsak/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGitHubAction);
    QAction *visitGiteeAction = new QAction(QIcon(":/resources/images/Gitee.png"), tr("Gitee"), this);
    connect(visitGiteeAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String("https://gitee.com/qsak/QtSwissArmyKnife")));});
    srcMenu->addAction(visitGiteeAction);

    QAction *updateAction = new QAction(tr("Check for update"), this);
    helpMenu->addAction(updateAction);
    connect(updateAction, &QAction::triggered, mUpdateManager, &SAKUpdateManager::show);

    QAction *moreInformationAction = new QAction(tr("More information"), this);
    helpMenu->addAction(moreInformationAction);
    connect(moreInformationAction, &QAction::triggered, mMoreInformation, &SAKMainWindowMoreInformationDialog::show);

    helpMenu->addSeparator();
    QAction *qrCodeAction = new QAction(tr("QR code"), this);
    helpMenu->addAction(qrCodeAction);
    connect(qrCodeAction, &QAction::triggered, mQrCodeDialog, &SAKMainWindowQrCodeView::show);
}

void SAKMainWindow::initLinksMenu()
{
    QMenu *linksMenu = new QMenu(tr("&Links"), this);
    menuBar()->addMenu(linksMenu);

    struct Link {
        QString name;
        QString url;
        QString iconPath;
    };
    QList<Link> linkList;
    linkList << Link{tr("Qt official download"), QString("http://download.qt.io/official_releases/qt"), QString(":/resources/images/Qt.png")}
             << Link{tr("Qt official blog"), QString("https://www.qt.io/blog"), QString(":/resources/images/Qt.png")}
             << Link{tr("Qt official release"), QString("https://wiki.qt.io/Qt_5.12_Release"), QString(":/resources/images/Qt.png")}
             << Link{tr("Download SAK from github"), QString("https://github.com/qsak/QtSwissArmyKnife/releases"), QString(":/resources/images/GitHub.png")}
             << Link{tr("Download SAK from gitee"), QString("https://gitee.com/qsak/QtSwissArmyKnife/releases"), QString(":/resources/images/Gitee.png")};

    for (auto var:linkList){
        QAction *action = new QAction(QIcon(var.iconPath), var.name, this);
        action->setObjectName(var.url);
        linksMenu->addAction(action);

        connect(action, &QAction::triggered, this, [=](){
            QDesktopServices::openUrl(QUrl(sender()->objectName()));
        });
    }
}

void SAKMainWindow::changeStylesheet(QString styleSheetName)
{
    SAKSettings::instance()->setAppStylesheet(styleSheetName);
    if (!styleSheetName.isEmpty()){
        mDefaultStyleSheetAction->setChecked(false);
    }
}

void SAKMainWindow::changeAppStyle(QString appStyle)
{
    SAKSettings::instance()->setAppStyle(appStyle);
}

void SAKMainWindow::about()
{
    QMessageBox::information(this, tr("About"), QString("<font color=green>%1</font><br />%2<br />"
                                                     "<font color=green>%3</font><br />%4<br />"
                                                     "<font color=green>%5</font><br />%6<br />"
                                                     "<font color=green>%7</font><br /><a href=%8>%8</a><br />"
                                                     "<font color=green>%9</font><br />%10<br />"
                                                     "<font color=green>%11</font><br />%12<br />"
                                                     "<font color=green>%13</font><br />%14<br />"
                                                     "<font color=green>%15</font><br />%16<br />"
                                                     "<font color=green>%17</font><br />%18<br />"
                                                     "<font color=red>%19</font><br />%20")
                             .arg(tr("Version")).arg(SAK::instance()->version())
                             .arg(tr("Author")).arg(SAK::instance()->authorName())
                             .arg(tr("Nickname")).arg(SAK::instance()->authorNickname())
                             .arg(tr("Release")).arg(SAK::instance()->officeUrl())
                             .arg(tr("Email")).arg(SAK::instance()->email())
                             .arg(tr("QQ")).arg(SAK::instance()->qqNumber())
                             .arg(tr("QQ group")).arg(SAK::instance()->qqGroupNumber())
                             .arg(tr("Build time")).arg(SAK::instance()->buildTime())
                             .arg(tr("Copyright")).arg(SAK::instance()->copyright())
                             .arg(tr("Cooperation")).arg(SAK::instance()->business()));
}

void SAKMainWindow::removeRemovableDebugPage(int index)
{
    QWidget *w = mTabWidget->widget(index);
    mTabWidget->removeTab(index);
    w->close();
}

QWidget *SAKMainWindow::debugPageFromType(int type)
{
    QWidget *widget = Q_NULLPTR;
    switch (type) {
    case SAKCommonDataStructure::DebugPageTypeTest:
        widget = new SAKTestDebugPage;
        break;
#ifdef SAK_IMPORT_COM_MODULE
    case SAKCommonDataStructure::DebugPageTypeCOM:
        widget = new SAKSerialPortDebugPage;
        break;
#endif
#ifdef SAK_IMPORT_HID_MODULE
    case SAKDataStruct::DebugPageTypeHID:
        widget = new SAKHidDebugPage;
        break;
#endif
#ifdef SAK_IMPORT_USB_MODULE
    case SAKDataStruct::DebugPageTypeUSB:
        widget = new SAKUsbDebugPage;
        break;
#endif
    case SAKCommonDataStructure::DebugPageTypeUdpClient:
        widget = new SAKUdpClientDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeUdpServer:
        widget = new SAKUdpServerDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeTCPClient:
        widget = new SAKTcpClientDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeTCPServer:
        widget = new SAKTcpServerDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeSslSocketClient:
        widget = new SAKSslSocketClientDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeSslSocketServer:
        widget = new SAKSslSocketServerDebugPage;
        break;
#ifdef SAK_IMPORT_SCTP_MODULE
    case SAKDataStruct::DebugPageTypeSCTPClient:
        widget = new SAKSctpClientDebugPage;
        break;
    case SAKDataStruct::DebugPageTypeSCTPServer:
        widget = new SAKSctpServerDebugPage;
        break;
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
    case SAKDataStruct::DebugPageTypeBluetoothClient:
        widget = new SAKBluetoothClientDebugPage;
        break;
    case SAKDataStruct::DebugPageTypeBluetoothServer:
        widget = new SAKBluetoothServerDebugPage;
        break;
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
    case SAKCommonDataStructure::DebugPageTypeWebSocketClient:
        widget = new SAKWebSocketClientDebugPage;
        break;
    case SAKCommonDataStructure::DebugPageTypeWebSocketServer:
        widget = new SAKWebSocketServerDebugPage;
        break;
#endif
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow window type.");
        break;
    }

    return widget;
}

void SAKMainWindow::appendWindowAction(QWidget *page)
{
    QAction *action = new QAction(page->windowTitle(), mWindowsMenu);
    action->setData(QVariant::fromValue(page));
    mWindowsMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKMainWindow::activePage);
    connect(page, &QWidget::destroyed, action, &QAction::deleteLater);
}

void SAKMainWindow::testPageActionTriggered()
{
    // ??
    bool checked = mTestPageAction->isChecked();
    mTestPageAction->setChecked(checked);
    SAKSettings::instance()->setValue(mSettingKeyEnableTestPage, QVariant::fromValue(checked));
    rebootRequestion();
};

void SAKMainWindow::clearConfiguration()
{
    SAKSettings::instance()->setValue(mSettingKeyClearConfiguration, QVariant::fromValue(true));
    rebootRequestion();
}

void SAKMainWindow::rebootRequestion()
{
    int ret = QMessageBox::information(this, tr("Reboot application to effective"), tr("Need to reboot, reboot to effective now?"), QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        qApp->closeAllWindows();
        qApp->exit(SAK_REBOOT_CODE);
    }
}

void SAKMainWindow::showToolWidget()
{
    if (sender()){
        if (sender()->inherits("QAction")){
            bool ok = false;
            QAction *action = qobject_cast<QAction *>(sender());
            int type = action->data().toInt(&ok);
            if (ok){
                SAKToolsManager::instance()->showToolWidget(type);
            }
        }
    }
}

void SAKMainWindow::activePage()
{
    if (sender()){
        if (sender()->inherits("QAction")){
            QAction *action = qobject_cast<QAction*>(sender());
            QWidget *widget = action->data().value<QWidget*>();
            if (widget->parent()){
                mTabWidget->setCurrentWidget(widget);
            }else{
                widget->activateWindow();
            }
        }
    }
}

void SAKMainWindow::installLanguage()
{
    if (sender()){
        if (sender()->inherits("QAction")){
            QAction *action = reinterpret_cast<QAction*>(sender());
            action->setChecked(true);

            QString language = action->objectName();
            QString name = action->data().toString();
            SAKSettings::instance()->setLanguage(language+"-"+name);
            rebootRequestion();
        }
    }
}

void SAKMainWindow::openDebugPageWidget()
{
    if (sender()){
        SAKMainWindowTabPageNameEditDialog dialog;
        dialog.setName(sender()->objectName());
        if (QDialog::Accepted == dialog.exec()){
            if (sender()->inherits("QAction")){
                int type = qobject_cast<QAction*>(sender())->data().value<int>();
                QWidget *widget = debugPageFromType(type);
                if (widget){
                    widget->setAttribute(Qt::WA_DeleteOnClose, true);
                    widget->setWindowTitle(dialog.name());
                    widget->show();
                    appendWindowAction(widget);
                }
            }
        }
    }
}

void SAKMainWindow::appendRemovablePage()
{
    if (sender()){
        SAKMainWindowTabPageNameEditDialog dialog;
        dialog.setName(sender()->objectName());
        if (QDialog::Accepted == dialog.exec()){
            if (sender()->inherits("QAction")){
                int type = qobject_cast<QAction*>(sender())->data().value<int>();
                QWidget *widget = debugPageFromType(type);
                if (widget){
                    widget->setAttribute(Qt::WA_DeleteOnClose, true);
                    widget->setWindowTitle(dialog.name());
                    mTabWidget->addTab(widget, dialog.name());
                    appendWindowAction(widget);
                }
            }
        }
    }
}
