/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKRESPONSERTOOLUI_H
#define SAKRESPONSERTOOLUI_H

#include <QWidget>

#include "sakresponsertooluieditor.h"
#include "saktablemodeltoolui.h"

class SAKResponserToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    explicit SAKResponserToolUi(QWidget *parent = nullptr);
    ~SAKResponserToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns() override;
    virtual QDialog *itemEditor() override;

private:
    SAKResponserToolUiEditor *mEditor{nullptr};
};

#endif // SAKRESPONSERTOOL_HH
