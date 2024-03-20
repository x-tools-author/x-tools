/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseToolUi.h"

namespace Ui {
class xToolsCrcCalculatorToolUi;
}

class xToolsCrcCalculatorToolUi : public xToolsBaseToolUi
{
    Q_OBJECT
public:
    xToolsCrcCalculatorToolUi(QWidget *parent = nullptr);
    ~xToolsCrcCalculatorToolUi();

protected:
    void onIsWorkingChanged(bool isWorking) override;
    void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup) override;

private:
    Ui::xToolsCrcCalculatorToolUi *ui;
};
