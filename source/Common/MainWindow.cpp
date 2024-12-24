/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QUrl>

#include "Common/xTools.h"

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "StyleSheetManager.h"
#endif

namespace xTools {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_fileMenu(nullptr)
    , m_optionMenu(nullptr)
    , m_languageMenu(nullptr)
    , m_helpMenu(nullptr)
    , m_exitAction(nullptr)
    , m_gitHubAction(nullptr)
    , m_giteeAction(nullptr)
    , m_qqGroupAction(nullptr)
    , m_aboutAction(nullptr)
    , m_aboutQtAction(nullptr)
    , m_appStyleActionGroup(nullptr)
    , m_languageActionGroup(nullptr)
    , m_appPaletteActionGroup(nullptr)
{
    m_appStyleActionGroup = new QActionGroup(this);
    m_languageActionGroup = new QActionGroup(this);
    m_appPaletteActionGroup = new QActionGroup(this);

    initMenuFile();
    initMenuOption();
    initMenuLanguage();
    initMenuHelp();

    MainWindow::updateWindowTitle();
}

QIcon MainWindow::cookedIcon(const QString& svgFileName)
{
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    QSvgRenderer renderer(svgFileName);
    QImage image(QSize(128, 128), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // Transparent background

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    renderer.render(&painter);

    // Change color
    QColor color = StyleSheetManager::singleton().themeColor("primaryColor");
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor ic = image.pixelColor(x, y);
            if (ic.alpha() > 0) { // If not transparent
                image.setPixelColor(x, y, color);
            }
        }
    }

    return QIcon{QPixmap::fromImage(image)};
#else
    return QIcon(svgFileName);
#endif
}

QIcon MainWindow::cookedIcon(const QIcon& icon)
{
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

    QColor color = StyleSheetManager::singleton().themeColor("primaryColor");
    painter.fillRect(pixmap.rect(), color);
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
#else
    return icon;
#endif
}

void MainWindow::updateWindowTitle()
{
    xTools& xTools = xTools::singleton();
    QString title = xTools.appFriendlyName();
    title += " v";
    title += QApplication::applicationVersion();
    setWindowTitle(title);
}

QString MainWindow::qtConfFileName()
{
    return QCoreApplication::applicationDirPath() + "/qt.conf";
}

void MainWindow::initMenuFile()
{
    QMenuBar* menuBar = this->menuBar();
    m_fileMenu = menuBar->addMenu(tr("&File"));
    m_exitAction = m_fileMenu->addAction(tr("Exit Application"), this, []() {
        QApplication::closeAllWindows();
        QApplication::quit();
    });
    m_exitAction->setShortcut(QKeySequence::Quit);
}

void MainWindow::initMenuOption()
{
    m_optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(m_optionMenu);

    initOptionMenuHdpiPolicy();
    initOptionMenuAppStyleMenu();
    initOptionMenuColorScheme();
    m_optionMenu->addSeparator();
    initOptionMenuSettingsMenu();
}

void MainWindow::initMenuLanguage()
{
    m_languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(m_languageMenu);

    xTools& xTools = xTools::singleton();
    QStringList languages = xTools.languageSupportedLanguages();
    QString settingLanguage = xTools.settingsLanguage();
    for (auto& language : languages) {
        auto* action = new QAction(language, this);
        action->setCheckable(true);
        m_languageMenu->addAction(action);
        m_languageActionGroup->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            g_xTools.languageSetupAppLanguage(language);
            g_xTools.tryToReboot();
        });

        if (settingLanguage.isEmpty()) {
            if (language == xTools.languageDefaultLanguage()) {
                action->setChecked(true);
            }
        } else {
            if (language == settingLanguage) {
                action->setChecked(true);
            }
        }
    }
}

void MainWindow::initMenuHelp()
{
    QMenuBar* menuBar = this->menuBar();
    m_helpMenu = menuBar->addMenu(tr("&Help"));
    m_aboutQtAction = m_helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
    m_aboutAction = m_helpMenu->addAction(tr("About") + " " + QApplication::applicationName());
#if defined(QT_DEBUG)
    m_helpMenu->addAction(tr("Screenshot"), this, [=]() {
        QPixmap pix = this->grab();
        // copy to clipboard
        QApplication::clipboard()->setPixmap(pix);
    });
#endif

    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);
}

void MainWindow::initOptionMenuAppStyleMenu()
{
    xTools& xTools = xTools::singleton();
    m_appStyleMenu = new QMenu(tr("Application Style"), this);
    m_optionMenu->addMenu(m_appStyleMenu);
    QStringList keys = QStyleFactory::keys();
    const QString style = xTools.settingsAppStyle();
    for (QString& key : keys) {
        auto* action = new QAction(key, this);
        action->setObjectName(key);
        action->setCheckable(true);
        m_appStyleActionGroup->addAction(action);

        if (key == style || key.toLower() == style) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            g_xTools.settingsSetAppStyle(key);
            tryToReboot();
        });
    }

    m_appStyleMenu->addActions(m_appStyleActionGroup->actions());

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    m_themeAction = m_optionMenu->addMenu(StyleSheetManager::singleton().themeMenu());
#endif
}

void MainWindow::initOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    m_optionMenu->addMenu(menu);

    auto clearAction = new QAction(tr("Clear Settings"), this);
    menu->addAction(clearAction);
    connect(clearAction, &QAction::triggered, this, [=]() {
        xTools& xTools = xTools::singleton();
        xTools.settingsSetClearSettings(true);
        tryToReboot();
    });

    auto openAction = new QAction(tr("Open Settings Directory"), this);
    menu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, [=]() {
        xTools& xTools = xTools::singleton();
        QDesktopServices::openUrl(xTools.settingsPath());
    });
}

