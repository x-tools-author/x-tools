/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <QActionGroup>
#include <QApplication>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>
#include <QStyleFactory>

class xToolsMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    xToolsMainWindow(QWidget* parent = Q_NULLPTR);
    ~xToolsMainWindow();

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
