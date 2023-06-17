/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUIOUTPUTMENU_HH
#define SAKTOOLBOXUIOUTPUTMENU_HH

#include <QMenu>
#include <QWidget>
#include <QTextDocument>

namespace Ui {
class SAKToolBoxUiOutputMenu;
}

class SAKToolBoxUiOutputMenu : public QMenu
{
    Q_OBJECT

public:
    explicit SAKToolBoxUiOutputMenu(const QString &settingsGroup,
                                    QTextDocument *doc,
                                    QWidget *parent = nullptr);
    ~SAKToolBoxUiOutputMenu();

private:
    Ui::SAKToolBoxUiOutputMenu *ui;
};

#endif // SAKTOOLBOXUIOUTPUTMENU_HH