void MainWindow::initOptionMenuHdpiPolicy()
{
    QMenu* menu = new QMenu(tr("HDPI Policy"));
    QActionGroup* actionGroup = new QActionGroup(this);
    xTools& xTools = xTools::singleton();
    int currentPolicy = xTools.settingsHdpiPolicy();
    auto supportedPolicies = xTools.hdpiSupportedPolicies();
    for (auto& policy : supportedPolicies) {
        auto name = xTools.hdpiPolicyName(policy.toInt());
        auto action = menu->addAction(name, this, [=]() {
            onHdpiPolicyActionTriggered(policy.toInt());
        });

        actionGroup->addAction(action);
        action->setCheckable(true);
        if (policy.toInt() == currentPolicy) {
            action->setChecked(true);
        }
    }
    menu->addActions(actionGroup->actions());
    m_optionMenu->addMenu(menu);
}

void MainWindow::initOptionMenuColorScheme()
{
    m_colorSchemeMenu = new QMenu(tr("Color Scheme"));
    static QActionGroup actionGroup(this);
    auto sys = m_colorSchemeMenu->addAction(tr("System"));
    auto dark = m_colorSchemeMenu->addAction(tr("Dark"));
    auto light = m_colorSchemeMenu->addAction(tr("Light"));

    sys->setCheckable(true);
    dark->setCheckable(true);
    light->setCheckable(true);

    actionGroup.addAction(sys);
    actionGroup.addAction(dark);
    actionGroup.addAction(light);

    m_colorSchemeMenu->addActions(actionGroup.actions());
    m_optionMenu->addMenu(m_colorSchemeMenu);

    connect(&actionGroup, &QActionGroup::triggered, this, [=](QAction* action) {
        xTools& xTools = xTools::singleton();
        if (action == dark) {
            xTools.settingsSetColorScheme(static_cast<int>(Qt::ColorScheme::Dark));
        } else if (action == light) {
            xTools.settingsSetColorScheme(static_cast<int>(Qt::ColorScheme::Light));
        } else {
            xTools.settingsSetColorScheme(static_cast<int>(Qt::ColorScheme::Unknown));
        }

        auto currentScheme = xTools.settingsColorScheme();
        QStyleHints* styleHints = QApplication::styleHints();
        styleHints->setColorScheme(static_cast<Qt::ColorScheme>(currentScheme));
    });

    xTools& xTools = xTools::singleton();
    auto currentScheme = xTools.settingsColorScheme();
    switch (currentScheme) {
    case static_cast<int>(Qt::ColorScheme::Dark):
        dark->setChecked(true);
        break;
    case static_cast<int>(Qt::ColorScheme::Light):
        light->setChecked(true);
        break;
    default:
        currentScheme = static_cast<int>(Qt::ColorScheme::Unknown);
        sys->setChecked(true);
        break;
    }

    QStyleHints* styleHints = QApplication::styleHints();
    styleHints->setColorScheme(static_cast<Qt::ColorScheme>(currentScheme));
}

void MainWindow::onHdpiPolicyActionTriggered(int policy)
{
    xTools& xTools = xTools::singleton();
    xTools.settingsSetHdpiPolicy(int(policy));
    tryToReboot();
}

void MainWindow::onAboutActionTriggered()
{
    xTools& xTools = xTools::singleton();
    QString buildDateTimeFormat = xTools.dtSystemDateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += xTools.dtSystemTimeFormat();
    QString buildDateTimeString = xTools.dtBuildDateTimeString(buildDateTimeFormat);
    QString year = xTools.dtBuildDateTimeString("yyyy");
    const QString version = xTools.xToolsVersion();
    const QString name = qApp->applicationName();
    QString info;
    info += name + QString(" ") + version + " " + tr("(A Part of xTools Project)") + "\n\n";
#ifdef X_TOOLS_GIT_COMMIT
    info += tr("Commit") + QString("(xTools)") + ": " + X_TOOLS_GIT_COMMIT + "\n\n";
#endif
#ifdef X_APP_GIT_COMMIT
    info += tr("Commit") + QString("(%1)").arg(name) + ": " + X_APP_GIT_COMMIT + "\n\n";
#endif
    info += tr("Build Date") + ": " + buildDateTimeString + "\n\n";
    info += QString("Copyright 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About"), info);
}

bool MainWindow::tryToReboot()
{
    return g_xTools.tryToReboot();
}

void MainWindow::createQtConf()
{
    QString fileName = qtConfFileName();
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        QTextStream out(&file);
        out << "[Platforms]\nWindowsArguments = dpiawareness=0\n";
        file.close();
    } else {
        auto info = QString("Open file(%1) failed: %2").arg(fileName, file.errorString());
        qWarning() << qPrintable(info);
    }
}

void MainWindow::showQqQrCode()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("QR Code"));

    typedef QPair<QString, QString> QrCodeInfo;
    QList<QPair<QString, QString>> qrCodeInfoList;
    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/resources/Images/UserQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/resources/Images/QtQQ.jpg")};

    QTabWidget* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        QLabel* label = new QLabel(tabWidget);
        label->setPixmap(QPixmap::fromImage(QImage(var.second)));
        tabWidget->addTab(label, var.first);
    }

    QHBoxLayout* layout = new QHBoxLayout(&dialog);
    layout->addWidget(tabWidget);
    dialog.setLayout(layout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

} // namespace xTools
