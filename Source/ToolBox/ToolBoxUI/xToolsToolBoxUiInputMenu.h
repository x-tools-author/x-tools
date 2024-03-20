/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>

namespace Ui {
class xToolsToolBoxUiInputMenu;
}

class xToolsToolBoxUiInputMenu : public QMenu
{
    Q_OBJECT
public:
    explicit xToolsToolBoxUiInputMenu(const QString &settingsGroup, QWidget *parent = nullptr);
    ~xToolsToolBoxUiInputMenu();

    struct Parameters
    {
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
    Ui::xToolsToolBoxUiInputMenu *ui;
};
