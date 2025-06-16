/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>

#include "wx/wxf.h"

class wxCrcFrame : public wxF
{
public:
    wxCrcFrame();
    ~wxCrcFrame() override;

private:
    struct UiCtx
    {
        wxComboBox* dataFormat;
        wxComboBox* algorithm;
        wxTextCtrl* data;
        wxTextCtrl* result;
        wxCheckBox* upper;
        wxButton* calculate;
    } m_ui;
};