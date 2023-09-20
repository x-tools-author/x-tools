/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKVELOMETERTOOLUI_H
#define SAKVELOMETERTOOLUI_H

#include <QLoggingCategory>
#include "sakbasetoolui.h"

namespace Ui {
class SAKVelometerToolUi;
}

class SAKVelometerToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    SAKVelometerToolUi(QWidget *parent = nullptr);
    ~SAKVelometerToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingsGroup) final;

private:
    const QLoggingCategory mLoggingCategory{"sak.velometertoolui"};
    Ui::SAKVelometerToolUi *ui;
};

#endif // SAKVELOMETERTOOLUI_H
