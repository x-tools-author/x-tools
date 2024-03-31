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
#include <QProcess>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QUrl>

#include "xToolsApplication.h"
#include "xToolsSettings.h"

xToolsMainWindow::xToolsMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_xToolsApp = qobject_cast<xToolsApplication*>(qApp);
    Q_ASSERT_X(m_xToolsApp, Q_FUNC_INFO, "The application is not xToolsApplication.");

    m_appStyleActionGroup = new QActionGroup(this);
    m_languageActionGroup = new QActionGroup(this);
    m_appPaletteActionGroup = new QActionGroup(this);

    init();
}

QString xToolsMainWindow::qtConfFileName()
{
    return qApp->applicationDirPath() + "/qt.conf";
}

void xToolsMainWindow::init()
{
    initMenu();
}

void xToolsMainWindow::initMenu()
{
    initMenuFile();
    initMenuOption();
    initMenuLanguage();
    initMenuHelp();
}

void xToolsMainWindow::initMenuFile()
{
    QMenuBar* menu_bar = menuBar();
    m_fileMenu = menu_bar->addMenu(tr("&File"));

    QAction* importAction = new QAction(tr("Import Palette"), m_fileMenu);
    m_fileMenu->addAction(importAction);
    connect(importAction, &QAction::triggered, this, &xToolsMainWindow::onImportActionTriggered);

    QAction* exportAction = new QAction(tr("Export Palette"), m_fileMenu);
    m_fileMenu->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &xToolsMainWindow::onExportActionTriggered);

    m_fileMenu->addSeparator();
    m_exitAction = m_fileMenu->addAction(tr("&Exit"), this, &xToolsMainWindow::close);
}

void xToolsMainWindow::initMenuOption()
{
    m_optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(m_optionMenu);

    initOptionMenuHdpiPolicy();
    initOptionMenuAppStyleMenu();
    initOptionMenuAppPaletteMenu();
    m_optionMenu->addSeparator();
    initOptionMenuSettingsMenu();
}

void xToolsMainWindow::initMenuLanguage()
{
    m_languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(m_languageMenu);

    QStringList languages = qobject_cast<xToolsApplication*>(qApp)->supportedLanguages();
    QString settingLanguage = xToolsSettings::instance()->language();
    for (auto& language : languages) {
        QAction* action = new QAction(language, this);
        action->setCheckable(true);
        m_languageMenu->addAction(action);
        m_languageActionGroup->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            xToolsSettings::instance()->setLanguage(language);
            tryToReboot();
        });

        if (settingLanguage.isEmpty()) {
            settingLanguage = qobject_cast<xToolsApplication*>(qApp)->language();
        }
        if (settingLanguage == language) {
            action->setChecked(true);
        }
    }
}

void xToolsMainWindow::initMenuHelp()
{
    QMenuBar* menu_bar = menuBar();
    m_helpMenu = menu_bar->addMenu(tr("&Help"));
    m_aboutAction = m_helpMenu->addAction(tr("&About"),
                                          this,
                                          &xToolsMainWindow::onAboutActionTriggered);
    m_aboutQtAction = m_helpMenu->addAction(tr("About Qt"), this, [=]() {
        QMessageBox::aboutQt(this, tr("About Qt"));
    });
}

void xToolsMainWindow::initOptionMenuAppStyleMenu()
{
    QList<QAction*> actions = m_appStyleActionGroup->actions();
    for (auto& action : actions) {
        m_appStyleActionGroup->removeAction(action);
    }

    QMenu* appStyleMenu = new QMenu(tr("Application Style"), this);
    m_optionMenu->addMenu(appStyleMenu);
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

    appStyleMenu->addActions(m_appStyleActionGroup->actions());
}

void xToolsMainWindow::initOptionMenuAppPaletteMenu()
{
    if (!m_appPaletteActionGroup->actions().isEmpty()) {
        return;
    }
    const QString darkPalette(":/Resources/Palettes/DarkPalette");
    const QString lightPalette(":/Resources/Palettes/LightPalette");

    QMenu* appPaletteMenu = new QMenu(tr("Application Palette"), this);
    m_optionMenu->addMenu(appPaletteMenu);
    auto defaultAction = appPaletteMenu->addAction(tr("Default"), this, [=]() {
        setPalette(QString(""));
    });
    auto darkAction = appPaletteMenu->addAction(tr("Dark"), this, [=]() {
        setPalette(darkPalette);
    });
    auto lightAction = appPaletteMenu->addAction(tr("Light"), this, [=]() {
        setPalette(lightPalette);
    });
    appPaletteMenu->addSeparator();
    auto customAction = appPaletteMenu->addAction(tr("Import"),
                                                  this,
                                                  &xToolsMainWindow::onImportActionTriggered);
    defaultAction->setCheckable(true);
    darkAction->setCheckable(true);
    lightAction->setCheckable(true);
    customAction->setCheckable(true);

    m_appPaletteActionGroup->addAction(defaultAction);
    m_appPaletteActionGroup->addAction(darkAction);
    m_appPaletteActionGroup->addAction(lightAction);
    m_appPaletteActionGroup->addAction(customAction);

    QString paletteFile = xToolsSettings::instance()->palette();
    if (paletteFile.isEmpty()) {
        defaultAction->setChecked(true);
        customAction->setToolTip(paletteFile);
    } else if (paletteFile == darkPalette) {
        darkAction->setChecked(true);
    } else if (paletteFile == lightPalette) {
        lightAction->setChecked(true);
    } else {
        customAction->setToolTip(paletteFile);
        customAction->setChecked(true);
    }
}

