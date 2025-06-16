/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wxf.h"
#include "wxa.h"

#include <fstream>
#include <wx/artprov.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

wxF::wxF()
    : wxFrame(nullptr, wxID_ANY, "wxTools")
    , xID_AboutApp(xNewWxId())
    , xID_OnLine(xNewWxId())
    , xID_Gitee(xNewWxId())
    , xID_Github(xNewWxId())
    , xID_Author(xNewWxId())
    , xID_AppStore(xNewWxId())
{
#if !defined(W_RELEASE)
#if 0
    [Ubuntu]
    I20250219 13:13:10.775637 139801817956736 MainWindow.cpp:29] GetExecutablePath():/home/user/Project/wx-tools/build/Desktop_Qt_6_8_2-Debug/assets/wxTools/wxTools
    I20250219 13:13:10.775692 139801817956736 MainWindow.cpp:30] GetConfigDir():/etc
    I20250219 13:13:10.775699 139801817956736 MainWindow.cpp:31] GetUserConfigDir():/home/user
    I20250219 13:13:10.775705 139801817956736 MainWindow.cpp:32] GetDataDir():/usr/local/share/wxTools
    I20250219 13:13:10.775712 139801817956736 MainWindow.cpp:33] GetUserDataDir():/home/user/.wxTools
    I20250219 13:13:10.775716 139801817956736 MainWindow.cpp:34] GetUserLocalDataDir():/home/user/.wxTools
    I20250219 13:13:10.775719 139801817956736 MainWindow.cpp:35] GetPluginsDir():/usr/local/lib/wxTools
    I20250219 13:13:10.775722 139801817956736 MainWindow.cpp:36] MakeConfigFileName():wxTools.conf
    I20250219 13:13:10.775734 139801817956736 MainWindow.cpp:37] GetGlobalFileName()/etc/wxTools.conf
    I20250219 13:13:10.775746 139801817956736 MainWindow.cpp:38] GetLocalFileName()/home/user/.wxTools
    I20250219 13:13:10.775746 139801817956736 MainWindow.cpp:38] wxGetCwd():/home/user/Project/wx-tools/build/Desktop_Qt_6_8_2-Debug/assets/wxTools
#endif
    xInfo() << "GetExecutablePath():" << wxStandardPaths::Get().GetExecutablePath();
    xInfo() << "GetConfigDir():" << wxStandardPaths::Get().GetConfigDir();
    xInfo() << "GetUserConfigDir():" << wxStandardPaths::Get().GetUserConfigDir();
    xInfo() << "GetDataDir():" << wxStandardPaths::Get().GetDataDir();
    xInfo() << "GetUserDataDir():" << wxStandardPaths::Get().GetUserDataDir();
    xInfo() << "GetUserLocalDataDir():" << wxStandardPaths::Get().GetUserLocalDataDir();
    xInfo() << "GetPluginsDir():" << wxStandardPaths::Get().GetPluginsDir();
    xInfo() << "MakeConfigFileName():" << wxStandardPaths::Get().MakeConfigFileName("wxTools");
    xInfo() << "GetGlobalFileName():" << wxFileConfig::GetGlobalFileName("wxTools");
    xInfo() << "GetLocalFileName():" << wxFileConfig::GetLocalFileName("wxTools");
    xInfo() << "wxGetCwd():" << wxGetCwd();
#endif

    DoInit();
}

wxF::~wxF() {}

void wxF::moveTolCenter()
{
    wxSize size = GetSize();
    wxSize screenSize = wxGetDisplaySize();

    int x = (screenSize.x - size.x) / 2;
    int y = (screenSize.y - size.y) / 2;

    SetPosition(wxPoint(x, y));
}

void wxF::DoInit()
{
    DoInitMenuBar();
}

void wxF::DoInitMenuBar()
{
    m_menuBar = new wxMenuBar();
    SetMenuBar(m_menuBar);

    DoInitMenuBarFile();
    DoInitMenuBarOption();
    DoInitMenuBarView();
    DoInitMenuBarHelp();
}

void wxF::DoInitMenuBarFile()
{
    m_fileMenu = new wxMenu();
    m_menuBar->Append(m_fileMenu, _("&File"));
    wxMenuItem* item = m_fileMenu->Append(wxID_EXIT, _("E&xit"));

    Bind(wxEVT_MENU, [=](wxCommandEvent& event) {
        if (event.GetId() == wxID_EXIT) {
            Close(true);
        }
    });
}

