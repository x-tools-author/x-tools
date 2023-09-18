/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSTATISTICIANTOOLUI_HH
#define SAKSTATISTICIANTOOLUI_HH

#include "SAKBaseToolUi.h"

namespace Ui {
class SAKStatisticianToolUi;
}

class SAKStatisticianToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    SAKStatisticianToolUi(QWidget *parent = nullptr);
    ~SAKStatisticianToolUi();

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingsGroup) final;

private:
    Ui::SAKStatisticianToolUi *ui;
};

#endif // SAKSTATISTICIANTOOLUI_HH