void xToolsMainWindow::initOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    m_optionMenu->addMenu(menu);

    QAction* action = new QAction(tr("Clear Configuration"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=]() {
        xToolsSettings::instance()->setClearSettings(true);
        tryToReboot();
    });
    action = new QAction(tr("Open configuration floder"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=]() {
        QString file_name = xToolsSettings::instance()->fileName();
        QUrl file_url = QUrl(file_name);
        QString floder_url = file_name.remove(file_url.fileName());
        QDesktopServices::openUrl(floder_url);
    });
}

#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
#include "xToolsDataStructure.h"
#endif
void xToolsMainWindow::initOptionMenuHdpiPolicy()
{
#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
    QMenu* menu = new QMenu(tr("HDPI Policy"));
    QActionGroup* actionGroup = new QActionGroup(this);
    int currentPolicy = xToolsSettings::instance()->hdpiPolicy();
    auto supportedPolicies = xToolsDataStructure::supportedHighDpiPolicies();
    for (auto& policy : supportedPolicies) {
        auto name = xToolsDataStructure::highDpiPolicyName(policy.toInt());
        auto action = menu->addAction(name, this, [=]() {
            onHdpiPolicyActionTriggered(policy.toInt());
        });
        action->setCheckable(true);
        actionGroup->addAction(action);
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
#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
#ifdef Q_OS_WIN
    if (policy == xToolsDataStructure::HighDpiPolicySystem) {
        createQtConf();
        tryToReboot();
        return;
    }
#endif

    if (QFile::remove(qtConfFileName())) {
        qInfo() << qtConfFileName() << "was removed!";
    } else {
        qInfo() << "removed" << qtConfFileName() << "failed";
    }

    xToolsSettings::instance()->setHdpiPolicy(int(policy));
    tryToReboot();
#else
    Q_UNUSED(policy)
#endif
}

void xToolsMainWindow::onGithubActionTriggered()
{
    QDesktopServices::openUrl(QUrl(X_TOOLS_GITHUB_REPOSITORY_URL));
}

void xToolsMainWindow::onGiteeActionTriggered()
{
    QDesktopServices::openUrl(QUrl(X_TOOLS_GITEE_REPOSITORY_URL));
}

void xToolsMainWindow::onUserQqGroupTriggerd()
{
    QPixmap pix;
    if (!pix.load(":/Resources/Images/QSAKQQ.jpg")) {
        qWarning() << "Can not load QSAKQQ.jpg.";
        return;
    }

    QLabel* label = new QLabel(this);
    label->resize(pix.size());
    label->setPixmap(pix);
    QDialog dialog;
    dialog.setLayout(new QHBoxLayout());
    dialog.layout()->addWidget(label);
    dialog.setModal(true);
    dialog.exec();
}

void xToolsMainWindow::onAboutActionTriggered()
{
    QString year = xToolsApplication::buildDateTime("yyyy");
    QString info;
    info += windowTitle() + QString(" ") + tr("(A Part of xTools Project)");
    info += "\n\n";
#ifdef X_TOOLS_GIT_COMMIT
    info += tr("Commit") + ": " + X_TOOLS_GIT_COMMIT;
    info += "\n\n";
#endif
#ifdef X_TOOLS_GIT_COMMIT_TIME
    info += tr("Date") + ": " + X_TOOLS_GIT_COMMIT_TIME;
    info += "\n\n";
#endif
    info += QString("© 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year)
            + tr("All rights reserved.");
    QMessageBox::about(this, tr("About"), info);
}

void xToolsMainWindow::onImportActionTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import"), "Palete", tr("All (*)"));
    if (fileName.isEmpty()) {
        qInfo() << "Importing palette had been cancled!";
        return;
    }

    setPalette(fileName);
}

void xToolsMainWindow::onExportActionTriggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Export"), "Palete", tr("All (*)"));
    if (fileName.isEmpty()) {
        qInfo() << "Exporting palette had been cancled!";
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString message = tr("Open file(%1) failed: %2").arg(fileName, file.errorString());
        qWarning() << qPrintable(message);
        return;
    }

    QPalette p = qApp->palette();
    QDataStream out(&file);
    out << p;
    file.close();
}

void xToolsMainWindow::tryToReboot()
{
    int ret = QMessageBox::information(this,
                                       tr("Reboot application to effective"),
                                       tr("Need to reboot, reboot to effective now?"),
                                       QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QCoreApplication::applicationFilePath());

        qApp->closeAllWindows();
        qApp->exit();
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
        qWarning() << fileName;
        qWarning() << "can not open file:" << file.errorString();
    }
}

void xToolsMainWindow::showQqQrCode()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("QR Code"));

    struct QrCodeInfo
    {
        QString title;
        QString qrCode;
    };
    QList<QrCodeInfo> qrCodeInfoList;

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/Resources/Images/UserQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/Resources/Images/QtQQ.jpg")};

    QTabWidget* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        QLabel* label = new QLabel(tabWidget);
        label->setPixmap(QPixmap::fromImage(QImage(var.qrCode)));
        tabWidget->addTab(label, var.title);
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
