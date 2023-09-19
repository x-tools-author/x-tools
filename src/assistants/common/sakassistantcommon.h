/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#ifndef SAKASCIIASSISTANTCOMMON_H
#define SAKASCIIASSISTANTCOMMON_H

#include <QApplication>
#include <QDebug>
#include <QLoggingCategory>
#include <QStyleFactory>

#include "SAKInterface.h"
#include "SAKSettings.h"
#include "sakcommonmainwindow.h"

template <typename T>
QApplication* CreateAssistant(int argc, char* argv[], const QString& app_name,
                              const char* logging_category_path) {
  QCoreApplication::setOrganizationName(QString("Qsaker"));
  QCoreApplication::setOrganizationDomain(QString("IT"));
  QCoreApplication::setApplicationName(app_name);

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
  main_window->setWindowTitle(assistant->windowTitle());
  main_window->setCentralWidget(assistant);
  main_window->resize(main_window->height() * 1.732, main_window->height());
  main_window->show();

  return app;
}

#endif  // SAKASCIIASSISTANTCOMMON_H
