/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "xToolsMainWindow.h"

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
#include <QSvgRenderer>
#include <QTimer>
#include <QUrl>

#include "xToolsApplication.h"
#include "xToolsSettings.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include "xToolsDataStructure.h"
#endif
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
#include "xToolsStyleSheetManager.h"
#endif

QString xToolsMainWindow::s_version = QString("0.0.0");
xToolsMainWindow::xToolsMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_xToolsApp = dynamic_cast<xToolsApplication*>(qApp);
    Q_ASSERT_X(m_xToolsApp, Q_FUNC_INFO, "The application is not xToolsApplication.");

    m_appStyleActionGroup = new QActionGroup(this);
    m_languageActionGroup = new QActionGroup(this);
    m_appPaletteActionGroup = new QActionGroup(this);

    initMenuFile();
    initMenuOption();
    initMenuLanguage();
    initMenuHelp();
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    connect(&xToolsStyleSheetManager::instance(),
            &xToolsStyleSheetManager::stylesheetChanged,
            this,
            [=]() { tryToReboot(); });
#endif
}

#include <QJsonObject>
QIcon xToolsMainWindow::cookedIcon(const QString& svgFileName)
{
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    QSvgRenderer renderer(svgFileName);
    QImage image(QSize(128, 128), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // Transparent background

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    renderer.render(&painter);

    // Change color
    QColor color = xToolsStyleSheetManager::instance().themeColor("primaryColor");
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor ic = image.pixelColor(x, y);
            if (ic.alpha() > 0) { // If not transparent
                image.setPixelColor(x, y, color);
            }
        }
    }

    return QIcon(QPixmap::fromImage(image));
#else
    return QIcon(svgFileName);
#endif
}

QString xToolsMainWindow::version()
{
    return s_version;
}

void xToolsMainWindow::setVersion(const QString& version)
{
    s_version = version;
}

QString xToolsMainWindow::qtConfFileName()
{
    return qApp->applicationDirPath() + "/qt.conf";
}

void xToolsMainWindow::initMenuFile()
{
    QMenuBar* menu_bar = menuBar();
    m_fileMenu = menu_bar->addMenu(tr("&File"));
#ifndef X_TOOLS_ENABLE_MODULE_STYLESHEET
    QAction* importAction = new QAction(tr("Import Palette"), m_fileMenu);
    m_fileMenu->addAction(importAction);
    connect(importAction, &QAction::triggered, this, &xToolsMainWindow::onImportActionTriggered);

    QAction* exportAction = new QAction(tr("Export Palette"), m_fileMenu);
    m_fileMenu->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &xToolsMainWindow::onExportActionTriggered);

    m_fileMenu->addSeparator();
#endif
    m_exitAction = m_fileMenu->addAction(tr("&Exit"), this, []() {
        QApplication::closeAllWindows();
        QApplication::quit();
    });
}

void xToolsMainWindow::initMenuOption()
{
    m_optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(m_optionMenu);

    initOptionMenuHdpiPolicy();
    initOptionMenuAppStyleMenu();
#ifndef X_TOOLS_ENABLE_MODULE_STYLESHEET
    initOptionMenuAppPaletteMenu();
#endif
    m_optionMenu->addSeparator();
    initOptionMenuSettingsMenu();
}

void xToolsMainWindow::initMenuLanguage()
{
    m_languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(m_languageMenu);

    xToolsApplication* app = dynamic_cast<xToolsApplication*>(qApp);
    if (!app) {
        qWarning() << "The application is not xToolsApplication.";
        return;
    }

    QStringList languages = app->supportedLanguages();
    QString settingLanguage = xToolsSettings::instance()->language();
    for (auto& language : languages) {
        auto* action = new QAction(language, this);
        action->setCheckable(true);
        m_languageMenu->addAction(action);
        m_languageActionGroup->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            xToolsSettings::instance()->setLanguage(language);
            tryToReboot();
        });

        if (settingLanguage.isEmpty()) {
            if (language == app->language()) {
                action->setChecked(true);
            }
        }
    }
}

