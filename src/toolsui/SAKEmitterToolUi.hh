/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKEMITTERTOOLUI_HH
#define SAKEMITTERTOOLUI_HH

#include <QWidget>

namespace Ui {
class SAKEmitterToolUi;
}

class SAKEmitterTool;
class SAKEmitterToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKEmitterToolUi(QWidget *parent = nullptr);
    ~SAKEmitterToolUi();
    void setupEmitterTool(SAKEmitterTool *tool);

private:
    SAKEmitterTool *mTool{nullptr};

private:
    Ui::SAKEmitterToolUi *ui;
};

#endif // SAKEMITTERTOOLUI_HH
