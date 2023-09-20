/*******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QAction>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMetaEnum>
#include <QSettings>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTranslator>

namespace Ui {
class SAKMainWindow;
}

class SAKMainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit SAKMainWindow(QWidget* parent = Q_NULLPTR);
  ~SAKMainWindow();

#ifdef Q_OS_WIN
 protected:
  void closeEvent(QCloseEvent* event);
#endif

 private:
  struct SettingsKeyContext {
    const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
  } mSettingsKey;
  const QLoggingCategory mLoggingCategory{"sak.mainwindow"};
  Ui::SAKMainWindow* ui;

 private:
  void initMenuBar();
  void initFileMenu();
  void initToolMenu();
  void initOptionMenu();
  void initOptionMenuAppStyleMenu(QMenu* optionMenu);
  void initOptionMenuMainWindowMenu(QMenu* optionMenu);
  void initOptionMenuSettingsMenu(QMenu* optionMenu);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  void initOptionMenuHdpiPolicy(QMenu* optionMenu);
#endif
  void initOptionMenuPalette(QMenu* optionMenu);
  void initWindowMenu();
  void initLanguageMenu();
  void initHelpMenu();
  void initLinksMenu();
  void initDemoMenu();
  void initNav();
  void initNav(QButtonGroup* bg, const QIcon& icon, const QString& name,
               QWidget* page, QToolBar* tb);
  void initStatusBar();

  void aboutSoftware();
  void clearConfiguration();
  void rebootRequestion();
  void showHistory();
  void showQrCode();
  void showDonation();
  void createQtConf();

 private slots:
  void onImportActionTriggered();
  void onExportActionTriggered();
};

#endif  // MAINWINDOW_H
