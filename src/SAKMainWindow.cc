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
#include <QPainter>
#include <QProcess>
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

#include "SAKLogUi.hh"
#include "SAKSettings.hh"
#include "SAKToolBoxUi.hh"
#include "SAKMainWindow.hh"
#include "SAKTranslator.hh"
#include "SAKApplication.hh"
#include "SAKPreferencesUi.hh"
#include "SAKAssistantsFactory.hh"

#ifdef SAK_IMPORT_MODULE_CANBUSUI
#include "SAKCanBusUi.hh"
#endif

#ifdef SAK_IMPORT_MODULE_MODBUSUI
#include "SAKModbusUi.hh"
#endif

#include "ui_SAKMainWindow.h"

SAKMainWindow::SAKMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SAKMainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_ANDROID
    setWindowFlags(Qt::FramelessWindowHint);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    scrollArea->setWidget(mTabWidget);
#else

    QString title = QString(tr("Qt Swiss Army Knife"));
    title.append(QString(" "));
    title.append(QString("v"));
    title.append(qobject_cast<SAKApplication*>(qApp)->applicationVersion());
    title.append(" ");
    title.append(SAK_EDITION);
    setWindowTitle(title);
    setWindowIcon(QIcon(":/resources/images/SAKLogo.png"));
#endif

    initMenuBar();    
    initNav();
}

SAKMainWindow::~SAKMainWindow()
{
    delete ui;
}

void SAKMainWindow::initMenuBar()
{
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initLanguageMenu();
    initLinksMenu();
    initDemoMenu();
    initHelpMenu();
}

#ifdef Q_OS_WIN
void SAKMainWindow::closeEvent(QCloseEvent *event)
{
    auto key = mSettingsKey.exitToSystemTray;
    bool ignore = SAKSettings::instance()->value(key).toBool();
    if (ignore) {
        this->hide();
        event->ignore();
    }
}
#endif

