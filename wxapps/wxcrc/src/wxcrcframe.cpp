/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "wxcrcframe.h"

#include <wx/gbsizer.h>
#include <wx/icon.h>
#include <wx/stattext.h>

wxCrcFrame::wxCrcFrame()
    : wxF()
{
    wxIcon icon;
    icon.LoadFile("IDI_ICON1", wxBITMAP_TYPE_ICO_RESOURCE);
    SetIcon(icon);
    SetTitle(_("wxCRC - CRC Calculator"));

    wxGridBagSizer* leftSizer = new wxGridBagSizer(4, 4);
    wxStaticText* label = new wxStaticText(this, wxID_ANY, _("Input Format"));
    wxComboBox* inputFormatCombo = new wxComboBox(this, wxID_ANY);
    inputFormatCombo->Append(_("Hexadecimal"));
    inputFormatCombo->Append(_("ASCII"));
    leftSizer->Add(label, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    leftSizer->Add(inputFormatCombo, wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    label = new wxStaticText(this, wxID_ANY, _("CRC Algorithm"));
    m_ui.algorithm = new wxComboBox(this, wxID_ANY);
    leftSizer->Add(label, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    leftSizer->Add(m_ui.algorithm, wxGBPosition(3, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    label = new wxStaticText(this, wxID_ANY, _("CRC Result"));
    m_ui.result = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    leftSizer->Add(label, wxGBPosition(4, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 0);
    leftSizer->Add(m_ui.result, wxGBPosition(5, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    m_ui.upper = new wxCheckBox(this, wxID_ANY, _("Upper Case Result"));
    m_ui.calculate = new wxButton(this, wxID_ANY, _("Calculate"));
    leftSizer->Add(m_ui.upper, wxGBPosition(6, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);
    leftSizer->Add(m_ui.calculate, wxGBPosition(7, 0), wxGBSpan(1, 1), wxEXPAND | wxALL, 0);

    wxGridBagSizer* sizer = new wxGridBagSizer(4, 4);
    sizer->Add(leftSizer, wxGBPosition(0, 0), wxGBSpan(2, 1), wxEXPAND | wxALL, 4);

    label = new wxStaticText(this, wxID_ANY, _("CRC Data"));
    m_ui.data = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    m_ui.data->SetMinSize(wxSize(360, 128));
    sizer->Add(label, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxALL, 4);
    sizer->Add(m_ui.data, wxGBPosition(1, 1), wxGBSpan(1, 1), wxEXPAND | wxALL, 4);

    SetSizerAndFit(sizer);
    leftSizer->AddGrowableCol(1, 1);
    sizer->AddGrowableCol(1, 1);
}

wxCrcFrame::~wxCrcFrame() {}