void xToolsMainWindow::initMenuHelp()
{
    QMenuBar* menu_bar = menuBar();
    m_helpMenu = menu_bar->addMenu(tr("&Help"));
    m_aboutAction = m_helpMenu->addAction(tr("&About"));
    m_aboutQtAction = m_helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);

    connect(m_aboutAction, &QAction::triggered, this, &xToolsMainWindow::onAboutActionTriggered);
}

void xToolsMainWindow::initOptionMenuAppStyleMenu()
{
    QMenu* appStyleMenu = new QMenu(tr("Application Style"), this);
    m_optionMenu->addMenu(appStyleMenu);
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    initOptionMenuAppStyleMenuThirdParty(appStyleMenu);
#else
    initOptionMenuAppStyleMenuQt(appStyleMenu);
#endif
}

void xToolsMainWindow::initOptionMenuAppStyleMenuQt(QMenu* menu)
{
    QStringList keys = QStyleFactory::keys();
    QString style = xToolsSettings::instance()->appStyle();
    for (QString& key : keys) {
        QAction* action = new QAction(key, this);
        action->setObjectName(key);
        action->setCheckable(true);
        m_appStyleActionGroup->addAction(action);

        if (key == style || key.toLower() == style) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            xToolsSettings::instance()->setAppStyle(key);
            tryToReboot();
        });
    }

    menu->addActions(m_appStyleActionGroup->actions());
}

void xToolsMainWindow::initOptionMenuAppStyleMenuThirdParty(QMenu* menu)
{
#ifdef X_TOOLS_ENABLE_MODULE_STYLESHEET
    menu->addAction(tr("none"), this, [=]() {
        xToolsStyleSheetManager::instance().setThemeName("");
    });
    menu->addSeparator();
    menu->addActions(xToolsStyleSheetManager::instance().darkThemeMenu()->actions());
    menu->addSeparator();
    menu->addActions(xToolsStyleSheetManager::instance().lightThemeMenu()->actions());
#endif
}

void xToolsMainWindow::initOptionMenuAppPaletteMenu()
{
    const QString emptyPalette("");
    const QString darkPalette(":/Resources/Palettes/DarkPalette");
    const QString lightPalette(":/Resources/Palettes/LightPalette");

    QMenu* appPaletteMenu = new QMenu(tr("Application Palette"), this);
    m_optionMenu->addMenu(appPaletteMenu);
    auto defaultAction = appPaletteMenu->addAction(tr("Default"));
    connect(defaultAction, &QAction::triggered, this, [=]() { setPalette(emptyPalette); });
    auto darkAction = appPaletteMenu->addAction(tr("Dark"));
    connect(darkAction, &QAction::triggered, this, [=]() { setPalette(darkPalette); });
    auto lightAction = appPaletteMenu->addAction(tr("Light"));
    connect(lightAction, &QAction::triggered, this, [=]() { setPalette(lightPalette); });
    appPaletteMenu->addSeparator();
    auto customAction = appPaletteMenu->addAction(tr("Import"));
    connect(customAction, &QAction::triggered, this, &xToolsMainWindow::onImportActionTriggered);

    defaultAction->setCheckable(true);
    darkAction->setCheckable(true);
    lightAction->setCheckable(true);
    customAction->setCheckable(true);
    m_appPaletteActionGroup->addAction(defaultAction);
    m_appPaletteActionGroup->addAction(darkAction);
    m_appPaletteActionGroup->addAction(lightAction);
    m_appPaletteActionGroup->addAction(customAction);

    QMap<QString, QAction*> stringActionMap;
    QString paletteFile = xToolsSettings::instance()->palette();
    stringActionMap.insert(emptyPalette, defaultAction);
    stringActionMap.insert(darkPalette, darkAction);
    stringActionMap.insert(lightPalette, lightAction);
    if (stringActionMap.contains(paletteFile)) {
        stringActionMap.value(paletteFile)->setChecked(true);
    } else {
        customAction->setChecked(true);
        customAction->setToolTip(paletteFile);
    }
}

