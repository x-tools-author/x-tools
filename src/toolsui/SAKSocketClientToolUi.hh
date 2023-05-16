/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTOOLUI_HH
#define SAKSOCKETCLIENTTOOLUI_HH

#include "SAKCommunicationToolUi.hh"

namespace Ui {
class SAKSocketClientToolUi;
}
class SAKSocketClientToolUi : public SAKCommunicationToolUi
{
    Q_OBJECT
public:
    explicit SAKSocketClientToolUi(QWidget *parent = nullptr);
    ~SAKSocketClientToolUi();
    virtual void setupCommunicationTool(SAKCommunicationTool *tool) final;
    virtual void updateUiState(bool isWorking) final;

private:
    Ui::SAKSocketClientToolUi *ui{nullptr};
};

#endif // SAKSOCKETCLIENTTOOLUI_H
