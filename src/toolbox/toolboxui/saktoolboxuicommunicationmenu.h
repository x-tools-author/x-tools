/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKTOOLBOXUICOMMUNICATIONMENU_H
#define SAKTOOLBOXUICOMMUNICATIONMENU_H

#include <QMenu>
#include <QTextDocument>
#include <QLoggingCategory>

namespace Ui {
class SAKToolBoxUiCommunicationMenu;
}

class SAKToolBox;
class SAKHighlighter;
class SAKToolBoxUiCommunicationMenu : public QMenu
{
    Q_OBJECT
public:
    SAKToolBoxUiCommunicationMenu(QWidget *parent = nullptr);
    ~SAKToolBoxUiCommunicationMenu();

    void initialize(SAKToolBox *toolBox, const QString &settingsGroup);

private:
    const QLoggingCategory mLoggingCategory{"SAK.CustomBoxUiParameters"};

private:
    Ui::SAKToolBoxUiCommunicationMenu *ui;
};

#endif // SAKTOOLBOXUICOMMUNICATIONMENU_H
