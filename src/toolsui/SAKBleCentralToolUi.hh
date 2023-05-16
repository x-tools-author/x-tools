/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALTOOLUI_HH
#define SAKBLECENTRALTOOLUI_HH

#include "SAKCommunicationToolUi.hh"

namespace Ui {
class SAKBleCentralToolUi;
}
class SAKBleCentralToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKBleCentralToolUi(QWidget *parent = nullptr);
    ~SAKBleCentralToolUi();
    virtual void setupCommunicationTool(SAKCommunicationTool *tool) final;
    virtual void updateUiState(bool isWorking) final;

private:
    Ui::SAKBleCentralToolUi *ui{nullptr};
};

#endif // SAKBLECENTRALTOOLUI_HH
