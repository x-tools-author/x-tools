/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakmainwindow.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QStyle>
#include <QStyleFactory>
#include <QUrl>

#include "sakdatastructure.h"
#include "sakinterface.h"
#include "saksettings.h"
#include "saktranslator.h"

SAKMainWindow::SAKMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_appStyleActionGroup = new QActionGroup(this);
    m_languageActionGroup = new QActionGroup(this);
    QString language = SAKSettings::instance()->language();
    SAKTranslator::instance()->setupLanguage(language);
    init();
}

SAKMainWindow::~SAKMainWindow() {}

void SAKMainWindow::init()
{
    initMenu();
}

void SAKMainWindow::initMenu()
{
    initMenuFile();
    initMenuOption();
    initMenuLanguage();
    initMenuHelp();
}

void SAKMainWindow::initMenuFile()
{
    QMenuBar* menu_bar = menuBar();
    m_fileMenu = menu_bar->addMenu(tr("&File"));
    m_fileMenu->addAction(tr("&Exit"), this, &SAKMainWindow::close);
}

void SAKMainWindow::initMenuOption()
{
    m_optionMenu = new QMenu(tr("&Options"));
    menuBar()->addMenu(m_optionMenu);

    initOptionMenuAppStyleMenu();
    initOptionMenuSettingsMenu();
    initOptionMenuHdpiPolicy();
}

void SAKMainWindow::initMenuLanguage()
{
    m_languageMenu = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(m_languageMenu);

    QStringList languages = SAKTranslator::instance()->languanges();
    for (auto& language : languages) {
        QAction* action = new QAction(language, this);
        action->setCheckable(true);
        m_languageMenu->addAction(action);
        m_languageActionGroup->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            SAKSettings::instance()->setLanguage(language);
            tryToReboot();
        });

        QString setting_language = SAKSettings::instance()->language();
        if (setting_language == language) {
            action->setChecked(true);
        }
    }
}

void SAKMainWindow::initMenuHelp()
{
    QMenuBar* menu_bar = menuBar();
    m_helpMenu = menu_bar->addMenu(tr("&Help"));
    m_helpMenu->addAction(QIcon(":/resources/images/GitHub.png"),
                          "Github",
                          this,
                          &SAKMainWindow::onGithubActionTriggered);
    m_helpMenu->addAction(QIcon(":/resources/images/Gitee.png"),
                          "Gitee",
                          this,
                          &SAKMainWindow::onGiteeActionTriggered);
    m_helpMenu->addAction(QIcon(":/resources/icon/IconQQGray.svg"),
                          tr("User QQ Group"),
                          this,
                          &SAKMainWindow::onUserQqGroupTriggerd);
    m_helpMenu->addAction(tr("&About"), this, &SAKMainWindow::onAboutActionTriggered);
}

void SAKMainWindow::initOptionMenuAppStyleMenu()
{
    QList<QAction*> actions = m_appStyleActionGroup->actions();
    for (auto action : actions) {
        m_appStyleActionGroup->removeAction(action);
    }

    QMenu* appStyleMenu = new QMenu(tr("Application Style"), this);
    m_optionMenu->addMenu(appStyleMenu);
    QStringList keys = QStyleFactory::keys();
    QString style = SAKSettings::instance()->appStyle();
    if (style.isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
        style = qApp->style()->name();
#endif
    }

    for (QString& key : keys) {
        QAction* action = new QAction(key, this);
        action->setObjectName(key);
        action->setCheckable(true);
        m_appStyleActionGroup->addAction(action);

        if (key == style) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            SAKSettings::instance()->setAppStyle(key);
            tryToReboot();
        });
    }

    appStyleMenu->addActions(m_appStyleActionGroup->actions());
}

void SAKMainWindow::initOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    m_optionMenu->addMenu(menu);

    QAction* action = new QAction(tr("Clear Configuration"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=]() {
        SAKSettings::instance()->setClearSettings(true);
        tryToReboot();
    });
    action = new QAction(tr("Open configuration floder"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=]() {
        QString file_name = SAKSettings::instance()->fileName();
        QUrl file_url = QUrl(file_name);
        QString floder_url = file_name.remove(file_url.fileName());
        QDesktopServices::openUrl(floder_url);
    });
}

