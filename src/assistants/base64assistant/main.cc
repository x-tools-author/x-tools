/**********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#include <QApplication>

#include "sakbase64assistant.h"
#include "sakcommonmainwindow.h"

int main(int argc, char* argv[]) {
  QApplication* app = CreateCommonMainWindowApplication<SAKBase64Assisatnt>(
      argc, argv, QObject::tr("Base64 Assisatnt"), "SAK.Base64Assisatnt");
  int ret = app->exec();
  app->deleteLater();

  return ret;
}
