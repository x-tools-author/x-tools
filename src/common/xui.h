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

protected:
    QMenu *m_fileMenu{nullptr};
    QMenu *m_optionMenu{nullptr};
    QMenu *m_languageMenu{nullptr};
    QMenu *m_helpMenu{nullptr};

private:
    void initMenuBar();
    void initMenuBarFile();
    void initMenuBarOption();
    void initMenuBarOptionHdpi();
    void initMenuBarOptionStyle();
    void initMenuBarOptionTheme();
    void initMenuBarOptionSetting();
    void initMenuBarOptionStaysOnTop();
    void initMenuBarLanguage();
    void initMenuBarHelp();
    void initMenuBarHelpGrab();

    void showAboutInfo();
};
