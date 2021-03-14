/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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
#include <QDebug>
#include <QLocale>
#include <QTabBar>
#include <QAction>
#include <QLocale>
#include <QVariant>
#include <QSysInfo>
#include <QMetaEnum>
#include <QSettings>
#include <QLineEdit>
#include <QStatusBar>
#include <QClipboard>
#include <QJsonArray>
#include <QScrollBar>
#include <QSizePolicy>
#include <QScrollArea>
#include <QJsonObject>
#include <QSpacerItem>
#include <QMessageBox>
#include <QActionGroup>
#include <QStyleFactory>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDesktopServices>

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKUpdateManager.hh"
#include "SAKTestDebugPage.hh"
#include "SAKCommonDataStructure.hh"

// Debugging tools
#ifdef SAK_IMPORT_MODULE_FILECHECKER
#include "SAKToolFileChecker.hh"
#endif
#include "SAKToolCRCCalculator.hh"
#ifdef SAK_IMPORT_MODULE_QRCODE
#include "SAKToolQRCodeCreator.hh"
#endif
#include "SAKToolFloatAssistant.hh"
#include "SAKToolStringAssistant.hh"

// Debugging pages
#include "SAKTestDebugPage.hh"
#ifdef SAK_IMPORT_MODULE_SERIALBUS
#include "SAKModbusDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_UDP
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_TCP
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include "SAKSerialPortDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
#include "SAKBluetoothClientDebugPage.hh"
#include "SAKBluetoothServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketServerDebugPage.hh"
#endif
#ifdef SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY
#include "SAKBluetoothLowEnergyDebugPage.hh"
#endif

#include "ui_SAKMainWindow.h"

SAKMainWindow::SAKMainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,mWindowsMenu(Q_NULLPTR)
    ,mUpdateManager(Q_NULLPTR)
    ,mSettingKeyEnableTestPage(QString("enableTestPage"))
    ,mUi(new Ui::SAKMainWindow)
    ,mTabWidget(new QTabWidget)
{
    mSettingsKeyContext.enableTestPage = QString("%1/enableTestPage").arg(qApp->applicationName());
    mSettingsKeyContext.currentTabPage = QString("%1/currentTabPage").arg(qApp->applicationName());

    mUi->setupUi(this);
    mUpdateManager = new SAKUpdateManager(this);

    initToosMetaObjectInfoList();
    initializingMetaObject();

#ifdef Q_OS_ANDROID
    setWindowFlags(Qt::FramelessWindowHint);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    scrollArea->setWidget(mTabWidget);
#else
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(mTabWidget);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
    centralWidget->layout()->setContentsMargins(6, 6, 6, 6);
    QString title = QString(tr("Qt Swiss Army Knife"));
    title.append(QString(" "));
    title.append(QString("v") + qobject_cast<SAKApplication*>(qApp)->applicationVersion());
    setWindowTitle(title);
#endif

    // Initializing menu bar
    initMenuBar();

    // Connecting the signal of tab page to it's slot.
    mTabWidget->setTabsClosable(true);
    connect(mTabWidget, &QTabWidget::tabCloseRequested, this, &SAKMainWindow::removeRemovableDebugPage);
    connect(mTabWidget, &QTabWidget::currentChanged, this, [=](int index){
        sakApp->settings()->setValue(mSettingsKeyContext.currentTabPage, index);
    });

    // Create debugging pages, the operation will emit the signal named currentChanged.
    // So you should block it, or the value of setting opetion(mSettingsKeyContext.currentTabPage) will be aways 0.
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    mTabWidget->blockSignals(true);
    for (int i = 0; i < metaEnum.keyCount(); i++){
#ifdef QT_DEBUG
        // Test page is selectable, it is for developer of the project.
        bool enableTestPage = sakApp->settings()->value(mSettingsKeyContext.enableTestPage).toBool();
        if (!enableTestPage && (metaEnum.value(i) == DebugPageTypeTest)){
            continue;
        }
#endif

        // The page can not be closed.
        QWidget *page = debugPageFromDebugPageType(metaEnum.value(i));
        if (page){
            mTabWidget->addTab(page, page->windowTitle());
            appendWindowAction(page);
        }
    }
    mTabWidget->blockSignals(false);
    if (mWindowsMenu){
        mWindowsMenu->addSeparator();
    }

    // Set the current page to last time
    int currentPage = sakApp->settings()->value(mSettingsKeyContext.currentTabPage).toInt();
    mTabWidget->setCurrentIndex(currentPage);

    // Hide the close button, the step must be done after calling setTabsClosable() function.
    for (int i = 0; i < mTabWidget->count(); i++){
        mTabWidget->tabBar()->setTabButton(i, QTabBar::RightSide, Q_NULLPTR);
        mTabWidget->tabBar()->setTabButton(i, QTabBar::LeftSide, Q_NULLPTR);
    }
}

