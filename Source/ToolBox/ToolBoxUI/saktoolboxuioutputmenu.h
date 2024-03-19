/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTOOLBOXUIOUTPUTMENU_H
#define SAKTOOLBOXUIOUTPUTMENU_H

#include <QMenu>
#include <QTextDocument>
#include <QWidget>

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
    QString filter();

private:
    Ui::SAKToolBoxUiOutputMenu *ui;
};

#endif // SAKTOOLBOXUIOUTPUTMENU_H
