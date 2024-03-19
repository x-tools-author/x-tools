/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKANALYZERTOOLUI_H
#define SAKANALYZERTOOLUI_H

#include "sakbasetoolui.h"

namespace Ui {
class SAKAnalyzerToolUi;
}

class SAKAnalyzerTool;
class SAKAnalyzerToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    explicit SAKAnalyzerToolUi(QWidget *parent = nullptr);
    void setToolName(const QString &name);
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup) final;

private:
    Ui::SAKAnalyzerToolUi *ui;
};

#endif // SAKANALYZERTOOLUI_H
