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

#include "App/Application.h"
#include "App/Settings.h"
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
    m_xToolsApp = dynamic_cast<Application*>(QCoreApplication::instance());
    Q_ASSERT_X(m_xToolsApp, Q_FUNC_INFO, "The application is not xToolsApplication.");

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

void MainWindow::updateWindowTitle()
{
    QString title = Application::friendlyAppName();
    title += " v";
    title += Application::applicationVersion();
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
    m_exitAction = m_fileMenu->addAction(tr("&Exit"), this, []() {
        QApplication::closeAllWindows();
        QApplication::quit();
    });
}

void MainWindow::initMenuOption()
{
    m_optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(m_optionMenu);

    initOptionMenuHdpiPolicy();
    initOptionMenuAppStyleMenu();
    m_optionMenu->addSeparator();
    initOptionMenuSettingsMenu();
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    initOptionMenuColorScheme();
#endif
}

void MainWindow::initMenuLanguage()
{
    m_languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(m_languageMenu);

    auto* app = dynamic_cast<Application*>(QCoreApplication::instance());
    if (!app) {
        qWarning() << "The application is not xToolsApplication.";
        return;
    }

    QStringList languages = app->supportedLanguages();
    QString settingLanguage = Settings::instance()->language();
    for (auto& language : languages) {
        auto* action = new QAction(language, this);
        action->setCheckable(true);
        m_languageMenu->addAction(action);
        m_languageActionGroup->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            Settings::instance()->setLanguage(language);
            tryToReboot();
        });

        if (settingLanguage.isEmpty()) {
            if (language == app->language()) {
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
    QMenuBar* menu_bar = menuBar();
    m_helpMenu = menu_bar->addMenu(tr("&Help"));
    m_aboutAction = m_helpMenu->addAction(tr("&About"));
    m_aboutQtAction = m_helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);

    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);
}

void MainWindow::initOptionMenuAppStyleMenu()
{
    auto* appStyleMenu = new QMenu(tr("Application Style"), this);
    m_optionMenu->addMenu(appStyleMenu);
    QStringList keys = QStyleFactory::keys();
    const QString style = Settings::instance()->appStyle();
    for (QString& key : keys) {
        auto* action = new QAction(key, this);
        action->setObjectName(key);
        action->setCheckable(true);
        m_appStyleActionGroup->addAction(action);

        if (key == style || key.toLower() == style) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            Settings::instance()->setAppStyle(key);
            tryToReboot();
        });
    }

    appStyleMenu->addActions(m_appStyleActionGroup->actions());

#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    m_optionMenu->addMenu(StyleSheetManager::singleton().themeMenu());
#endif
}

void MainWindow::initOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    m_optionMenu->addMenu(menu);

    auto clearAction = new QAction(tr("Clear Settings"), this);
    menu->addAction(clearAction);
    connect(clearAction, &QAction::triggered, this, [=]() {
        Settings::instance()->clear();
        tryToReboot();
    });

    auto openAction = new QAction(tr("Open Settings Directory"), this);
    menu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, [=]() {
        QString file_name = Settings::instance()->fileName();
        QUrl fileUrl = QUrl(file_name);
        QString path = file_name.remove(fileUrl.fileName());
        QDesktopServices::openUrl(path);
    });
}

void MainWindow::initOptionMenuHdpiPolicy()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QMenu* menu = new QMenu(tr("HDPI Policy"));
    QActionGroup* actionGroup = new QActionGroup(this);
    int currentPolicy = Settings::instance()->hdpiPolicy();
    auto supportedPolicies = xTools::supportedHighDpiPolicies();
    for (auto& policy : supportedPolicies) {
        auto name = xTools::highDpiPolicyName(policy.toInt());
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
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
void MainWindow::initOptionMenuColorScheme()
{
    QMenu* menu = new QMenu(tr("Color Scheme"));
    static QActionGroup actionGroup(this);
    auto sys = menu->addAction(tr("System"));
    auto dark = menu->addAction(tr("Dark"));
    auto light = menu->addAction(tr("Light"));

    sys->setCheckable(true);
    dark->setCheckable(true);
    light->setCheckable(true);

    actionGroup.addAction(sys);
    actionGroup.addAction(dark);
    actionGroup.addAction(light);

    menu->addActions(actionGroup.actions());
    m_optionMenu->addMenu(menu);

    connect(&actionGroup, &QActionGroup::triggered, this, [=](QAction* action) {
        if (action == dark) {
            Settings::instance()->setColorScheme(static_cast<int>(Qt::ColorScheme::Dark));
        } else if (action == light) {
            Settings::instance()->setColorScheme(static_cast<int>(Qt::ColorScheme::Light));
        } else {
            Settings::instance()->setColorScheme(static_cast<int>(Qt::ColorScheme::Unknown));
        }

        auto currentScheme = Settings::instance()->colorScheme();
        QStyleHints* styleHints = QApplication::styleHints();
        styleHints->setColorScheme(static_cast<Qt::ColorScheme>(currentScheme));
    });

    auto currentScheme = Settings::instance()->colorScheme();
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
#endif

void MainWindow::onHdpiPolicyActionTriggered(int policy)
{
    Q_UNUSED(policy)
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    Settings::instance()->setHdpiPolicy(int(policy));
    tryToReboot();
#endif
}

void MainWindow::onAboutActionTriggered()
{
    QString buildDateTimeFormat = Application::systemDateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += Application::systemTimeFormat();
    QString buildDateTimeString = Application::buildDateTimeString(buildDateTimeFormat);
    QString year = Application::buildDateTimeString("yyyy");
    const QString version = Application::applicationVersion();
    QString info;
    info += qApp->applicationName() + QString(" ") + version + " "
            + tr("(A Part of xTools Project)") + "\n\n";
#ifdef X_TOOLS_GIT_COMMIT
    info += tr("Commit") + ": " + X_TOOLS_GIT_COMMIT + "\n\n";
#endif
#ifdef X_TOOLS_PRIVATE_GIT_COMMIT
    info += tr("Commit") + ": " + X_TOOLS_PRIVATE_GIT_COMMIT + "\n\n";
#endif
    info += tr("Build Date") + ": " + buildDateTimeString + "\n\n";
    info += QString("Copyright 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About"), info);
}

bool MainWindow::tryToReboot()
{
    int ret = QMessageBox::information(this,
                                       tr("Reboot application to effective"),
                                       tr("Need to reboot, reboot to effective now?"),
                                       QMessageBox::Ok | QMessageBox::No,
                                       QMessageBox::No);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
        QTimer::singleShot(1000, this, [=]() {
            qApp->closeAllWindows();
            qApp->exit();
        });
        return true;
    }

    return false;
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
    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/Resources/Images/UserQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/Resources/Images/QtQQ.jpg")};

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