SAKMainWindow::~SAKMainWindow()
{
    delete mUi;
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
    initDemoMenu();
    initHelpMenu();
}

void SAKMainWindow::initFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    QMenu *tabMenu = new QMenu(tr("New Page"), this);
    fileMenu->addMenu(tabMenu);
    QMetaEnum enums = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(debugPageTitleFromDebugPageType(enums.value(i)), this);
        // The object name is the default title of debug page
        a->setObjectName(debugPageTitleFromDebugPageType(enums.value(i)));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        a->setData(var);
        connect(a, &QAction::triggered, this, &SAKMainWindow::appendRemovablePage);
        tabMenu->addAction(a);
    }

    QMenu *windowMenu = new QMenu(tr("New Window"), this);
    fileMenu->addMenu(windowMenu);
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(debugPageTitleFromDebugPageType(enums.value(i)), this);
        // The object name is the default title of debug page
        a->setObjectName(debugPageTitleFromDebugPageType(enums.value(i)));
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

    for (auto &var : mToolMetaObjectInfoList){
        QWidget *w = qobject_cast<QWidget*>(var.metaObject.newInstance());
        Q_ASSERT_X(w, __FUNCTION__, "A null pointer!");
        w->hide();
        QAction *action = new QAction(var.title, this);
        action->setData(QVariant::fromValue(w));
        toolMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=](){
            QWidget *w = action->data().value<QWidget*>();
            if (w->isHidden()){
                w->show();
            }else{
                w->activateWindow();
            }
        });
    }
}

