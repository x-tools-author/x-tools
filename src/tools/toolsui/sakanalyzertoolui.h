/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKANALYZERTOOLUI_HH
#define SAKANALYZERTOOLUI_HH

#include "SAKBaseToolUi.h"

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
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingsGroup) final;

private:
    Ui::SAKAnalyzerToolUi *ui;
};

#endif // SAKANALYZERTOOLUI_HH
