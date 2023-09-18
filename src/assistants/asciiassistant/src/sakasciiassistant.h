/*******************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/

#ifndef SAKASCIIASSISTANT_H
#define SAKASCIIASSISTANT_H

#include <QDialog>

namespace Ui {
class SAKAsciiAssistant;
}

class SAKAsciiAssistant : public QDialog {
  Q_OBJECT
 public:
  Q_INVOKABLE SAKAsciiAssistant(QWidget* parent = Q_NULLPTR);
  ~SAKAsciiAssistant();

 private:
  Ui::SAKAsciiAssistant* ui_;
};

#endif  // SAKASCIIASSISTANT_H