void SAKMainWindow::initOptionMenu()
{
    QMenu *optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(optionMenu);

    // Initializing application style menu.
    QMenu *appStyleMenu = new QMenu(tr("Application Style"), this);
    optionMenu->addMenu(appStyleMenu);
    auto styleKeys = QStyleFactory::keys();
    QList<QAction*> actionsList;
    QActionGroup *actionGroup = new QActionGroup(this);
    for (auto &var : styleKeys){
        QAction *action = new QAction(var, this);
        action->setObjectName(var);
        action->setCheckable(true);
        actionsList.append(action);
        actionGroup->addAction(action);
        connect(action, &QAction::triggered, this, [=](){
            QString style = qobject_cast<QAction*>(sender())->objectName();
            sakApp->setStyle(style);
            sakApp->settings()->setValue(sakApp->settingsKeyContext()->appStyle, style);
        });
    }
    // Readin the specified style.
    QString style = sakApp->settings()->value(sakApp->settingsKeyContext()->appStyle).toString();
    if (style.length()){
        for (auto &var : actionsList){
            if (var->objectName().compare(style) == 0){
                var->blockSignals(true);
                var->setChecked(true);
                var->blockSignals(false);
                sakApp->setStyle(style);
                sakApp->settings()->setValue(sakApp->settingsKeyContext()->appStyle, style);
                break;
            }
        }
    }
    appStyleMenu->addActions(actionsList);

#ifdef QT_DEBUG
    optionMenu->addSeparator();
    mTestPageAction = new QAction(tr("Enable Testing Page"), this);
    optionMenu->addAction(mTestPageAction);
    mTestPageAction->setCheckable(true);
    connect(mTestPageAction, &QAction::triggered, this, &SAKMainWindow::testPageActionTriggered);
    bool enableTestPage = sakApp->settings()->value(mSettingsKeyContext.enableTestPage).toBool();
    if (enableTestPage){
        mTestPageAction->setChecked(true);
    }else{
        mTestPageAction->setChecked(false);
    }
#endif
    QAction *action = new QAction(tr("Clear Configuration"), this);
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

    QString language = sakApp->settings()->value(sakApp->settingsKeyContext()->language).toString();

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
            for(auto &var:infoList){
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

    QAction *aboutAction = new QAction(tr("About Application"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &SAKMainWindow::aboutQsak);

    QMenu *srcMenu = new QMenu(tr("Get Source"), this);
    helpMenu->addMenu(srcMenu);
    QAction *visitGitHubAction = new QAction(QIcon(":/resources/images/GitHub.png"), tr("GitHub"), this);
    connect(visitGitHubAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String(SAK_GITHUB_REPOSITORY_URL)));});
    srcMenu->addAction(visitGitHubAction);
    QAction *visitGiteeAction = new QAction(QIcon(":/resources/images/Gitee.png"), tr("Gitee"), this);
    connect(visitGiteeAction, &QAction::triggered, [](){QDesktopServices::openUrl(QUrl(QLatin1String(SAK_GITEE_REPOSITORY_URL)));});
    srcMenu->addAction(visitGiteeAction);

    QAction *updateAction = new QAction(tr("Check for Update"), this);
    helpMenu->addAction(updateAction);
    connect(updateAction, &QAction::triggered, mUpdateManager, &SAKUpdateManager::show);

    QAction *releaseHistoryAction = new QAction(tr("Release History"), this);
    helpMenu->addAction(releaseHistoryAction);
    connect(releaseHistoryAction, &QAction::triggered, this, &SAKMainWindow::showReleaseHistoryActionDialog);

    helpMenu->addSeparator();
    QAction *qrCodeAction = new QAction(tr("QR Code"), this);
    helpMenu->addAction(qrCodeAction);
    connect(qrCodeAction, &QAction::triggered, this, &SAKMainWindow::showQrCodeDialog);
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
    linkList << Link{tr("Qt Official Download"), QString("http://download.qt.io/official_releases/qt"), QString(":/resources/images/Qt.png")}
             << Link{tr("Qt Official Blog"), QString("https://www.qt.io/blog"), QString(":/resources/images/Qt.png")}
             << Link{tr("Qt Official Release"), QString("https://wiki.qt.io/Qt_5.15_Release"), QString(":/resources/images/Qt.png")}
             << Link{tr("Download SAK from Github"), QString("%1/releases").arg(SAK_GITHUB_REPOSITORY_URL), QString(":/resources/images/GitHub.png")}
             << Link{tr("Download SAK from Gitee"), QString("%1/releases").arg(SAK_GITEE_REPOSITORY_URL), QString(":/resources/images/Gitee.png")}
             << Link{tr("Office Web Site"), QString("https://qsaker.gitee.io/qsak/"), QString(":/resources/images/Gitee.png")};

    for (auto &var:linkList){
        QAction *action = new QAction(QIcon(var.iconPath), var.name, this);
        action->setObjectName(var.url);
        linksMenu->addAction(action);

        connect(action, &QAction::triggered, this, [=](){
            QDesktopServices::openUrl(QUrl(sender()->objectName()));
        });
    }
}

void SAKMainWindow::initDemoMenu()
{
    QMenu *demoMenu = new QMenu(tr("&Demo"), this);
    menuBar()->addMenu(demoMenu);

    struct Link {
        QString name;
        QString url;
        QString iconPath;
    };
    QList<Link> linkList;
    linkList << Link{tr("Qt SerialPort Demo"), QString("https://gitee.com/qsaker/qt-demo-serial-port-widget.git"), QString(":/resources/images/Qt.png")};

    for (auto &var:linkList){
        QAction *action = new QAction(QIcon(var.iconPath), var.name, this);
        action->setObjectName(var.url);
        demoMenu->addAction(action);

        connect(action, &QAction::triggered, this, [=](){
            QDesktopServices::openUrl(QUrl(sender()->objectName()));
        });
    }
}

