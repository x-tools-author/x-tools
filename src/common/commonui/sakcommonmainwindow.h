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
#include <QApplication>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMenu>
#include <QStyleFactory>

#include "SAKInterface.h"
#include "SAKSettings.h"

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

template <typename T>
QApplication* CreateCommonMainWindowApplication(
    int argc, char* argv[], const QString& title,
    const char* logging_category_path) {
  QCoreApplication::setOrganizationName(QString("Qsaker"));
  QCoreApplication::setOrganizationDomain(QString("IT"));
  QCoreApplication::setApplicationName(QString(title).remove(" "));

  // Application style.
  QLoggingCategory logging_category(logging_category_path);
  QString style = SAKSettings::instance()->appStyle();
  if (!style.isEmpty() && QStyleFactory::keys().contains(style)) {
    qCInfo(logging_category) << "The application style is:" << style;
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

  SAKCommonMainWindow* main_window = new SAKCommonMainWindow();
  T* assistant = new T(main_window);
  main_window->setWindowTitle(title);
  main_window->setCentralWidget(assistant);
  main_window->resize(main_window->height() * 1.732, main_window->height());
  main_window->show();

  return app;
}

#endif  // SAKCOMMONMAINWINDOW_H
