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
#ifndef _FILESETTINGS_H_
#define _FILESETTINGS_H_


/*!
 * Includes
 */

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/artprov.h>

#include "FileSettings.h"

/*!
 * Forward declarations
 */

class wxSplitterWindow;

/*!
 * Control identifiers
 */
#define ID_FILESETTINGS 10000
#define ID_COMBOBOX 10006
#define ID_COMBOBOX1 10009
#define ID_SPLITTERWINDOW 10001
#define ID_TREECTRL 10002
#define ID_PANEL 10003
#define ID_TEXTCTRL 10004
#define ID_TEXTCTRL1 10005
#define ID_ADD_NEW 10006
#define SYMBOL_FILESETTINGS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_FILESETTINGS_TITLE _("File Settings")
#define SYMBOL_FILESETTINGS_IDNAME ID_FILESETTINGS
#define SYMBOL_FILESETTINGS_SIZE wxSize(640, 480)
#define SYMBOL_FILESETTINGS_POSITION wxDefaultPosition


/*!
 * FileSettings class declaration
 */

class FileSettingsDlg : public wxDialog
{
public:
    /// Constructors
    FileSettingsDlg();
    FileSettingsDlg(wxWindow* parent, wxWindowID id = SYMBOL_FILESETTINGS_IDNAME, const wxString& caption = SYMBOL_FILESETTINGS_TITLE, const wxPoint& pos = SYMBOL_FILESETTINGS_POSITION, const wxSize& size = SYMBOL_FILESETTINGS_SIZE, long style = SYMBOL_FILESETTINGS_STYLE, class FileSettings* pFileSettings = nullptr);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_FILESETTINGS_IDNAME, const wxString& caption = SYMBOL_FILESETTINGS_TITLE, const wxPoint& pos = SYMBOL_FILESETTINGS_POSITION, const wxSize& size = SYMBOL_FILESETTINGS_SIZE, long style = SYMBOL_FILESETTINGS_STYLE, class FileSettings *pFileSettings = nullptr);

    /// Destructor
    ~FileSettingsDlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);

    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);

    /// Should we show tooltips?
    static bool ShowToolTips();
private:
    class wxTreeCtrl* SetTree();

private:
    wxComboBox* m_pProjectConfigCtrl;
    wxComboBox* m_pPlatformConfCtrl;
    wxSplitterWindow* m_pSplitter;
    wxPropertyGrid* m_pPropGridCtrl;
    wxTreeCtrl* m_pTreeCtrl;
    class FileSettings* m_pFileSettings;
    std::unordered_map<void*, FileSettings::EProperty> m_Settings;

    void OnBeginSelection(wxTreeEvent& event);
    void OnEndSelection(wxTreeEvent& event);
    wxDECLARE_EVENT_TABLE();
};

#endif
// _FILESETTINGS_H_