void SAKMainWindow::aboutQsak()
{
    struct Info {
        QString name;
        QString value;
        bool valueIsUrl;
    };

    auto dateTimeString = sakApp->buildDate()->toString(QLocale::system().dateFormat());
    dateTimeString = dateTimeString.append(" ");
    dateTimeString = dateTimeString.append(sakApp->buildTime()->toString("hh:mm:ss"));
    QList<Info> infoList;
    infoList << Info{tr("Version"), QString(qApp->applicationVersion()), false}
             << Info{tr("Author"), QString(SAK_AUTHOR), false}
             << Info{tr("Email"), QString(SAK_AUTHOR_EMAIL), false}
             << Info{tr("QQ"), QString("QQ:2869470394"), false}
             << Info{tr("QQ Group"), QString("QQ:952218522"), false}
             << Info{tr("Build Time"), dateTimeString, false}
             << Info{tr("Copyright"), tr("Copyright 2018-%1 Qter. All rights reserved.").arg(sakApp->buildDate()->toString("yyyy")), false}
             << Info{tr("Gitee Url"), QString("<a href=%1>%1</a>").arg(SAK_GITEE_REPOSITORY_URL), true}
             << Info{tr("Gitbub Url"), QString("<a href=%1>%1</a>").arg(SAK_GITHUB_REPOSITORY_URL), true};

    QDialog dialog;
    dialog.setWindowTitle(tr("About QSAK"));

    QGridLayout *gridLayout = new QGridLayout(&dialog);
    int i = 0;
    for (auto &var : infoList){
        QLabel *nameLabel = new QLabel(QString("<font color=green>%1</font>").arg(var.name), &dialog);
        QLabel *valueLabel = new QLabel(var.value, &dialog);
        gridLayout->addWidget(nameLabel, i, 0, 1, 1);
        gridLayout->addWidget(valueLabel, i, 1, 1, 1);
        i += 1;

        if (var.valueIsUrl){
            connect(valueLabel, &QLabel::linkActivated, [](QString url){QDesktopServices::openUrl(QUrl(url));});
        }
    }
    dialog.setLayout(gridLayout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void SAKMainWindow::removeRemovableDebugPage(int index)
{
    QWidget *w = mTabWidget->widget(index);
    mTabWidget->removeTab(index);
    w->close();
}

void SAKMainWindow::appendWindowAction(QWidget *page)
{
    if (mWindowsMenu){
        QAction *action = new QAction(page->windowTitle(), mWindowsMenu);
        action->setData(QVariant::fromValue(page));
        mWindowsMenu->addAction(action);
        connect(action, &QAction::triggered, this, &SAKMainWindow::activePage);
        connect(page, &QWidget::destroyed, action, &QAction::deleteLater);
    }
}

void SAKMainWindow::testPageActionTriggered()
{
    // ??
    bool checked = mTestPageAction->isChecked();
    mTestPageAction->setChecked(checked);
    sakApp->settings()->setValue(mSettingsKeyContext.enableTestPage, QVariant::fromValue(checked));
    rebootRequestion();
};

void SAKMainWindow::clearConfiguration()
{
    sakApp->settings()->setValue(sakApp->settingsKeyContext()->appStyle, QString(""));
    sakApp->settings()->setValue(sakApp->settingsKeyContext()->removeSettingsFile, QVariant::fromValue(true));
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

void SAKMainWindow::initializingMetaObject()
{
#ifdef QT_DEBUG
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTest, SAKTestDebugPage::staticMetaObject, tr("Test")});
#endif
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeCOM, SAKSerialPortDebugPage::staticMetaObject, tr("COM")});
#endif
#ifdef SAK_IMPORT_HID_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeHID, SAKHIDDebugPage::staticMetaObject, tr("HID")});
#endif
#ifdef SAK_IMPORT_USB_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUSB, SAKUSBDebugPage::staticMetaObject, tr("USB")});
#endif
#ifdef SAK_IMPORT_MODULE_UDP
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUdpClient, SAKUdpClientDebugPage::staticMetaObject, tr("UDP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUdpServer, SAKUdpServerDebugPage::staticMetaObject, tr("UDP-S")});
#endif
#ifdef SAK_IMPORT_MODULE_TCP
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTCPClient, SAKTcpClientDebugPage::staticMetaObject, tr("TCP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTCPServer, SAKTcpServerDebugPage::staticMetaObject, tr("TCP-S")});
#endif
#ifdef SAK_IMPORT_MODULE_SSLSOCKET
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{SAKSslSocketClientDebugPage::staticMetaObject, tr("SSL-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{SAKSslSocketServerDebugPage::staticMetaObject, tr("SSL-S")});
#endif
#ifdef SAK_IMPORT_SCTP_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{SAKSCTPClientDebugPage::staticMetaObject, tr("SCTP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{SAKSCTPServerDebugPage::staticMetaObject, tr("SCTP-S")});
#endif
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeBluetoothClient, SAKBluetoothClientDebugPage::staticMetaObject, tr("BT-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeBluetoothServer, SAKBluetoothServerDebugPage::staticMetaObject, tr("BT-S")});
#endif
#ifdef SAK_IMPORT_MODULE_BLUETOOTHLOWENERGY
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeBluetoothLowEnergy, SAKBluetoothLowEnergyDebugPage::staticMetaObject, tr("BLE")});
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeWebSocketClient, SAKWebSocketClientDebugPage::staticMetaObject, tr("WS-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeWebSocketServer, SAKWebSocketServerDebugPage::staticMetaObject, tr("WS-S")});
#endif
#ifdef SAK_IMPORT_MODULE_SERIALBUS
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeModbus, SAKModbusDebugPage::staticMetaObject, tr("Modbus")});
#endif
}

void SAKMainWindow::initToosMetaObjectInfoList()
{
#ifdef SAK_IMPORT_MODULE_FILECHECKER
    mToolMetaObjectInfoList.append(SAKToolMetaObjectInfo{SAKToolFileChecker::staticMetaObject, tr("File Assistant")});
#endif
    mToolMetaObjectInfoList.append(SAKToolMetaObjectInfo{SAKToolCRCCalculator::staticMetaObject, tr("CRC Assistant")});
    mToolMetaObjectInfoList.append(SAKToolMetaObjectInfo{SAKToolFloatAssistant::staticMetaObject, tr("Float Assistant")});
    mToolMetaObjectInfoList.append(SAKToolMetaObjectInfo{SAKToolStringAssistant::staticMetaObject, tr("String Assistant")});
}

void SAKMainWindow::showReleaseHistoryActionDialog()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("Release History"));
    dialog.resize(600, 400);

    QTextBrowser *textBrowser = new QTextBrowser(&dialog);
    QFile file(":/resources/files/History.txt");
    if (file.open(QFile::ReadOnly)){
        QByteArray data = file.readAll();
        textBrowser->setText(QString::fromUtf8(data));
    }

    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(textBrowser);
    dialog.setLayout(layout);
    dialog.show();
    dialog.exec();
}

