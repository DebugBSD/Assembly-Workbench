/*
 * BSD 3-Clause License
 * 
 * Copyright (c) 2020, DebugBSD
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "stdafx.h"
#include "wx/wxprec.h"
#include "wx/imaglist.h"
#include "NewFileDlg.h"

/*
 * NewFileDlg event table definition
 */

BEGIN_EVENT_TABLE(NewFileDlg, wxDialog)

////@begin NewProjectDlg event table entries
EVT_BUTTON(ID_CREATE, NewFileDlg::OnCreate)
EVT_BUTTON(ID_SELECT_FILE_DIR_BTN, NewFileDlg::OnSelectDirectory)
////@end NewProjectDlg event table entries

END_EVENT_TABLE()


/*
    * NewFileDlg constructors
    */

NewFileDlg::NewFileDlg()
{
    Init();
}

NewFileDlg::NewFileDlg(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * NewFileDlg creator
 */

bool NewFileDlg::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    ////@begin NewFileDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    SetMinSize(size);
    Centre();
    ////@end NewFileDlg creation
    return true;
}


/*
 * NewFileDlg destructor
 */

NewFileDlg::~NewFileDlg()
{
    ////@begin NewFileDlg destruction
    ////@end NewFileDlg destruction
}


/*
 * Member initialisation
 */

void NewFileDlg::Init()
{
    ////@begin NewFileDlg member initialisation
    ////@end NewFileDlg member initialisation
}


/*
 * Control creation for NewFileDlg
 */

void NewFileDlg::CreateControls()
{

    NewFileDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW | wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW | wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText(itemDialog1, wxID_STATIC, _("Create a new empty file or a file based on a template."), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer4->Add(itemStaticText5, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxSplitterWindow* itemSplitterWindow6 = new wxSplitterWindow(itemDialog1, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER | wxSP_3DSASH | wxNO_BORDER);
    itemSplitterWindow6->SetMinimumPaneSize(0);

    wxTreeCtrl* itemTreeCtrl7 = new wxTreeCtrl(itemSplitterWindow6, ID_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_SINGLE);

    wxArrayString itemListBox8Strings;
    wxListBox* itemListBox8 = new wxListBox(itemSplitterWindow6, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, itemListBox8Strings, wxLB_SINGLE);

    itemSplitterWindow6->SplitVertically(itemTreeCtrl7, itemListBox8, 50);
    itemBoxSizer3->Add(itemSplitterWindow6, 1, wxGROW | wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer9, 0, wxGROW | wxALL, 2);

    wxStaticText* itemStaticText10 = new wxStaticText(itemDialog1, wxID_STATIC, _("File Name:"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_pFileName = new wxTextCtrl(itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer9->Add(m_pFileName, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer12, 0, wxGROW | wxALL, 2);

    wxStaticText* itemStaticText13 = new wxStaticText(itemDialog1, wxID_STATIC, _("Directory:"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_pFileLocation = new wxTextCtrl(itemDialog1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer12->Add(m_pFileLocation, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* itemButton15 = new wxButton(itemDialog1, ID_SELECT_FILE_DIR_BTN, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    itemBoxSizer12->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer16, 0, wxGROW | wxALL, 2);

    itemBoxSizer16->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* itemButton18 = new wxButton(itemDialog1, ID_CREATE, _("Create"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer16->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* itemButton19 = new wxButton(itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer16->Add(itemButton19, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    ////@end NewFileDlg content construction
}


/*
 * Should we show tooltips?
 */

bool NewFileDlg::ShowToolTips()
{
    return true;
}

void NewFileDlg::OnCreate(wxCommandEvent& event)
{
    int retCode = 0;
    int stop = 1;

    // First we need to make some checks.
    if (m_pFileName->IsEmpty() || m_pFileLocation->IsEmpty()) return;

    EndModal(retCode); // 0 not error
}

void NewFileDlg::OnSelectDirectory(wxCommandEvent& event)
{
    wxDirDialog dlg(NULL, "Choose base directory", "",
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    int res = dlg.ShowModal();

    if (res == wxID_OK)
    {
        wxString selectedDirectory = dlg.GetPath();
        m_pFileLocation->SetValue(selectedDirectory);
    }
    else
    {

    }
}

/*
 * Get bitmap resources
 */

wxBitmap NewFileDlg::GetBitmapResource(const wxString& name)
{
    // Bitmap retrieval
////@begin NewFileDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
    ////@end NewFileDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon NewFileDlg::GetIconResource(const wxString& name)
{
    // Icon retrieval
////@begin NewFileDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
    ////@end NewFileDlg icon retrieval
}

