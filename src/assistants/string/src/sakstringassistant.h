/***************************************************************************************************
 * Copyright 2020-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSTRINGASSISTANT_H
#define SAKSTRINGASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKStringAssistant;
}
class SAKStringAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKStringAssistant(QWidget* parent = Q_NULLPTR);
    ~SAKStringAssistant();

private:
    Ui::SAKStringAssistant* ui_;

private slots:
    void OnTextEditTextChanged();
    void OnInputFormatComboBoxCurrentIndexChanged(int index);
    void OnCreatePushButtonClicked();
    void OnOutputFormatComboBoxCurrentTextChanged(const QString& text);
};

#endif // SAKSTRINGASSISTANT_H
