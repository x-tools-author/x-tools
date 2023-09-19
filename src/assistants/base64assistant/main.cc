/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#include <QApplication>
#include <QDebug>
#include <QLoggingCategory>
#include <QStyleFactory>

#include "SAKInterface.h"
#include "SAKSettings.h"
#include "sakbase64assistant.h"
#include "sakcommonmainwindow.h"

int main(int argc, char* argv[]) {
  QCoreApplication::setOrganizationName(QString("Qsaker"));
  QCoreApplication::setOrganizationDomain(QString("IT"));
  QCoreApplication::setApplicationName(QString("Base64Assistant"));

  // Application style.
  QLoggingCategory logging_category{"SAK.Base64Assistant"};
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

  QApplication app(argc, argv);

  SAKCommonMainWindow main_window;
  SAKBase64Assisatnt* central_widget = new SAKBase64Assisatnt(&main_window);
  main_window.setWindowTitle(central_widget->windowTitle());
  main_window.setCentralWidget(central_widget);
  main_window.resize(main_window.height() * 1.732, main_window.height());
  main_window.show();

  return app.exec();
}
