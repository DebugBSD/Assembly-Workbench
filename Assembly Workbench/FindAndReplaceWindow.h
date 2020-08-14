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

#include <unordered_map>

#include <wx/panel.h>
#include <wx/dataview.h>
#include <wx/txtstrm.h>

/*!
 * Control identifiers
 */

#define ID_FINDANDREPLACEWINDOW 10000
#define ID_FIND_COMBOBOX 10001
#define ID_REPLACE_COMBOBOX 10002
#define ID_CASE_SENSITIVE_CBOX 10003
#define ID_COMPLETE_WORDS_CBOX 10004
#define ID_REG_EX_CBOX 10005
#define ID_COMBOBOX2 10006
#define ID_CHECKBOX 10016
#define ID_FIND_IN_DIRECTORY_BTN 10007
#define ID_COMBOBOX3 10008
#define ID_FIND_BEFORE_BTN 10009
#define ID_FIND_NEXT_BTN 10010
#define ID_OMIT_FILE_BTN 10011
#define ID_REPLACE_NEXT_BTN 10012
#define ID_REPLACE_ALL 10013
#define ID_FIND_ALL 10014
#define ID_FIND_AND_REPLACE_RESULTS_TREECTRL 10015
#define SYMBOL_FINDANDREPLACEWINDOW_STYLE wxDV_SINGLE | wxNO_BORDER
#define SYMBOL_FINDANDREPLACEWINDOW_TITLE _("Find and Replace")
#define SYMBOL_FINDANDREPLACEWINDOW_IDNAME ID_FINDANDREPLACEWINDOW
#define SYMBOL_FINDANDREPLACEWINDOW_SIZE wxSize(640, 480)
#define SYMBOL_FINDANDREPLACEWINDOW_POSITION wxDefaultPosition

 // Only for pairs of std::hash-able types for simplicity.
 // You can of course template this struct to allow other hash functions
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;
    }
};

/*!
 * FindAndReplaceWindow class declaration
 */
class FindAndReplaceWindow :
	public wxPanel
{
public:
    /// Constructors
    FindAndReplaceWindow();
    FindAndReplaceWindow(wxWindow* parent, wxWindowID id = SYMBOL_FINDANDREPLACEWINDOW_IDNAME, const wxString& caption = SYMBOL_FINDANDREPLACEWINDOW_TITLE, const wxPoint& pos = SYMBOL_FINDANDREPLACEWINDOW_POSITION, const wxSize& size = SYMBOL_FINDANDREPLACEWINDOW_SIZE, long style = SYMBOL_FINDANDREPLACEWINDOW_STYLE);

    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_FINDANDREPLACEWINDOW_IDNAME, const wxString& caption = SYMBOL_FINDANDREPLACEWINDOW_TITLE, const wxPoint& pos = SYMBOL_FINDANDREPLACEWINDOW_POSITION, const wxSize& size = SYMBOL_FINDANDREPLACEWINDOW_SIZE, long style = SYMBOL_FINDANDREPLACEWINDOW_STYLE);

    /// Destructor
    ~FindAndReplaceWindow();

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
    class wxComboBox* m_FindComboBox;
    class wxComboBox* m_ReplaceComboBox;
    class wxButton* m_FindInDirectory;
    class wxDataViewCtrl* m_pSearchResultTreeCtrl;
    class wxComboBox* m_WhereToFind;
    wxObjectDataPtr<class SearchModel> m_searchModel;

    class MainFrame* m_pMainFrame;
private:
    void Find(const wxString &pattern, std::vector<wxStyledTextCtrl*> &inputBuffers, int flags, std::unordered_map<std::pair<int, int> /*line, column*/, wxString /*filename*/, pair_hash> &elementsFound  );

    // Event table methods
    void OnFindAllBtnClicked(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

