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

#include "sakbase64assistant.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  SAKBase64Assisatnt assistant;
  assistant.show();

  return app.exec();
}
