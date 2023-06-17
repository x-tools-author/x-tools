/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUIINPUTMENU_HH
#define SAKTOOLBOXUIINPUTMENU_HH

#include <QMenu>

namespace Ui {
class SAKToolBoxUiInputMenu;
}

class SAKToolBoxUiInputMenu : public QMenu
{
    Q_OBJECT
public:
    explicit SAKToolBoxUiInputMenu(const QString &settingsGroup,
                                   QWidget *parent = nullptr);
    ~SAKToolBoxUiInputMenu();

    struct Parameters {
        int prefix;
        int suffix;
        int escapeCharacter;

        bool appendCrc;
        bool bigEndian;
        int algorithm;
        int startIndex;
        int endIndex;

        QString algorithmName;
    };

    Parameters parameters();

signals:
    void parametersChanged();

private:
    Parameters mParameters;

private:
    Ui::SAKToolBoxUiInputMenu *ui;
};

#endif // SAKTOOLBOXUIINPUTMENU_HH
