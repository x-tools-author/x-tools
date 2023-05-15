/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSTORERTOOLUI_HH
#define SAKSTORERTOOLUI_HH

#include <QWidget>

namespace Ui {
class SAKStorerToolUi;
}

class SAKStorerTool;
class SAKStorerToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKStorerToolUi(QWidget *parent = nullptr);
    ~SAKStorerToolUi();
    void setupStorer(SAKStorerTool *tool);

private:
    Ui::SAKStorerToolUi *ui{nullptr};
};

#endif // SAKSTORERTOOLUI_HH
