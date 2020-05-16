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

#ifndef _NEWPROJECTDLG_H_
#define _NEWPROJECTDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/splitter.h"
#include "wx/treectrl.h"
#include "wx/filename.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_NEWPROJECTDLG 10000
#define ID_SPLITTERWINDOW 10001
#define ID_TREECTRL 10002
#define ID_LISTBOX 10003
#define ID_TEXTCTRL 10004
#define ID_TEXTCTRL1 10005
#define ID_SELECT_PROJECT_DIR_BTN 10008
#define ID_CREATE 10006
#define SYMBOL_NEWPROJECTDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_NEWPROJECTDLG_TITLE _("New Project")
#define SYMBOL_NEWPROJECTDLG_IDNAME ID_NEWPROJECTDLG
#define SYMBOL_NEWPROJECTDLG_SIZE wxSize(640, 480)
#define SYMBOL_NEWPROJECTDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * NewProjectDlg class declaration
 */

class NewProjectDlg: public wxDialog
{    
public:
    /// Constructors
    NewProjectDlg();
    NewProjectDlg( wxWindow* parent, wxWindowID id = SYMBOL_NEWPROJECTDLG_IDNAME, const wxString& caption = SYMBOL_NEWPROJECTDLG_TITLE, const wxPoint& pos = SYMBOL_NEWPROJECTDLG_POSITION, const wxSize& size = SYMBOL_NEWPROJECTDLG_SIZE, long style = SYMBOL_NEWPROJECTDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_NEWPROJECTDLG_IDNAME, const wxString& caption = SYMBOL_NEWPROJECTDLG_TITLE, const wxPoint& pos = SYMBOL_NEWPROJECTDLG_POSITION, const wxSize& size = SYMBOL_NEWPROJECTDLG_SIZE, long style = SYMBOL_NEWPROJECTDLG_STYLE );

    /// Destructor
    ~NewProjectDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin NewProjectDlg event handler declarations
////@end NewProjectDlg event handler declarations

////@begin NewProjectDlg member function declarations
    /// Get Filename
    wxString GetFileName() const { return m_pProjectNameTextCtrl->GetValue()+".awp"; }
    wxString GetDirectory() const { return m_pProjectBaseDirectoryTextCtrl->GetValue(); }
    wxString GetProjectName() const { return m_pProjectNameTextCtrl->GetValue(); }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end NewProjectDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

private:
////@begin NewProjectDlg member variables
    class wxTextCtrl* m_pProjectNameTextCtrl;
    class wxTextCtrl* m_pProjectBaseDirectoryTextCtrl;
////@end NewProjectDlg member variables

private:

    void OnCreate(wxCommandEvent& event);
    void OnSelectDirectory(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

#endif
    // _NEWPROJECTDLG_H_
