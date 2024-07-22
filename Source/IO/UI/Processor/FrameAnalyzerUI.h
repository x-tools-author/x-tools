/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseToolUi.h"

namespace Ui {
class FrameAnalyzer;
}

class xToolsAnalyzerTool;
class FrameAnalyzer : public xToolsBaseToolUi
{
    Q_OBJECT
public:
    explicit FrameAnalyzer(QWidget *parent = nullptr);
    void setToolName(const QString &name);
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup) final;

private:
    Ui::FrameAnalyzer *ui;
};
