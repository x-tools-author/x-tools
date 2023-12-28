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
    QMenu* m_fileMenu;
    QMenu* m_optionMenu;
    QMenu* m_languageMenu;
    QMenu* m_helpMenu;

private:
    QActionGroup* m_appStyleActionGroup;
    QActionGroup* m_languageActionGroup;

private:
    void init();
    void initMenu();
    void initMenuFile();
    void initMenuOption();
    void initMenuLanguage();
    void initMenuHelp();

    void initOptionMenuAppStyleMenu();
    void initOptionMenuSettingsMenu();
    void initOptionMenuHdpiPolicy();

    void onHdpiPolicyActionTriggered(int policy);
    void onGithubActionTriggered();
    void onGiteeActionTriggered();
    void onUserQqGroupTriggerd();
    void onAboutActionTriggered();

    void tryToReboot();
    void createQtConf();
    QString getQtConfFileName();
};

#endif // SAKCOMMONMAINWINDOW_H
