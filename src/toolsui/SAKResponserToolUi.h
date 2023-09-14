/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKRESPONSERTOOLUI_HH
#define SAKRESPONSERTOOLUI_HH

#include <QWidget>

#include "SAKTableModelToolUi.hh"
#include "SAKResponserToolUiEditor.hh"

class SAKResponserToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    explicit SAKResponserToolUi(QWidget *parent = nullptr);
    ~SAKResponserToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns() override;
    virtual QDialog *itemEditor() override;

private:
    SAKResponserToolUiEditor *mEditor{nullptr};
};

#endif // SAKRESPONSERTOOL_HH
