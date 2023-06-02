/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKPRESTORERTOOLUI_HH
#define SAKPRESTORERTOOLUI_HH

#include "SAKTableModelToolUi.hh"

class SAKPrestorerTool;
class SAKPrestorerToolUiEditor;
class SAKPrestorerToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    explicit SAKPrestorerToolUi(QWidget *parent = nullptr);
    ~SAKPrestorerToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingGroup) override;
    virtual QDialog *itemEditor() override;

private:
    SAKPrestorerToolUiEditor *mEditor{nullptr};
};

#endif // SAKPRESTORERTOOLUI_HH
