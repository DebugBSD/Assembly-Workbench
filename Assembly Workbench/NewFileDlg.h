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
#pragma once

/*!
 * Includes
 */

 ////@begin includes
#include <wx/dialog.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
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
#define ID_NEWFILEDLG 10000
#define ID_SPLITTERWINDOW 10001
#define ID_TREECTRL 10002
#define ID_LISTBOX 10003
#define ID_TEXTCTRL 10004
#define ID_TEXTCTRL1 10005
#define ID_SELECT_FILE_DIR_BTN 10008
#define ID_CREATE 10006
#define SYMBOL_NEWFILEDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_NEWFILEDLG_TITLE _("New File")
#define SYMBOL_NEWFILEDLG_IDNAME ID_NEWFILEDLG
#define SYMBOL_NEWFILEDLG_SIZE wxSize(640, 480)
#define SYMBOL_NEWFILEDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * NewFileDlg class declaration
 */

class NewFileDlg : public wxDialog
{
public:
    /// Constructors
    NewFileDlg();
    NewFileDlg(wxWindow* parent, wxWindowID id = SYMBOL_NEWFILEDLG_IDNAME, const wxString& caption = SYMBOL_NEWFILEDLG_TITLE, const wxPoint& pos = SYMBOL_NEWFILEDLG_POSITION, const wxSize& size = SYMBOL_NEWFILEDLG_SIZE, long style = SYMBOL_NEWFILEDLG_STYLE);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_NEWFILEDLG_IDNAME, const wxString& caption = SYMBOL_NEWFILEDLG_TITLE, const wxPoint& pos = SYMBOL_NEWFILEDLG_POSITION, const wxSize& size = SYMBOL_NEWFILEDLG_SIZE, long style = SYMBOL_NEWFILEDLG_STYLE);

    /// Destructor
    ~NewFileDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    ////@begin NewFileDlg event handler declarations
    ////@end NewFileDlg event handler declarations

    ////@begin NewFileDlg member function declarations
    /// Sets the text field for file location
    void SetFileLocation(const wxString& fileLocation) { if (m_pFileLocation) m_pFileLocation->SetValue(fileLocation); }
    void GetFileName(wxString& path, wxString& fileName) { path = m_pFileLocation->GetValue();  fileName = m_pFileName->GetValue(); }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);

    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
    ////@end NewFileDlg member function declarations

        /// Should we show tooltips?
    static bool ShowToolTips();

    ////@begin NewFileDlg member variables
    ////@end NewFileDlg member variables


private:
    class wxTextCtrl* m_pFileLocation;
    class wxTextCtrl* m_pFileName;
private:

    void OnCreate(wxCommandEvent& event);
    void OnSelectDirectory(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
