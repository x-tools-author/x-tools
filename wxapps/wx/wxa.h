/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <string>

#include <glog/logging.h>
#include <wx/wx.h>

#define xLog(severity) LOG(severity)
#define xInfo() LOG(INFO)
#define xWarning() LOG(WARNING)
#define xError() LOG(ERROR)
std::string LogPath();
void DoInitLogging(const char *argv0);
void DoShutdownLogging();
const int xNewWxId();

class wxA : public wxApp
{
public:
    bool OnInit() override;
    ~wxA() override;

    void installLog();
    void uninstallLog();

private:
    wxLocale m_locale;
};