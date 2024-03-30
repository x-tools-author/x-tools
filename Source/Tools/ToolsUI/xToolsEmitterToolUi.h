/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsEmitterToolUiEditor.h"
#include "xToolsTableModelToolUi.h"

class xToolsEmitterToolUi : public xToolsTableModelToolUi
{
    Q_OBJECT
public:
    explicit xToolsEmitterToolUi(QWidget *parent = nullptr);
    ~xToolsEmitterToolUi();

protected:
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns() override;
    virtual QDialog *itemEditor() override;

private:
    xToolsEmitterToolUiEditor *m_editor{nullptr};
};
