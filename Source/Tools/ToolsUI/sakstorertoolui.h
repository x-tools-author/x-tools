/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSTORERTOOLUI_H
#define SAKSTORERTOOLUI_H

#include "sakbasetoolui.h"

namespace Ui {
class SAKStorerToolUi;
}

class SAKStorerTool;
class SAKStorerToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    explicit SAKStorerToolUi(QWidget *parent = nullptr);
    ~SAKStorerToolUi();
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup) final;

private:
    Ui::SAKStorerToolUi *ui{nullptr};

    void onPushButtonSelectFileClicked();
};

#endif // SAKSTORERTOOLUI_H