void SAKMainWindow::initOptionMenuHdpiPolicy()
{
    QMenu* menu = new QMenu(tr("HDPI Policy"));
    QActionGroup* action_group = new QActionGroup(this);
    int setting_policy = SAKSettings::instance()->hdpiPolicy();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QList<QPair<int, QString>> policy_list;
    typedef QPair<int, QString> policy_pair;
    policy_list << policy_pair(SAKDataStructure::HdpiPolicyRound, tr("Round up for .5 and above"));
    policy_list << policy_pair(SAKDataStructure::HdpiPolicyCeil, tr("Always round up"));
    policy_list << policy_pair(SAKDataStructure::HdpiPolicyFloor, tr("Always round down"));
    policy_list << policy_pair(SAKDataStructure::HdpiPolicyRoundPreferFloor,
                               tr("Round up for .75 and above"));
    policy_list << policy_pair(SAKDataStructure::HdpiPolicyPassThrough, tr("Don't round"));
    for (auto& policy : policy_list) {
        QAction* action = new QAction(policy.second, this);
        action_group->addAction(action);
        if (setting_policy == policy.first) {
            action->setCheckable(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            onHdpiPolicyActionTriggered(policy.first);
        });
    }
    menu->addActions(action_group->actions());
    m_optionMenu->addMenu(menu);
#endif

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QAction* system_action = new QAction(tr("System"), this);
    system_action->setCheckable(true);
    action_group->addAction(system_action);
    menu->addSeparator();
    menu->addAction(system_action);

    if (setting_policy == SAKDataStructure::HdpiPolicySystem) {
        system_action->setChecked(true);
    }

    connect(system_action, &QAction::triggered, this, [=]() {
        SAKSettings::instance()->setHdpiPolicy(SAKDataStructure::HdpiPolicySystem);
        createQtConf();
        tryToReboot();
    });
#endif
#endif
}

void SAKMainWindow::onHdpiPolicyActionTriggered(int policy)
{
    if (QFile::remove(getQtConfFileName())) {
        qInfo() << getQtConfFileName() << "was removed!";
    } else {
        qInfo() << "removed" << getQtConfFileName() << "failed";
    }

    SAKSettings::instance()->setHdpiPolicy(int(policy));
    tryToReboot();
}

void SAKMainWindow::onGithubActionTriggered()
{
    QDesktopServices::openUrl(QUrl(SAK_GITHUB_REPOSITORY_URL));
}

void SAKMainWindow::onGiteeActionTriggered()
{
    QDesktopServices::openUrl(QUrl(SAK_GITEE_REPOSITORY_URL));
}

void SAKMainWindow::onUserQqGroupTriggerd()
{
    QPixmap pix;
    if (!pix.load(":/resources/images/QSAKQQ.jpg")) {
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

void SAKMainWindow::onAboutActionTriggered()
{
    QString year = SAKInterface::buildDateTime("yyyy");
    QString info;
    info += centralWidget()->windowTitle() + tr("(Part of Qt Swiss Army knife)");
    info += "\n";
    info += tr("Author: ") + SAK_AUTHOR;
    info += "\n";
    info += tr("Email: ") + SAK_AUTHOR_EMAIL;
    info += "\n";
    info += tr("Commit: ") + SAK_GIT_COMMIT;
    info += "\n";
    info += tr("Date: ") + SAK_GIT_COMMIT_TIME;
    info += "\n";
    info += tr("Copyright 2018-%1 Qsaker(qsaker@foxmail.com). All rights reserved.").arg(year);
    QMessageBox::about(this, tr("About"), info);
}

void SAKMainWindow::tryToReboot()
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

void SAKMainWindow::createQtConf()
{
    QString fileName = getQtConfFileName();
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

QString SAKMainWindow::getQtConfFileName()
{
    return qApp->applicationDirPath() + "/qt.conf";
}
