/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKCOMMONMAINWINDOW_H
#define SAKCOMMONMAINWINDOW_H

#include <QActionGroup>
#include <QApplication>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>
#include <QStyleFactory>

#include "sakinterface.h"
#include "saksettings.h"

class SAKCommonMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    SAKCommonMainWindow(QWidget* parent = Q_NULLPTR);
    ~SAKCommonMainWindow();

protected:
    QMenu* file_menu_;
    QMenu* option_menu_;
    QMenu* language_menu_;
    QMenu* help_menu_;

private:
    QActionGroup* app_style_action_group_;
    QActionGroup* language_action_group_;
    const QLoggingCategory logging_category_{"SAK.CommonMainWindow"};

private:
    void Init();
    void InitMenu();
    void InitMenuFile();
    void InitMenuOption();
    void InitMenuLanguage();
    void InitMenuHelp();

    void InitOptionMenuAppStyleMenu();
    void InitOptionMenuSettingsMenu();
    void InitOptionMenuHdpiPolicy();

    void OnHdpiPolicyActionTriggered(int policy);
    void OnGithubActionTriggered();
    void OnGiteeActionTriggered();
    void OnUserQqGroupTriggerd();
    void OnAboutActionTriggered();

    void TryToReboot();
    void CreateQtConf();
    QString GetQtConfFileName();
};

template<typename T>
QApplication* sakNewAssistantApp(int argc, char* argv[], const QString& title)
{
    QCoreApplication::setOrganizationName(QString("Qsaker"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(QString(title).remove(" "));

    // Application style.
    QString style = SAKSettings::instance()->appStyle();
    if (!style.isEmpty() && QStyleFactory::keys().contains(style)) {
        QApplication::setStyle(QStyleFactory::create(style));
    }

    // High dpi settings.
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    int policy = SAKSettings::instance()->hdpiPolicy();
    if (SAKInterface::isQtHighDpiScalePolicy(policy)) {
        auto cookedPolicy = Qt::HighDpiScaleFactorRoundingPolicy(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    }
#endif

    QApplication* app = new QApplication(argc, argv);

    SAKCommonMainWindow* mainWindow = new SAKCommonMainWindow();
    T* assistant = new T(mainWindow);
    mainWindow->setWindowTitle(title);
    mainWindow->setCentralWidget(assistant);
    mainWindow->showMinimized();
    mainWindow->resize(mainWindow->height() * 1.732, mainWindow->height());
    mainWindow->show();

    return app;
}

#endif // SAKCOMMONMAINWINDOW_H