QString SAKMainWindow::tabPageName(int type)
{
    QString name;
    QString defaultName = debugPageTitleFromDebugPageType(type);
    QDialog dialog;
    dialog.setWindowTitle(tr("Edit Page Name"));

    QLabel *tipLabel = new QLabel(tr("Please input the name of tab page"), &dialog);
    QLabel *tipPageNameLabel = new QLabel(tr("Tab page name"), &dialog);
    QLineEdit *tapPageNameLineEdit = new QLineEdit(&dialog);
    QLabel *emptyLabel = new QLabel("", &dialog);
    QPushButton *okPushButton = new QPushButton(tr("OK"), &dialog);
    QPushButton *cancelPushButton = new QPushButton(tr("Cancel"), &dialog);
    connect(okPushButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelPushButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    tapPageNameLineEdit->setText(defaultName);
    emptyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QGridLayout *gridLayout = new QGridLayout(&dialog);
    gridLayout->addWidget(tipLabel, 0, 0, 1, 4);
    gridLayout->addWidget(tipPageNameLabel, 1, 0, 1, 1);
    gridLayout->addWidget(tapPageNameLineEdit, 1, 1, 1, 3);
    gridLayout->addWidget(emptyLabel, 2, 1, 1, 1);
    gridLayout->addWidget(okPushButton, 2, 2, 1, 1);
    gridLayout->addWidget(cancelPushButton, 2, 3, 1, 1);
    dialog.setLayout(gridLayout);
    dialog.show();
    if (QDialog::Accepted == dialog.exec()){
        name = tapPageNameLineEdit->text();
    }

    return name;
}

QWidget *SAKMainWindow::debugPage(QObject *sender)
{
    if (!sender){
        return Q_NULLPTR;
    }

    if (sender->inherits("QAction")){
        int type = qobject_cast<QAction*>(sender)->data().value<int>();
        QString title = tabPageName(type);
        if (title.length()){
            QWidget *widget = debugPageFromDebugPageType(type);
            if (widget){
                widget->setAttribute(Qt::WA_DeleteOnClose, true);
                widget->setWindowTitle(title);
                return widget;
            }
        }
    }

    return Q_NULLPTR;
}

void SAKMainWindow::showQrCodeDialog()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("QR Code"));

    struct QrCodeInfo {
        QString title;
        QString qrCode;
    };
    QList<QrCodeInfo> qrCodeInfoList;

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/resources/images/QSAKQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/resources/images/QtQQ.jpg")};

    QTabWidget *tabWidget = new QTabWidget(&dialog);
    for (auto &var : qrCodeInfoList){
        QLabel *label = new QLabel(tabWidget);
        label->setPixmap(QPixmap::fromImage(QImage(var.qrCode)));
        tabWidget->addTab(label, var.title);
    }

    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(tabWidget);
    dialog.setLayout(layout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
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
            sakApp->settings()->setValue(sakApp->settingsKeyContext()->language, QVariant::fromValue(QString(language + "-" + name)));
            qobject_cast<SAKApplication*>(qApp)->installLanguage();
            rebootRequestion();
        }
    }
}

