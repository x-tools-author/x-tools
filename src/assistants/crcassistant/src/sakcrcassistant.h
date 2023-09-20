/**********************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#ifndef SAKCRCASSISTANT_H
#define SAKCRCASSISTANT_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>

namespace Ui {
class SAKCRCAssistant;
}

class SAKCommonCrcInterface;
class SAKCRCAssistant : public QWidget {
  Q_OBJECT
 public:
  Q_INVOKABLE SAKCRCAssistant(QWidget* parent = Q_NULLPTR);
  ~SAKCRCAssistant();

 protected:
  bool eventFilter(QObject* watched, QEvent* event);

 private:
  const char* log_category_;
  SAKCommonCrcInterface* crc_interface_;

 private:
  void InitParameterModel();
  void Calculate();
  void TextFormatControl();
  void ChangedParameterModel(int index);

 private:
  Ui::SAKCRCAssistant* mUi;
  QComboBox* mWidthComboBox;
  QComboBox* mParameterComboBox;
  QCheckBox* mRefinCheckBox;
  QCheckBox* mRefoutCheckBox;
  QLineEdit* mPolyLineEdit;
  QLineEdit* mInitLineEdit;
  QLineEdit* mXorLineEdit;
  QRadioButton* mHexRadioBt;
  QRadioButton* mAsciiRadioBt;
  QLineEdit* mHexCRCOutput;
  QLineEdit* mBinCRCOutput;
  QTextEdit* mInputTextEdit;
  QPushButton* mCalculatedBt;
  QLabel* mLabelPolyFormula;
  QLabel* mLabelInfo;
};
#endif
