/**********************************************************************************
 * Copyright 2020-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#ifndef SAKNUMBERASSISTANT_H
#define SAKNUMBERASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKNumberAssistant;
}

class SAKCommonInterface;
class SAKNumberAssistant : public QWidget {
  Q_OBJECT
 public:
  Q_INVOKABLE SAKNumberAssistant(QWidget* parent = Q_NULLPTR);
  ~SAKNumberAssistant();

 private:
  SAKCommonInterface* common_interface_;

 private:
  void FixedLength(QStringList& stringList);

 private:
  Ui::SAKNumberAssistant* ui_;

 private slots:
  void OnHexRawDataCheckBoxClicked();
  void OnCreatePushButtonClicked();
  void OnRawDataLineEditTextChanged(const QString& text);
  void OnBigEndianCheckBoxClicked();
  void OnFloatRadioButtonClicked();
  void OnDoubleRadioButtonClicked();
};

#endif  // SAKNUMBERASSISTANT_H
