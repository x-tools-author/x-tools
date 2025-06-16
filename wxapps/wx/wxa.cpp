/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wxa.h"

#include <chrono>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>

#if defined(_MSC_VER)
#include <dwmapi.h>
#include <winuser.h>
#include <wx/msw/private.h>
#endif

wxString GetSettingsPath()
{
#if defined(X_PORTABLE_EDITION) && defined(WIN32)
    wxString path = wxGetCwd() + wxFileName::GetPathSeparator() + wxString("conf");
#else
    wxString path = wxStandardPaths::Get().GetUserDataDir();
#endif
    // Make full dir...
    if (!wxDirExists(path)) {
#if defined(WIN32)
        wxMkDir(path);
#else
        wxMkDir(path, 777);
#endif
    }

    return path;
}

wxString GetSettingsFileName()
{
    return GetSettingsPath() + wxFileName::GetPathSeparator() + _("wxTools.json");
}

void FailureWriter(const char *data, size_t size)
{
    // Remove the settings file
    wxString settingsFile = GetSettingsFileName();
    if (wxFileExists(settingsFile)) {
        wxRemoveFile(settingsFile);
    }
}

std::string LogPath()
{
    wxString path = GetSettingsPath();
    path += wxFileName::GetPathSeparator();
    path += wxString("log");

#if defined(WIN32)
    wxMkDir(path);
#else
    wxMkDir(path, 777);
#endif

    return path.ToStdString();
}

void DoInitLogging(const char *argv0)
{
    const std::chrono::minutes keep{7 * 24 * 60};

    google::SetLogFilenameExtension(".log");
    google::EnableLogCleaner(keep);
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(FailureWriter);

    fLB::FLAGS_logtostdout = false;
    fLB::FLAGS_logtostderr = false;
    fLS::FLAGS_log_dir = LogPath();
    fLI::FLAGS_logbufsecs = 0;
    fLU::FLAGS_max_log_size = 10;
    fLB::FLAGS_stop_logging_if_full_disk = true;
    fLB::FLAGS_colorlogtostderr = true;
#if 0
    fLB::FLAGS_alsologtostderr = true;
#endif
    google::InitGoogleLogging(argv0);
}

void DoShutdownLogging()
{
    google::ShutdownGoogleLogging();
}

const int xNewWxId()
{
    static int wxtId = wxID_HIGHEST + 10000;
    return wxtId++;
}

bool wxA::OnInit()
{
#if wxCHECK_VERSION(3, 3, 0)
    SetAppearance(Appearance::Dark);
#endif

    wxString i18nDir = wxGetCwd();
    i18nDir += wxFileName::GetPathSeparator();
    i18nDir += wxString("i18n");

    m_locale.AddCatalogLookupPathPrefix(i18nDir);
    m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT);
    m_locale.AddCatalog(wxString("wxTools"));
    m_locale.AddCatalog(wxString("wxWidgets"));
    xInfo() << "Application booting...";
    xInfo() << "--------------------------------------------------------------------------------";
    xInfo() << "Settings file is:" << GetSettingsFileName();
    return true;
}

wxA::~wxA() {}

void wxA::installLog() {}

void wxA::uninstallLog() {}