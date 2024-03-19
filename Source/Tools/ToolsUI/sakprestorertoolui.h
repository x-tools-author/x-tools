/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "saktablemodeltoolui.h"
#include <QMenu>

class xToolsPrestorerTool;
class SAKPrestorerToolUiEditor;
class SAKPrestorerToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    explicit SAKPrestorerToolUi(QWidget *parent = nullptr);
    ~SAKPrestorerToolUi();
    QMenu *menu();

protected:
    virtual void onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns() override;
    virtual void afterRowEdited(int row) override;
    virtual QDialog *itemEditor() override;

private:
    SAKPrestorerToolUiEditor *mEditor{nullptr};
    QMenu *mMenu{nullptr};

private:
    void updateMenu();
};
