/*****************************************************************************
 * Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDir>
#include <QFile>
#include <QRect>
#include <QDebug>
#include <QImage>
#include <QPixmap>
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
#include <QCloseEvent>
#include <QSizePolicy>
#include <QScrollArea>
#include <QJsonObject>
#include <QSpacerItem>
#include <QMessageBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QActionGroup>
#include <QTextBrowser>
#include <QStyleFactory>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDesktopServices>

#include "SAKSettings.hh"
#include "SAKToolBoxUi.hh"
#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKUpdateManager.hh"
#include "SAKAssistantsFactory.hh"

#include "ui_SAKMainWindow.h"

SAKMainWindow *sakMainWindow = Q_NULLPTR;
SAKMainWindow::SAKMainWindow(QSettings *settings,
                             QWidget *parent)
    :QMainWindow(parent)
    ,mWindowsMenu(Q_NULLPTR)
    ,mUpdateManager(Q_NULLPTR)
    ,mSettingKeyEnableTestPage(QString("enableTestPage"))
    ,mSettings(settings)
    ,ui(new Ui::SAKMainWindow)
    ,mToolBoxs(new QTabWidget)
{
    sakMainWindow = this;
    mSettingsKeyContext.enableTestPage = QString("%1/enableTestPage")
            .arg(qApp->applicationName());
    mSettingsKeyContext.currentTabPage = QString("%1/currentTabPage")
            .arg(qApp->applicationName());
    mSettingsKeyContext.exitToSystemTray = QString("%1/exitToSystenTray")
            .arg(qApp->applicationName());

    ui->setupUi(this);
    mUpdateManager = new SAKUpdateManager(this);

#ifdef Q_OS_ANDROID
    setWindowFlags(Qt::FramelessWindowHint);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    scrollArea->setWidget(mTabWidget);
#else
//    ui->tabWidget->addTab(mToolBoxs, tr("ToolBoxs"));
//    ui->tabWidget->addTab(new SAKModbusDebugger(mSettings), tr("Modbus"));
//    ui->tabWidget->addTab(new SAKCanBusDebugger(mSettings), tr("CAN"));

    QString title = QString(tr("Qt Swiss Army Knife"));
    title.append(QString(" "));
    title.append(QString("v"));
    title.append(qobject_cast<SAKApplication*>(qApp)->applicationVersion());
    title.append(" ");
    title.append(SAK_EDITION);
    setWindowTitle(title);
#endif

    // Initializing menu bar
    initMenuBar();

    // Connecting the signal of tab page to it's slot.
    mToolBoxs->setTabsClosable(true);
    connect(mToolBoxs, &QTabWidget::tabCloseRequested,
            this, &SAKMainWindow::removeRemovableDebugPage);
    connect(mToolBoxs, &QTabWidget::currentChanged, this, [=](int index){
        QString key = mSettingsKeyContext.currentTabPage;
        sakApp->settings()->setValue(key, index);
    });
#if 0
    // Create debugger, the operation will emit the signal named currentChanged.
    // So you should block it,
    // or the value of setting option(mSettingsKeyContext.currentTabPage)
    // will be 0 aways.
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    mTabWidget->blockSignals(true);
    for (int i = 0; i < metaEnum.keyCount(); i++){
#ifdef QT_DEBUG
#ifdef SAK_IMPORT_MODULE_TEST
        // Test page is selectable, it is for developer of the project.
        QString key = mSettingsKeyContext.enableTestPage;
        bool enableTestPage = sakApp->settings()->value(key).toBool();
        if (!enableTestPage && (metaEnum.value(i) ==
                                SAKDebuggerFactory::DebugPageTypeTest)){
            continue;
        }
#endif
#endif
        // The page can not be closed.
        QWidget *page = sakDebuggerFactor->createDebugger(metaEnum.value(i));
        if (page){
            mTabWidget->addTab(page, page->windowTitle());
            appendWindowAction(page);
        }
    }
    mTabWidget->blockSignals(false);
#else
    //ui->widgetNav->setLayout(new QVBoxLayout());
    //ui->widgetNav->layout()->setContentsMargins(0, 0, 0, 0);
    auto nav = ui->verticalLayoutNav;
    mToolBoxs->blockSignals(true);
    mNavBtGroup = new QButtonGroup(this);
    QList<int> types = SAKToolBoxUi::supportedCommuniticationTools();
    QString indexKey = mSettingsKeyContext.currentTabPage;
    for (int i = 0; i < types.count(); i++) {
        int type = types.at(i);
        SAKToolBoxUi *toolBoxUi = new SAKToolBoxUi(this);
        toolBoxUi->initialize(type);
        QToolButton *bt = new QToolButton();
        bt->setToolTip(toolBoxUi->windowTitle());
        bt->setIcon(toolBoxUi->windowIcon());
        bt->setToolButtonStyle(Qt::ToolButtonIconOnly);
        nav->layout()->addWidget(bt);
        ui->stackedWidget->addWidget(toolBoxUi);
        bt->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        connect(bt, &QToolButton::clicked, this, [=](){
            ui->stackedWidget->setCurrentIndex(i);
            sakApp->settings()->setValue(indexKey, i);
        });

        bt->setCheckable(true);
        bt->setAutoRaise(true);
        mNavBtGroup->addButton(bt);
    }
    mToolBoxs->blockSignals(false);
    nav->layout()->addWidget(new QLabel(" "));

    int index = sakApp->settings()->value(indexKey).toInt();
    if (index >= 0 && index < ui->stackedWidget->count()) {
        ui->stackedWidget->setCurrentIndex(index);
    }

    if (index >= 0 && index < mNavBtGroup->buttons().count()) {
        auto bt = qobject_cast<QToolButton*>(mNavBtGroup->buttons().at(index));
        if (bt) {
            bt->setChecked(true);
        }
    }
#endif
    if (mWindowsMenu){
        mWindowsMenu->addSeparator();
    }

    // Set the current page to last time
    QString key = mSettingsKeyContext.currentTabPage;
    int currentPage = sakApp->settings()->value(key).toInt();
    mToolBoxs->setCurrentIndex(currentPage);

    // Hide the close button,
    // the step must be done after calling setTabsClosable() function.
    for (int i = 0; i < mToolBoxs->count(); i++){
        mToolBoxs->tabBar()->setTabButton(i, QTabBar::RightSide, Q_NULLPTR);
        mToolBoxs->tabBar()->setTabButton(i, QTabBar::LeftSide, Q_NULLPTR);
    }
#if 0
    int count = mTabWidget->tabBar()->count();
    int maxWidth = std::numeric_limits<int>::min();
    for (int i = 0; i < count; i++) {
        int w = mTabWidget->tabBar()->tabRect(i).width();
        if (w > maxWidth) {
            maxWidth = w;
        }
    }
    mTabWidget->setStyleSheet(QString("QTabBar::tab{width:%1px;}").arg(maxWidth));
#endif
}

SAKMainWindow::~SAKMainWindow()
{
    sakMainWindow = Q_NULLPTR;
    delete ui;
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

void SAKMainWindow::closeEvent(QCloseEvent *event)
{
    bool isExitToSystemTray =
            mSettings->value(mSettingsKeyContext.exitToSystemTray).toBool();
    if (isExitToSystemTray) {
        this->hide();
        event->ignore();
    }
}

void SAKMainWindow::initFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("Exit"), this);
    fileMenu->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void SAKMainWindow::initToolMenu()
{
    QMenu *toolMenu = new QMenu(tr("&Tools"));
    menuBar()->addMenu(toolMenu);

    for (auto &var : sakToolsFactory->supportedToolsContext()){
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

    initOptionMenuAppStyleMenu(optionMenu);
    initOptionMenuMainWindowMenu(optionMenu);
    initOptionMenuSettingsMenu(optionMenu);
#ifdef QT_DEBUG
    initOptionMenuTestPageAction(optionMenu);
#endif
    initOptionMenuUiType(optionMenu);
    initOptionMenuHdpiPolicy(optionMenu);
}

void SAKMainWindow::initOptionMenuAppStyleMenu(QMenu *optionMenu)
{
    // Initializing application style menu.
    QMenu *appStyleMenu = new QMenu(tr("Application Style"), this);
    optionMenu->addMenu(appStyleMenu);
    auto styleKeys = QStyleFactory::keys();
    QList<QAction*> actionsList;
    mActionGroup = new QActionGroup(this);
    for (QString &var : styleKeys){
        QAction *action = new QAction(var, this);
        action->setObjectName(var);
        action->setCheckable(true);
        actionsList.append(action);
        mActionGroup->addAction(action);
        connect(action, &QAction::triggered, this, [=](){
            QString style = qobject_cast<QAction*>(sender())->objectName();
            QString styleKey = sakApp->settingsKeyContext()->appStyle;
            sakApp->setStyle(style);
            sakApp->settings()->setValue(styleKey, style);
        });
    }

    // Readin the specified style.
    QString styleKey = sakApp->settingsKeyContext()->appStyle;
    QString style = sakApp->settings()->value(styleKey).toString();
    if (!style.isEmpty()){
        for (QAction *var : actionsList){
            if (var->objectName().compare(style) == 0){
                QString styleKey = sakApp->settingsKeyContext()->appStyle;
                var->blockSignals(true);
                var->setChecked(true);
                var->blockSignals(false);
                sakApp->setStyle(style);
                sakApp->settings()->setValue(styleKey, style);
                break;
            }
        }
    }

    appStyleMenu->addActions(actionsList);
}

void SAKMainWindow::initOptionMenuMainWindowMenu(QMenu *optionMenu)
{
    if (!optionMenu) {
        return;
    }

    QMenu *mainWindowMenu = new QMenu(tr("Main Window"), this);
    QAction *exitToSystemTrayAction =
            new QAction(tr("Exit to Sysytem Tray"), this);
    exitToSystemTrayAction->setCheckable(true);
    mainWindowMenu->addAction(exitToSystemTrayAction);
    optionMenu->addMenu(mainWindowMenu);

    QVariant v = mSettings->value(mSettingsKeyContext.exitToSystemTray);
    if (!v.isNull()) {
        bool isExitToSystemTray = v.toBool();
        exitToSystemTrayAction->setChecked(isExitToSystemTray);
    }

    connect(exitToSystemTrayAction, &QAction::triggered, this, [=](){
        bool isExitToSystemTray = exitToSystemTrayAction->isChecked();
        mSettings->setValue(mSettingsKeyContext.exitToSystemTray,
                            isExitToSystemTray);
    });
}

void SAKMainWindow::initOptionMenuSettingsMenu(QMenu *optionMenu)
{
    QMenu *menu = new QMenu(tr("Settings"), this);
    optionMenu->addMenu(menu);

    QAction *action = new QAction(tr("Clear Configuration"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered,
            this, &SAKMainWindow::clearConfiguration);
    action = new QAction(tr("Open configuration floder"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=](){
        QUrl fileUrl = mSettings->fileName();
        QString floderUrl = mSettings->fileName().remove(fileUrl.fileName());
        QDesktopServices::openUrl(floderUrl);
    });
}

void SAKMainWindow::initOptionMenuTestPageAction(QMenu *optionMenu)
{
    optionMenu->addSeparator();
    mTestPageAction = new QAction(tr("Enable Testing Page"), this);
    optionMenu->addAction(mTestPageAction);
    mTestPageAction->setCheckable(true);
    connect(mTestPageAction, &QAction::triggered,
            this, &SAKMainWindow::testPageActionTriggered);
    QString enableTestPageKey = mSettingsKeyContext.enableTestPage;
    bool enableTestPage =
            sakApp->settings()->value(enableTestPageKey).toBool();
    if (enableTestPage){
        mTestPageAction->setChecked(true);
    }else{
        mTestPageAction->setChecked(false);
    }
}

void SAKMainWindow::initOptionMenuUiType(QMenu *optionMenu)
{
    QMenu *menu = new QMenu(tr("UI Type"));
    QActionGroup *ag = new QActionGroup(this);
    QAction *classicalAction = new QAction(tr("Classical"), this);
    classicalAction->setCheckable(true);
    connect(classicalAction, &QAction::triggered, this, [=](){
        SAKSettings::instance()->setUiType(SAKSettings::UiTypeWidget);
        rebootRequestion();
    });

    QAction *modernAction =  new QAction(tr("Modern"), this);
    modernAction->setCheckable(true);
    connect(modernAction, &QAction::triggered, this, [=](){
        SAKSettings::instance()->setUiType(SAKSettings::UiTypeQml);
        rebootRequestion();
    });

    ag->addAction(classicalAction);
    ag->addAction(modernAction);
    if (SAKSettings::instance()->uiType() == SAKSettings::UiTypeQml) {
        modernAction->setChecked(true);
    } else {
        classicalAction->setChecked(true);
    }

    menu->addAction(classicalAction);
    menu->addAction(modernAction);
    optionMenu->addMenu(menu);
}

void SAKMainWindow::initOptionMenuHdpiPolicy(QMenu *optionMenu)
{
    QMenu *menu = new QMenu(tr("HDPI Policy"));
    QActionGroup *ag = new QActionGroup(this);

    QAction *roundAction = new QAction(tr("Round up for .5 and above."), this);
    QAction *ceilAction = new QAction(tr("Always round up."), this);
    QAction *floorAction = new QAction(tr("Always round down."), this);
    QAction *rpfAction = new QAction(tr("Round up for .75 and above."), this);
    QAction *passThroughAction = new QAction(tr("Don't round."), this);

    auto round = Qt::HighDpiScaleFactorRoundingPolicy::Round;
    auto ceil = Qt::HighDpiScaleFactorRoundingPolicy::Ceil;
    auto floor = Qt::HighDpiScaleFactorRoundingPolicy::Floor;
    auto rpf = Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor;
    auto passThrough = Qt::HighDpiScaleFactorRoundingPolicy::PassThrough;

    connect(roundAction, &QAction::triggered, this, [=](){
        SAKSettings::instance()->setHdpiPolicy(int(round));
    });
    connect(ceilAction, &QAction::triggered, this, [=](){
        SAKSettings::instance()->setHdpiPolicy(int(ceil));
    });
    connect(floorAction, &QAction::triggered, this, [=](){
        SAKSettings::instance()->setHdpiPolicy(int(floor));
    });
    connect(rpfAction, &QAction::triggered, this, [=](){

        SAKSettings::instance()->setHdpiPolicy(int(rpf));
    });
    connect(passThroughAction, &QAction::triggered, this, [=](){

        SAKSettings::instance()->setHdpiPolicy(int(passThrough));
    });

    ag->addAction(roundAction);
    ag->addAction(ceilAction);
    ag->addAction(floorAction);
    ag->addAction(rpfAction);
    ag->addAction(passThroughAction);

    auto list = ag->actions();
    for (auto &a : list) {
        a->setCheckable(true);
    }

    if (SAKSettings::instance()->hdpiPolicy() == int(round)) {
        roundAction->setChecked(true);
    } else if (SAKSettings::instance()->hdpiPolicy() == int(ceil)) {
        ceilAction->setChecked(true);
    } else if (SAKSettings::instance()->hdpiPolicy() == int(floor)) {
        floorAction->setChecked(true);
    } else if (SAKSettings::instance()->hdpiPolicy() == int(rpf)) {
        rpfAction->setChecked(true);
    } else if (SAKSettings::instance()->hdpiPolicy() == int(passThrough)) {
        passThroughAction->setChecked(true);
    }

    menu->addActions(ag->actions());
    optionMenu->addMenu(menu);
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

    QString key = sakApp->settingsKeyContext()->language;
    QString language = sakApp->settings()->value(key).toString();

    QFile file(":/translations/sak/Translations.json");
    file.open(QFile::ReadOnly);
    QByteArray jsonData = file.readAll();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument =
            QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error == QJsonParseError::NoError){
        if (jsonDocument.isArray()){
            QJsonArray jsonArray = jsonDocument.array();
            struct info {
                QString locale;
                QString language;
                QString name;
            };
            QVector<info> infoVector;

            for (int i = 0; i < jsonArray.count(); i++){
                QJsonObject jsonObject = jsonArray.at(i).toObject();
                struct info languageInfo;
                languageInfo.locale = jsonObject.value("locale").toString();
                languageInfo.language =
                        jsonObject.value("language").toString();
                languageInfo.name = jsonObject.value("name").toString();
                infoVector.append(languageInfo);
            }

            mLanguagesActionGroup = new QActionGroup(this);
            for(auto &var:infoVector){
                QAction *action = new QAction(var.name, languageMenu);
                languageMenu->addAction(action);
                action->setCheckable(true);
                mLanguagesActionGroup->addAction(action);
                action->setObjectName(var.language);
                action->setData(QVariant::fromValue<QString>(var.name));
                action->setIcon(QIcon(QString(":/translations/sak/%1")
                                      .arg(var.locale).toLatin1()));
                connect(action, &QAction::triggered,
                        this, &SAKMainWindow::installLanguage);

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
    connect(aboutQtAction,
            &QAction::triggered,
            this,
            [=](){QMessageBox::aboutQt(this, tr("About Qt"));});

    QAction *aboutAction = new QAction(tr("About Application"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered,
            this, &SAKMainWindow::aboutQsak);

    QMenu *srcMenu = new QMenu(tr("Get Source"), this);
    helpMenu->addMenu(srcMenu);
    QAction *visitGitHubAction =
            new QAction(QIcon(":/resources/images/GitHub.png"),
                        tr("GitHub"), this);
    connect(visitGitHubAction, &QAction::triggered,
            [](){
        QUrl url = QUrl(QLatin1String(SAK_GITHUB_REPOSITORY_URL));
        QDesktopServices::openUrl(url);
    });
    srcMenu->addAction(visitGitHubAction);
    QAction *visitGiteeAction =
            new QAction(QIcon(":/resources/images/Gitee.png"),
                        tr("Gitee"), this);
    connect(visitGiteeAction, &QAction::triggered,
            [](){
        QUrl url = QUrl(QLatin1String(SAK_GITEE_REPOSITORY_URL));
        QDesktopServices::openUrl(url);
    });
    srcMenu->addAction(visitGiteeAction);

    QAction *updateAction = new QAction(tr("Check for Update"), this);
    helpMenu->addAction(updateAction);
    connect(updateAction, &QAction::triggered,
            mUpdateManager, &SAKUpdateManager::show);

    QAction *releaseHistoryAction = new QAction(tr("Release History"), this);
    helpMenu->addAction(releaseHistoryAction);
    connect(releaseHistoryAction, &QAction::triggered,
            this, &SAKMainWindow::showReleaseHistoryActionDialog);

    helpMenu->addSeparator();
    QAction *qrCodeAction = new QAction(tr("QR Code"), this);
    helpMenu->addAction(qrCodeAction);
    connect(qrCodeAction, &QAction::triggered,
            this, &SAKMainWindow::showQrCodeDialog);

    helpMenu->addAction(tr("支持作者"), this,
                        &SAKMainWindow::onDonationActionTriggered);
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
    linkList << Link {
                tr("Qt Official Download"),
                QString("http://download.qt.io/official_releases/qt"),
                QString(":/resources/images/Qt.png")}
             << Link {
                tr("Qt Official Blog"),
                QString("https://www.qt.io/blog"),
                QString(":/resources/images/Qt.png")}
             << Link {tr("Qt Official Release"),
                QString("https://wiki.qt.io/Qt_5.15_Release"),
                QString(":/resources/images/Qt.png")}
             << Link {tr("Download SAK from Github"),
                QString("%1/releases").arg(SAK_GITHUB_REPOSITORY_URL),
                QString(":/resources/images/GitHub.png")}
             << Link {tr("Download SAK from Gitee"),
                QString("%1/releases").arg(SAK_GITEE_REPOSITORY_URL),
                QString(":/resources/images/Gitee.png")}
             << Link {tr("Office Web Site"),
                QString("https://qsaker.gitee.io/qsak/"),
                QString(":/resources/images/Gitee.png")};

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
    QString baseUrl = "https://gitee.com/qsaker";
    linkList << Link{tr("Qt SerialPort Demo"),
                QString(baseUrl + "/qt-demo-serial-port-widget.git"),
                QString(":/resources/images/Qt.png")};

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

    QString format = QLocale::system().dateFormat();
    QString buildTimeString = sakApp->buildTime()->toString("hh:mm:ss");
    auto dateTimeString = sakApp->buildDate()->toString(format);
    dateTimeString = dateTimeString.append(" ");
    dateTimeString = dateTimeString.append(buildTimeString);
    QList<Info> infoList;
    infoList << Info{tr("Version"),
                QString(qApp->applicationVersion()), false}
             << Info{tr("Edition"), SAK_EDITION, false}
             << Info{tr("Author"), QString(SAK_AUTHOR), false}
             << Info{tr("Email"), QString(SAK_AUTHOR_EMAIL), false}
             << Info{tr("QQ"), QString("QQ:2869470394"), false}
             << Info{tr("QQ Group"), QString("QQ:952218522"), false}
             << Info{tr("Build Time"), dateTimeString, false}
             << Info{tr("Gitee Url"), QString("<a href=%1>%1</a>")
                .arg(SAK_GITEE_REPOSITORY_URL), true}
             << Info{tr("Gitbub Url"), QString("<a href=%1>%1</a>")
                .arg(SAK_GITHUB_REPOSITORY_URL), true}
             << Info{tr("Copyright"),
                tr("Copyright 2018-%1 Qsaker(qsaker@outlook.com)."
                   " All rights reserved.")
                .arg(sakApp->buildDate()->toString("yyyy")), false};

    QDialog dialog(this);
    dialog.setWindowTitle(tr("About QSAK"));

    QGridLayout *gridLayout = new QGridLayout(&dialog);
    int i = 0;
    for (auto &var : infoList){
        QLabel *nameLabel = new QLabel(
                    QString("<font color=green>%1</font>").arg(var.name),
                    &dialog
                    );
        QLabel *valueLabel = new QLabel(var.value, &dialog);
        gridLayout->addWidget(nameLabel, i, 0, 1, 1);
        gridLayout->addWidget(valueLabel, i, 1, 1, 1);
        i += 1;

        if (var.valueIsUrl){
            connect(valueLabel, &QLabel::linkActivated,
                    [](QString url){
                QDesktopServices::openUrl(QUrl(url));
            });
        }
    }
    dialog.setLayout(gridLayout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void SAKMainWindow::removeRemovableDebugPage(int index)
{
    QWidget *w = mToolBoxs->widget(index);
    mToolBoxs->removeTab(index);
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
    sakApp->settings()->setValue(mSettingsKeyContext.enableTestPage,
                                 QVariant::fromValue(checked));
    rebootRequestion();
}

void SAKMainWindow::clearConfiguration()
{
    sakApp->settings()->setValue(
                sakApp->settingsKeyContext()->appStyle,
                QString(""));
    sakApp->settings()->setValue(
                sakApp->settingsKeyContext()->removeSettingsFile,
                QVariant::fromValue(true));
    rebootRequestion();
}

void SAKMainWindow::rebootRequestion()
{
    int ret = QMessageBox::information(
                this,
                tr("Reboot application to effective"),
                tr("Need to reboot, reboot to effective now?"),
                QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        qApp->closeAllWindows();
        qApp->exit(SAK_REBOOT_CODE);
    }
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

void SAKMainWindow::showQrCodeDialog()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("QR Code"));

    struct QrCodeInfo {
        QString title;
        QString qrCode;
    };
    QList<QrCodeInfo> qrCodeInfoList;

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"),
                      QString(":/resources/images/QSAKQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"),
                      QString(":/resources/images/QtQQ.jpg")};

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

void SAKMainWindow::onDonationActionTriggered()
{
    QDialog dialog(this);
    dialog.setModal(true);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(&dialog);
    QString image = ":/resources/images/WeChat.jpg";
    QLabel *label = new QLabel(&dialog);
    label->setPixmap(QPixmap::fromImage(QImage(image)));
    hBoxLayout->addWidget(label);
    dialog.layout()->addWidget(label);
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
                mToolBoxs->setCurrentWidget(widget);
            }else{
                widget->activateWindow();
            }
        }
    }
}

void SAKMainWindow::installLanguage()
{
    if (sender()) {
        if (sender()->inherits("QAction")) {
            QAction *action = reinterpret_cast<QAction*>(sender());
            action->setChecked(true);

            QString language = action->objectName();
            QString name = action->data().toString();
            QString value = language + "-" + name;
            QString key = sakApp->settingsKeyContext()->language;
            mSettings->setValue(key, value);
            qobject_cast<SAKApplication*>(qApp)->installLanguage();
            rebootRequestion();
        }
    }
}
