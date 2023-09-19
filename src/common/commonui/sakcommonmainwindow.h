/******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMMONMAINWINDOW_H
#define SAKCOMMONMAINWINDOW_H

#include <QActionGroup>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>

class SAKCommonMainWindow : public QMainWindow {
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

#endif  // SAKCOMMONMAINWINDOW_H
