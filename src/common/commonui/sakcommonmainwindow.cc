/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakcommonmainwindow.h"

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

SAKCommonMainWindow::SAKCommonMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    app_style_action_group_ = new QActionGroup(this);
    language_action_group_ = new QActionGroup(this);
    QString language = SAKSettings::instance()->language();
    SAKTranslator::instance()->setupLanguage(language);
    Init();
}

SAKCommonMainWindow::~SAKCommonMainWindow() {}

void SAKCommonMainWindow::Init()
{
    InitMenu();
}

void SAKCommonMainWindow::InitMenu()
{
    InitMenuFile();
    InitMenuOption();
    InitMenuLanguage();
    InitMenuHelp();
}

void SAKCommonMainWindow::InitMenuFile()
{
    QMenuBar* menu_bar = menuBar();
    file_menu_ = menu_bar->addMenu(tr("&File"));
    file_menu_->addAction(tr("&Exit"), this, &SAKCommonMainWindow::close);
}

void SAKCommonMainWindow::InitMenuOption()
{
    option_menu_ = new QMenu(tr("&Options"));
    menuBar()->addMenu(option_menu_);

    InitOptionMenuAppStyleMenu();
    InitOptionMenuSettingsMenu();
    InitOptionMenuHdpiPolicy();
}

void SAKCommonMainWindow::InitMenuLanguage()
{
    language_menu_ = new QMenu(tr("&Languages"), this);
    menuBar()->addMenu(language_menu_);

    QStringList languages = SAKTranslator::instance()->languanges();
    for (auto& language : languages) {
        QAction* action = new QAction(language, this);
        action->setCheckable(true);
        language_menu_->addAction(action);
        language_action_group_->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            SAKSettings::instance()->setLanguage(language);
            TryToReboot();
        });

        QString setting_language = SAKSettings::instance()->language();
        if (setting_language == language) {
            action->setChecked(true);
        }
    }
}

void SAKCommonMainWindow::InitMenuHelp()
{
    QMenuBar* menu_bar = menuBar();
    help_menu_ = menu_bar->addMenu(tr("&Help"));
    help_menu_->addAction(QIcon(":/resources/images/GitHub.png"),
                          "Github",
                          this,
                          &SAKCommonMainWindow::OnGithubActionTriggered);
    help_menu_->addAction(QIcon(":/resources/images/Gitee.png"),
                          "Gitee",
                          this,
                          &SAKCommonMainWindow::OnGiteeActionTriggered);
    help_menu_->addAction(QIcon(":/resources/icon/IconQQGray.svg"),
                          tr("User QQ Group"),
                          this,
                          &SAKCommonMainWindow::OnUserQqGroupTriggerd);
    help_menu_->addAction(tr("&About"), this, &SAKCommonMainWindow::OnAboutActionTriggered);
}

void SAKCommonMainWindow::InitOptionMenuAppStyleMenu()
{
    QList<QAction*> actions = app_style_action_group_->actions();
    for (auto action : actions) {
        app_style_action_group_->removeAction(action);
    }

    QMenu* appStyleMenu = new QMenu(tr("Application Style"), this);
    option_menu_->addMenu(appStyleMenu);
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
        app_style_action_group_->addAction(action);

        if (key == style) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            SAKSettings::instance()->setAppStyle(key);
            TryToReboot();
        });
    }

    appStyleMenu->addActions(app_style_action_group_->actions());
}

void SAKCommonMainWindow::InitOptionMenuSettingsMenu()
{
    QMenu* menu = new QMenu(tr("Settings"), this);
    option_menu_->addMenu(menu);

    QAction* action = new QAction(tr("Clear Configuration"), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, [=]() {
        SAKSettings::instance()->setClearSettings(true);
        TryToReboot();
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

void SAKCommonMainWindow::InitOptionMenuHdpiPolicy()
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
            OnHdpiPolicyActionTriggered(policy.first);
        });
    }
    menu->addActions(action_group->actions());
    option_menu_->addMenu(menu);
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
        CreateQtConf();
        TryToReboot();
    });
#endif
#endif
}

void SAKCommonMainWindow::OnHdpiPolicyActionTriggered(int policy)
{
    if (QFile::remove(GetQtConfFileName())) {
        qCInfo(logging_category_) << GetQtConfFileName() << "was removed!";
    } else {
        qCInfo(logging_category_) << "removed" << GetQtConfFileName() << "failed";
    }

    SAKSettings::instance()->setHdpiPolicy(int(policy));
    TryToReboot();
}

void SAKCommonMainWindow::OnGithubActionTriggered()
{
    QDesktopServices::openUrl(QUrl(SAK_GITHUB_REPOSITORY_URL));
}

void SAKCommonMainWindow::OnGiteeActionTriggered()
{
    QDesktopServices::openUrl(QUrl(SAK_GITEE_REPOSITORY_URL));
}

void SAKCommonMainWindow::OnUserQqGroupTriggerd()
{
    QPixmap pix;
    if (!pix.load(":/resources/images/QSAKQQ.jpg")) {
        qCWarning(logging_category_) << "Can not load QSAKQQ.jpg.";
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

void SAKCommonMainWindow::OnAboutActionTriggered()
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
    info += tr("Copyright 2023-%1 Qsaker(qsaker@foxmail.com). "
               "All rights reserved.")
                .arg(year);
    QMessageBox::about(this, tr("About"), info);
}

void SAKCommonMainWindow::TryToReboot()
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

void SAKCommonMainWindow::CreateQtConf()
{
    QString fileName = GetQtConfFileName();
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        QTextStream out(&file);
        out << "[Platforms]\nWindowsArguments = dpiawareness=0\n";
        file.close();
    } else {
        qCWarning(logging_category_) << fileName;
        qCWarning(logging_category_) << "can not open file:" << file.errorString();
    }
}

QString SAKCommonMainWindow::GetQtConfFileName()
{
    return qApp->applicationDirPath() + "/qt.conf";
}
