/*
 * Copyright 2020 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLSTRINGASSISTANT_H
#define SAKTOOLSTRINGASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKStringAssistant;
}
class SAKToolStringAssistant : public QWidget {
  Q_OBJECT
 public:
  Q_INVOKABLE SAKToolStringAssistant(QWidget* parent = Q_NULLPTR);
  ~SAKToolStringAssistant();

 private:
  Ui::SAKStringAssistant* ui;
 private slots:
  void on_textEdit_textChanged();
  void on_inputFormatComboBox_currentIndexChanged(int index);
  void on_createPushButton_clicked();
  void on_outputFormatComboBox_currentTextChanged(const QString& arg1);
};

#endif  // SAKTOOLSTRINGASSISTANT_H