void SAKMainWindow::openDebugPageWidget()
{
    // The function must be called by signal of QAction
    QWidget *widget = debugPage(sender());
    if (widget){
        widget->show();
        appendWindowAction(widget);
    }
}

void SAKMainWindow::appendRemovablePage()
{
    // The function must be called by signal of QAction
    QWidget *widget = debugPage(sender());
    if (widget){
        mTabWidget->addTab(widget, widget->windowTitle());
        appendWindowAction(widget);
    }
}

QWidget *SAKMainWindow::debugPageFromDebugPageType(int type)
{
    QWidget *widget = Q_NULLPTR;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (auto &var : mDebugPageMetaInfoList){
        if (var.debugPageType == type){
            for (int i = 0; i < metaEnum.keyCount(); i++){
                if (var.debugPageType == metaEnum.value(i)){
                    widget = qobject_cast<QWidget*>(var.metaObject.newInstance(Q_ARG(int, metaEnum.value(i)), Q_ARG(QString, QString(metaEnum.key(i)))));
                    widget->setWindowTitle(debugPageTitleFromDebugPageType(type));
                    break;
                }
            }
        }
    }

    Q_ASSERT_X(widget, __FUNCTION__, "Unknown debug page type!");
    return widget;
}

QString SAKMainWindow::debugPageTitleFromDebugPageType(int type)
{
    QString title;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (auto &var : mDebugPageMetaInfoList){
        if (var.debugPageType == type){
            for (int i = 0; i < metaEnum.keyCount(); i++){
                if (var.debugPageType == metaEnum.value(i)){
                    title = var.defaultTitle;
                    break;
                }
            }
        }
    }

    if (title.isEmpty()){
        title = QString("UnknownDebugPage");
        qWarning() << "Unknown debug page type:" << type;
        Q_ASSERT_X(false, __FUNCTION__, "Unknown debug page type!");
    }

    return title;
}
