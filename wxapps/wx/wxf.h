/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/menu.h>
#include <wx/wx.h>

class wxF : public wxFrame
{
public:
    wxF();
    ~wxF() override;

    void moveTolCenter();

protected:
    wxMenuBar* m_menuBar{nullptr};
    wxMenu* m_fileMenu{nullptr};
    wxMenu* m_optionMenu{nullptr};
    wxMenu* m_viewMenu{nullptr};
    wxMenu* m_helpMenu{nullptr};

private:
    // Option menu items
    int xID_SettingsDir{wxID_ANY};
    int xID_ClearSettings{wxID_ANY};
    int xID_OnTop{wxID_ANY};
    // View menu items
    int xID_Language{wxID_ANY};
    int xID_Hdpi{wxID_ANY};
    int xID_Theme{wxID_ANY};
    // Help menu items
    int xID_AboutApp{wxID_ANY};
    int xID_Gitee{wxID_ANY};
    int xID_Github{wxID_ANY};
    int xID_OnLine{wxID_ANY};
    int xID_Author{wxID_ANY};
    int xID_AppStore{wxID_ANY};

private:
    void DoInit();
    void DoInitMenuBar();
    void DoInitMenuBarFile();
    void DoInitMenuBarOption();
    void DoInitMenuBarView();
    void DoInitMenuBarHelp();

    void OnOptionItemEvent(wxCommandEvent& event);
    void OnViewItemEvent(wxCommandEvent& event);
    void OnHelpItemEvent(wxCommandEvent& event);
};