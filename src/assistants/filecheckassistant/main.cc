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

#include "sakcommonmainwindow.h"
#include "sakfilecheckassistant.h"

int main(int argc, char* argv[]) {
  QApplication* app = CreateCommonMainWindowApplication<SAKFileCheckAssistant>(
      argc, argv, QObject::tr("File Check Assistant"),
      "SAK.FileCheckAssistant");
  int ret = app->exec();
  app->deleteLater();

  return ret;
}