void SAKMainWindow::initFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    QMenu *windowMenu = new QMenu(tr("New Window"), this);
    fileMenu->addMenu(windowMenu);
    QList<int> toolTypeList = SAKToolBoxUi::supportedCommuniticationTools();
    for (auto &toolType : toolTypeList) {
        const QString name = SAKToolBoxUi::communiticationToolName(toolType);
        QAction *action = new QAction(name, this);
        windowMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=](){
            SAKToolBoxUi *w = new SAKToolBoxUi();
            w->setAttribute(Qt::WA_DeleteOnClose, true);
            w->initialize(toolType);
            w->show();
        });
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

    for (auto &t : SAKAssistantsFactory::instance()->supportedAssistants()) {
        QString name = SAKAssistantsFactory::instance()->assistantName(t);
        QAction *action = new QAction(name, this);
        QWidget *w = SAKAssistantsFactory::instance()->newAssistant(t);

        Q_ASSERT_X(action, __FUNCTION__, "A null action!");
        Q_ASSERT_X(w, __FUNCTION__, "A null assistant widget!");

        w->hide();
        toolMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=](){
            if (w->isHidden()) {
                w->show();
            } else {
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
#ifdef Q_OS_WIN
    initOptionMenuMainWindowMenu(optionMenu);
#endif
    initOptionMenuSettingsMenu(optionMenu);
    initOptionMenuUiType(optionMenu);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    initOptionMenuHdpiPolicy(optionMenu);
#endif
}

void SAKMainWindow::initOptionMenuAppStyleMenu(QMenu *optionMenu)
{
    // Initializing application style menu.
    static QActionGroup gActionGroup(this);
    auto actions = gActionGroup.actions();
    for (auto action : actions) {
        gActionGroup.removeAction(action);
    }

    QMenu *appStyleMenu = new QMenu(tr("Application Style"), this);
    optionMenu->addMenu(appStyleMenu);
    auto keys = QStyleFactory::keys();
    for (QString &key : keys){
        QAction *action = new QAction(key, this);
        action->setObjectName(key);
        action->setCheckable(true);

        gActionGroup.addAction(action);

        connect(action, &QAction::triggered, this, [=](){
            SAKSettings::instance()->setAppStyle(key);
            rebootRequestion();
        });
    }

    appStyleMenu->addActions(gActionGroup.actions());

    // Reading the specified style.
    QString style = SAKSettings::instance()->appStyle();

    if (style.isEmpty()){
        return;
    }

    for (QAction *action : gActionGroup.actions()){
        if (action->objectName() == style){
            action->setChecked(true);
            break;
        }
    }
}

void SAKMainWindow::initOptionMenuMainWindowMenu(QMenu *optionMenu)
{
    if (!optionMenu) {
        return;
    }

    QMenu *mainWindowMenu = new QMenu(tr("Main Window"), this);
    QAction *action = new QAction(tr("Exit to Sysytem Tray"), this);
    action->setCheckable(true);
    mainWindowMenu->addAction(action);
    optionMenu->addMenu(mainWindowMenu);

    QVariant v = SAKSettings::instance()->value(mSettingsKey.exitToSystemTray);
    if (!v.isNull()) {
        bool isExitToSystemTray = v.toBool();
        action->setChecked(isExitToSystemTray);
    }

    connect(action, &QAction::triggered, this, [=](){
        bool keep = action->isChecked();
        SAKSettings::instance()->setValue(mSettingsKey.exitToSystemTray, keep);
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
        QString fileName = SAKSettings::instance()->fileName();
        QUrl fileUrl = QUrl(fileName);
        QString floderUrl = fileName.remove(fileUrl.fileName());
        QDesktopServices::openUrl(floderUrl);
    });
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
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
#endif

void SAKMainWindow::initLanguageMenu()
{
    QMenu *languageMenu = new QMenu(tr("&Languages"), this);
#if 0
    QIcon icon = QIcon(":/resources/icon/IconLanguage.svg");
    languageMenu->setIcon(icon);
#endif
    menuBar()->addMenu(languageMenu);

    static QActionGroup ag(this);
    QStringList languages = SAKTranslator::instance()->languanges();
    for (auto &language : languages) {
        QAction *action = new QAction(language, this);
        action->setCheckable(true);
        languageMenu->addAction(action);
        ag.addAction(action);

        connect(action, &QAction::triggered, this, [=](){
            SAKSettings::instance()->setLanguage(language);
            rebootRequestion();
        });

        QString l = SAKSettings::instance()->language();
        if (l == language) {
            action->setChecked(true);
        }
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
            this, &SAKMainWindow::aboutSoftware);

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

    QAction *releaseHistoryAction = new QAction(tr("Release History"), this);
    helpMenu->addAction(releaseHistoryAction);
    connect(releaseHistoryAction, &QAction::triggered,
            this, &SAKMainWindow::showHistory);

    helpMenu->addSeparator();
    QAction *qrCodeAction = new QAction(tr("QR Code"), this);
    helpMenu->addAction(qrCodeAction);
    connect(qrCodeAction, &QAction::triggered,
            this, &SAKMainWindow::showQrCode);

    helpMenu->addAction(tr("Donate"), this,
                        &SAKMainWindow::showDonation);
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

QIcon cookedIcon(const QIcon &icon)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), qApp->palette().windowText().color());
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

void SAKMainWindow::initNav()
{
    static QButtonGroup navButtonGroup;
    QList<int> types = SAKToolBoxUi::supportedCommuniticationTools();
    for (int i = 0; i < types.count(); i++) {
        int type = types.at(i);
        SAKToolBoxUi *toolBoxUi = new SAKToolBoxUi(this);
        toolBoxUi->initialize(type);

        initNav(&navButtonGroup, cookedIcon(toolBoxUi->windowIcon()),
                toolBoxUi->windowTitle(), toolBoxUi);
    }

    auto line = new QToolButton(this);
    line->setMaximumHeight(2);
    line->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->verticalLayoutNav->addWidget(line);

#ifdef SAK_IMPORT_MODULE_MODBUS
    initNav(&navButtonGroup,
            cookedIcon(QIcon(":/resources/icon/IconModbus.svg")),
            "Modbus Studio", new SAKModbusUi());
#endif
#ifdef SAK_IMPORT_MODULE_CANBUSUI
    initNav(&navButtonGroup,
            cookedIcon(QIcon(":/resources/icon/IconCanBus.svg")),
            "CAN Bus Studio", new SAKCanBusUi());
#endif
    ui->verticalLayoutNav->addWidget(new QLabel(" "));
    initNav(&navButtonGroup,
            cookedIcon(QIcon(":/resources/icon/IconLog.svg")),
            tr("Log Viewer"), new SAKLogUi(this));
    initNav(&navButtonGroup,
            cookedIcon(QIcon(":/resources/icon/IconSettings.svg")),
            tr("Preferences"), new SAKPreferencesUi(this));
#if 1
    bool isTextBesideIcon = SAKSettings::instance()->isTextBesideIcon();
    auto style = isTextBesideIcon ? Qt::ToolButtonTextBesideIcon
                                  : Qt::ToolButtonIconOnly;
    QToolButton *tbt = new QToolButton(this);
    tbt->setIcon(cookedIcon(QIcon(":/resources/icon/IconListWithIcon.svg")));
    tbt->setCheckable(true);
    tbt->setText(tr("Hide Text"));
    tbt->setToolTip(tr("Click to show(hide) nav text"));
    tbt->setAutoRaise(true);
    tbt->setChecked(isTextBesideIcon);
    tbt->setToolButtonStyle(style);
    tbt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->verticalLayoutNav->addWidget(tbt);
    auto bg = &navButtonGroup;
    connect(tbt, &QToolButton::clicked, tbt, [=](){
        auto bts = bg->buttons();
        auto style = tbt->isChecked() ? Qt::ToolButtonTextBesideIcon
                                      : Qt::ToolButtonIconOnly;
        tbt->setToolButtonStyle(style);
        for (auto &bt : bts) {
            auto cookedBt = qobject_cast<QToolButton*>(bt);

            cookedBt->setToolButtonStyle(style);
        }
        SAKSettings::instance()->setIsTextBesideIcon(tbt->isChecked());
    });
#endif
}

void SAKMainWindow::initNav(QButtonGroup *bg, const QIcon &icon,
                            const QString &name, QWidget *page)
{
    bool isTextBesideIcon = SAKSettings::instance()->isTextBesideIcon();
    auto style = isTextBesideIcon ? Qt::ToolButtonTextBesideIcon
                                  : Qt::ToolButtonIconOnly;
    QToolButton *bt = new QToolButton();
    bt->setAutoRaise(true);
    bt->setIcon(icon);
    bt->setCheckable(true);
    bt->setToolButtonStyle(style);
    bt->setToolTip(name);
    bt->setText(name);
    bt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
#if 0
    bt->setIconSize(QSize(32, 32));
#endif
    bg->addButton(bt);
    ui->verticalLayoutNav->addWidget(bt);

    if (page->layout()) {
        page->layout()->setContentsMargins(0, 0, 0, 0);
    }
    ui->stackedWidget->addWidget(page);

    int pageCount = bg->buttons().count();
    QObject::connect(bt, &QToolButton::clicked, bt, [=](){
        ui->stackedWidget->setCurrentIndex(pageCount - 1);
        SAKSettings::instance()->setPageIndex(pageCount - 1);
    });

    if (SAKSettings::instance()->pageIndex() == (pageCount -1)) {
        bt->setChecked(true);
        ui->stackedWidget->setCurrentIndex(pageCount - 1);
    }
}

void SAKMainWindow::aboutSoftware()
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
                tr("Copyright 2018-%1 Qsaker(qsaker@foxmail.com)."
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

void SAKMainWindow::clearConfiguration()
{
    SAKSettings::instance()->setClearSettings(true);
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
        QProcess::startDetached(QCoreApplication::applicationFilePath());

        qApp->closeAllWindows();
        qApp->exit();
    }
}

void SAKMainWindow::showHistory()
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

void SAKMainWindow::showQrCode()
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

void SAKMainWindow::showDonation()
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