void wxF::DoInitMenuBarOption()
{
    m_optionMenu = new wxMenu();
    m_menuBar->Append(m_optionMenu, _("&Options"));

    m_optionMenu->Append(xID_SettingsDir, _("&Settings Directory"));
    m_optionMenu->Append(xID_ClearSettings, _("&Clear Settings"));
    m_optionMenu->AppendSeparator();
    m_optionMenu->Append(xID_OnTop, _("&Always on Top"));

    Bind(wxEVT_MENU, [=](wxCommandEvent& event) { OnOptionItemEvent(event); });
}

void wxF::DoInitMenuBarView()
{
    m_viewMenu = new wxMenu();
    m_menuBar->Append(m_viewMenu, _("&View"));

    m_viewMenu->Append(xID_Language, _("&Languages"));
    m_viewMenu->AppendSeparator();
    m_viewMenu->Append(xID_Hdpi, _("&HDPI Settings"));
    m_viewMenu->Append(xID_Theme, _("&Theme Settings"));

    Bind(wxEVT_MENU, [=](wxCommandEvent& event) { OnViewItemEvent(event); });
}

void wxF::DoInitMenuBarHelp()
{
    m_helpMenu = new wxMenu();
    m_menuBar->Append(m_helpMenu, _("&Help"));

    m_helpMenu->Append(wxID_ABOUT, _("&About wxWidgets"));
    m_helpMenu->Append(xID_AboutApp, _("&About xCrc"));
    m_helpMenu->AppendSeparator();
    m_helpMenu->Append(xID_Gitee, _("Get Source from Gitee"));
    m_helpMenu->Append(xID_Github, _("Get Source from Github"));
    m_helpMenu->Append(xID_OnLine, _("&Online Documents"));
    m_helpMenu->AppendSeparator();
    m_helpMenu->Append(xID_AppStore, _("&App Store"));
    m_helpMenu->Append(xID_Author, _("&Visit Author Home Page"));

    Bind(wxEVT_MENU, [=](wxCommandEvent& event) { OnHelpItemEvent(event); });
}

void wxF::OnViewItemEvent(wxCommandEvent& event)
{
    if (event.GetId() == xID_Hdpi) {
        // Handle HDPI option
    } else if (event.GetId() == xID_Theme) {
        // Handle Theme option
    } else if (event.GetId() == xID_OnTop) {
        // Handle On Top option
    }
}

void wxF::OnOptionItemEvent(wxCommandEvent& event)
{
    if (event.GetId() == xID_Language) {
        // Handle Language Settings
    } else if (event.GetId() == xID_SettingsDir) {
        wxString settingsDir = wxStandardPaths::Get().GetUserConfigDir();
        wxMessageBox(_("Settings Directory: ") + settingsDir,
                     _("Settings Directory"),
                     wxOK | wxICON_INFORMATION,
                     this);
    } else if (event.GetId() == xID_ClearSettings) {
        // Handle Clear Settings
        wxMessageBox(_("Clear Settings functionality is not implemented yet."),
                     _("Clear Settings"),
                     wxOK | wxICON_INFORMATION,
                     this);
    }
}

void wxF::OnHelpItemEvent(wxCommandEvent& event)
{
    if (event.GetId() == wxID_ABOUT) {
        wxMessageBox(_("This is a wxWidgets application."),
                     _("About wxTools"),
                     wxOK | wxICON_INFORMATION,
                     this);
    }
    if (event.GetId() == xID_AboutApp) {
        wxMessageBox(_("This is a wxWidgets application."),
                     _("About wxTools"),
                     wxOK | wxICON_INFORMATION,
                     this);
    } else if (event.GetId() == xID_Gitee) {
        wxLaunchDefaultBrowser("https://gitee.com/x-tools/x-tools");
    } else if (event.GetId() == xID_Github) {
        wxLaunchDefaultBrowser("https://github.com/x-tools/x-tools");
    } else if (event.GetId() == xID_OnLine) {
        wxLaunchDefaultBrowser("https://x-tools.github.io/x-tools/");
    } else if (event.GetId() == xID_Author) {
        wxLaunchDefaultBrowser("https://x-tools.github.io/");
    } else if (event.GetId() == xID_AppStore) {
        wxLaunchDefaultBrowser("https://apps.apple.com/app/id6443700000");
    }
}