void xToolsMainWindow::initOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    m_optionMenu->addMenu(menu);

    auto clearAction = new QAction(tr("Clear Settings"), this);
    menu->addAction(clearAction);
    connect(clearAction, &QAction::triggered, this, [=]() {
        xToolsSettings::instance()->clear();
        tryToReboot();
    });

    auto openAction = new QAction(tr("Open Settings Directory"), this);
    menu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, [=]() {
        QString file_name = xToolsSettings::instance()->fileName();
        QUrl fileUrl = QUrl(file_name);
        QString path = file_name.remove(fileUrl.fileName());
        QDesktopServices::openUrl(path);
    });
}

void xToolsMainWindow::initOptionMenuHdpiPolicy()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QMenu* menu = new QMenu(tr("HDPI Policy"));
    QActionGroup* actionGroup = new QActionGroup(this);
    int currentPolicy = xToolsSettings::instance()->hdpiPolicy();
    auto supportedPolicies = xToolsDataStructure::supportedHighDpiPolicies();
    for (auto& policy : supportedPolicies) {
        auto name = xToolsDataStructure::highDpiPolicyName(policy.toInt());
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

void xToolsMainWindow::onHdpiPolicyActionTriggered(int policy)
{
    Q_UNUSED(policy)
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#ifdef Q_OS_WIN
    if (policy == xToolsDataStructure::HighDpiPolicySystem) {
        createQtConf();
        tryToReboot();
        return;
    }

    const QString confFileName = qtConfFileName();
    if (QFile::exists(confFileName) && !QFile::remove(confFileName)) {
        qWarning() << qPrintable(QString("Can not remove the file:").arg(confFileName));
        return;
    }
#endif
    xToolsSettings::instance()->setHdpiPolicy(int(policy));
    tryToReboot();
#endif
}

void xToolsMainWindow::onAboutActionTriggered()
{
    QString buildDateTimeFormat = xToolsApplication::systemDateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += xToolsApplication::systemTimeFormat();
    QString buildDateTimeString = xToolsApplication::buildDateTimeString(buildDateTimeFormat);
    QString year = xToolsApplication::buildDateTimeString("yyyy");
    QString info;
    info += windowTitle() + QString(" ") + s_version + " " + tr("(A Part of xTools Project)")
            + "\n\n";
#ifdef X_TOOLS_GIT_COMMIT
    info += tr("Commit") + ": " + X_TOOLS_GIT_COMMIT + "\n\n";
#endif
#ifdef X_TOOLS_PRIVATE_GIT_COMMIT
    info += tr("Commit") + ": " + X_TOOLS_PRIVATE_GIT_COMMIT + "\n\n";
#endif
    info += tr("Build Date") + ": " + buildDateTimeString + "\n\n";
    info += QString("© 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About"), info);
}

void xToolsMainWindow::onImportActionTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import"), "Palette", tr("All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    setPalette(fileName);
}

void xToolsMainWindow::onExportActionTriggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Export"), "Palette", tr("All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QPalette p = qApp->palette();
        QDataStream out(&file);
        out << p;
        file.close();
    } else {
        const QString errorString = file.errorString();
        QString message = tr("The operation(open file %1) failed: %2").arg(fileName, errorString);
        qWarning() << qPrintable(message);
        return;
    }
}

void xToolsMainWindow::tryToReboot()
{
    int ret = QMessageBox::information(this,
                                       tr("Reboot application to effective"),
                                       tr("Need to reboot, reboot to effective now?"),
                                       QMessageBox::Ok | QMessageBox::No,
                                       QMessageBox::No);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QCoreApplication::applicationFilePath());
        QTimer::singleShot(1000, this, [=]() {
            qApp->closeAllWindows();
            qApp->exit();
        });
    }
}

void xToolsMainWindow::createQtConf()
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

void xToolsMainWindow::showQqQrCode()
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

void xToolsMainWindow::setPalette(const QString& fileName)
{
    if (fileName.isEmpty()) {
        xToolsSettings::instance()->setPalette(fileName);
        tryToReboot();
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString message = tr("Open the file(%1) failed: %2").arg(fileName, file.errorString());
        qWarning() << qPrintable(message);
        return;
    }

    xToolsSettings::instance()->setPalette(fileName);
    m_xToolsApp->setupPalette(fileName);
    menuBar()->setPalette(qApp->palette());
    tryToReboot();
}
