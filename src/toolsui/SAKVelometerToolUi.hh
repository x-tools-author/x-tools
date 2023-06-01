/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKVELOMETERTOOLUI_HH
#define SAKVELOMETERTOOLUI_HH

#include <QLoggingCategory>
#include "SAKBaseToolUi.hh"

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
    const QLoggingCategory mLoggingCategory{"SAK.VelometerToolUi"};
    Ui::SAKVelometerToolUi *ui;
};

#endif // SAKVELOMETERTOOLUI_HH
