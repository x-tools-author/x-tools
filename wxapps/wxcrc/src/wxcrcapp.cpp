/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wxcrcapp.h"
#include "wxcrcframe.h"

#if defined(_MSC_VER)
#include <dwmapi.h>
#include <winuser.h>
#include <wx/msw/private.h>
#endif

bool wxCrcApp::OnInit()
{
    wxA::OnInit();

    auto* ui = new wxCrcFrame();
#if defined(WXT_MAGIC)
    ui->SetTransparent(50);
#endif
    ui->Show(true);
    ui->moveTolCenter();
    ui->SetMinSize(ui->GetSize());
    ui->SetMaxSize(ui->GetSize());

#if 1
    wxColour color = GetTopWindow()->GetBackgroundColour();
    COLORREF colorref = RGB(color.Red(), color.Green(), color.Blue());
    auto hwnd = GetTopWindow() ? GetTopWindow()->GetHWND() : nullptr;
    DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
#endif

    // Hide minimize and maximize buttons
    ui->SetWindowStyle(ui->GetWindowStyle() & ~(wxMAXIMIZE_BOX | wxMINIMIZE_BOX));

    return true;
}

wxCrcApp::~wxCrcApp() {}