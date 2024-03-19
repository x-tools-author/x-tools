/***************************************************************************************************
 * Copyright 2020-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class xToolsStringAssistant;
}
class xToolsStringAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsStringAssistant(QWidget* parent = Q_NULLPTR);
    ~xToolsStringAssistant();

private:
    Ui::xToolsStringAssistant* ui;

private slots:
    void onTextEditTextChanged();
    void onInputFormatComboBoxCurrentIndexChanged(int index);
    void onCreatePushButtonClicked();
    void onOutputFormatComboBoxCurrentTextChanged(const QString& text);
};
