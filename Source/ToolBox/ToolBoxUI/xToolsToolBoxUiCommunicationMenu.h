/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTOOLBOXUICOMMUNICATIONMENU_H
#define SAKTOOLBOXUICOMMUNICATIONMENU_H

#include <QLoggingCategory>
#include <QMenu>
#include <QTextDocument>

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
