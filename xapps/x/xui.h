/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMainWindow>
#include <QMenu>

class xUi : public QMainWindow
{
    Q_OBJECT
public:
    explicit xUi(QWidget *parent = nullptr);
    ~xUi() override;

    void moveToCenter();

public:
    struct SettingKeys
    {
        const QString language{"Application/language"};
    };

protected:
    QMenu *m_optionMenu{nullptr};
    QMenu *m_viewMenu{nullptr};
    QMenu *m_helpMenu{nullptr};

private:
    void initMenuBar();
    void initMenuBarOption();
    void initMenuBarOptionLanguage();
    void initMenuBarOptionSetting();
    void initMenuBarView();
    void initMenuBarViewHdpi();
    void initMenuBarViewTheme();
    void initializeMenuBarViewStaysOnTop();
    void initMenuBarHelp();

    void showAboutInfo();
};