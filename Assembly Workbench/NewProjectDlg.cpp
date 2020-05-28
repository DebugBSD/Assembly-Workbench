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

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include <wx/stdpaths.h>
#include <wx/imaglist.h>
#include <wx/dirdlg.h>
#include <wx/combobox.h>
////@end includes

#include "NewProjectDlg.h"

////@begin XPM images

////@end XPM images


/*
 * NewProjectDlg type definition
 */


/*
 * NewProjectDlg event table definition
 */

BEGIN_EVENT_TABLE( NewProjectDlg, wxDialog )

////@begin NewProjectDlg event table entries
EVT_BUTTON(ID_CREATE, NewProjectDlg::OnCreate)
EVT_BUTTON(ID_SELECT_PROJECT_DIR_BTN, NewProjectDlg::OnSelectDirectory)
////@end NewProjectDlg event table entries

END_EVENT_TABLE()


/*
 * NewProjectDlg constructors
 */

NewProjectDlg::NewProjectDlg()
{
    Init();
}

NewProjectDlg::NewProjectDlg( wxWindow* parent, const wxArrayString& choices, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, choices, id, caption, pos, size, style);
}


/*
 * NewProjectDlg creator
 */

bool NewProjectDlg::Create( wxWindow* parent, const wxArrayString& choices, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin NewProjectDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls(choices);
    SetMinSize(size);
    Centre();
////@end NewProjectDlg creation
    return true;
}


/*
 * NewProjectDlg destructor
 */

NewProjectDlg::~NewProjectDlg()
{
////@begin NewProjectDlg destruction
////@end NewProjectDlg destruction
}


/*
 * Member initialisation
 */

void NewProjectDlg::Init()
{
////@begin NewProjectDlg member initialisation
////@end NewProjectDlg member initialisation
}


/*
 * Control creation for NewProjectDlg
 */

void NewProjectDlg::CreateControls(const wxArrayString& choices)
{    

    NewProjectDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer1, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Create a new project based on a template or custom project."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSplitterWindow* itemSplitterWindow5 = new wxSplitterWindow( itemDialog1, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow5->SetMinimumPaneSize(0);

    wxTreeCtrl* itemTreeCtrl6 = new wxTreeCtrl( itemSplitterWindow5, ID_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_SINGLE );

    wxArrayString itemListBox7Strings;
    wxListBox* itemListBox7 = new wxListBox( itemSplitterWindow5, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, itemListBox7Strings, wxLB_SINGLE );

    itemSplitterWindow5->SplitVertically(itemTreeCtrl6, itemListBox7, 50);
    itemBoxSizer1->Add(itemSplitterWindow5, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer4, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project Name:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_pProjectNameTextCtrl = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(m_pProjectNameTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer7, 0, wxGROW|wxALL, 2);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project Directory:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString currentChoice = choices[choices.GetCount() - 1];
    m_pProjectBaseDirectoryComboBoxCtrl = new wxComboBox( itemDialog1, ID_TEXTCTRL1, currentChoice, wxDefaultPosition, wxDefaultSize, choices);
    itemBoxSizer7->Add(m_pProjectBaseDirectoryComboBoxCtrl, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton1 = new wxButton( itemDialog1, ID_SELECT_PROJECT_DIR_BTN, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
    itemBoxSizer7->Add(itemButton1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer10, 0, wxGROW|wxALL, 2);

    itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton2 = new wxButton( itemDialog1, ID_CREATE, _("Create"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemButton2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton3 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemButton3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end NewProjectDlg content construction
}


/*
 * Should we show tooltips?
 */

bool NewProjectDlg::ShowToolTips()
{
    return true;
}

void NewProjectDlg::OnCreate(wxCommandEvent& event)
{
    int retCode = 0;

    /* 
     * Look at Detailed Description of wxComboBox.  
     * Documentation says that this control doesn't behave well with strings 
     * which differs in case only (like in MSW) so portable programs should 
     * avoid adding such strings to this control.
     * 
     * Look at wxComboCtrl for other posibilities.
     */

    // First, lest make some checks before creating the project and the contents.
    if (m_pProjectBaseDirectoryComboBoxCtrl->GetValue().IsEmpty() ||
        m_pProjectNameTextCtrl->IsEmpty())
    {
        retCode = -1;
        return; // Do nothing
    }


    
    if (!wxDirExists(m_pProjectBaseDirectoryComboBoxCtrl->GetValue()))
    {
        // Here, we could show a message telling the user about an error with the directory.
        retCode = -2;
        return; // Do nothing
    }

    if (wxDirExists(m_pProjectBaseDirectoryComboBoxCtrl->GetValue() + wxFileName::GetPathSeparator() + m_pProjectNameTextCtrl->GetValue()))
    {
        // Directory and Project directory exists, so we don't do anything. We should show a message to handle it.

        // Maybe we could test if directory is empty and create the project inside the directory.
        retCode = -3;
        return;
    }

    EndModal(retCode); // 0 not error
}

void NewProjectDlg::OnSelectDirectory(wxCommandEvent& event)
{
    wxDirDialog dlg(NULL, "Choose base directory", "",
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    int res = dlg.ShowModal();

    if (res == wxID_OK)
    {
        wxString selectedDirectory = dlg.GetPath();
        m_pProjectBaseDirectoryComboBoxCtrl->SetValue(selectedDirectory);
    }
    else
    {

    }
}

/*
 * Get bitmap resources
 */

wxBitmap NewProjectDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin NewProjectDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end NewProjectDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon NewProjectDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin NewProjectDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end NewProjectDlg icon retrieval
}
