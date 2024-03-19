/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKEMITTERTOOLUI_H
#define SAKEMITTERTOOLUI_H

#include "sakemittertooluieditor.h"
#include "saktablemodeltoolui.h"

class SAKEmitterToolUi : public SAKTableModelToolUi
{
    Q_OBJECT
public:
    explicit SAKEmitterToolUi(QWidget *parent = nullptr);
    ~SAKEmitterToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns() override;
    virtual QDialog *itemEditor() override;

private:
    SAKEmitterToolUiEditor *mEditor{nullptr};
};

#endif // SAKEMITTERTOOLUI_H
