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

#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>

#include "Languages.h"
#include "Main.h"

enum
{
	ID_TextChanged = 1,
	ID_KeyEnter,// menu IDs
    myID_PROPERTIES = wxID_HIGHEST,
    myID_EDIT_FIRST,
    myID_INDENTINC = myID_EDIT_FIRST,
    myID_INDENTRED,
    myID_FINDNEXT,
    myID_REPLACE,
    myID_REPLACENEXT,
    myID_BRACEMATCH,
    myID_GOTO,
    myID_PAGEACTIVE,
    myID_DISPLAYEOL,
    myID_INDENTGUIDE,
    myID_LONGLINEON,
    myID_WHITESPACE,
    myID_FOLDTOGGLE,
    myID_OVERTYPE,
    myID_READONLY,
    myID_WRAPMODEON,
    myID_ANNOTATION_ADD,
    myID_ANNOTATION_REMOVE,
    myID_ANNOTATION_CLEAR,
    myID_ANNOTATION_STYLE_HIDDEN,
    myID_ANNOTATION_STYLE_STANDARD,
    myID_ANNOTATION_STYLE_BOXED,
    myID_CHANGECASE,
    myID_CHANGELOWER,
    myID_CHANGEUPPER,
    myID_HIGHLIGHTLANG,
    myID_HIGHLIGHTFIRST,
    myID_HIGHLIGHTLAST = myID_HIGHLIGHTFIRST + 99,
    myID_CONVERTEOL,
    myID_CONVERTCR,
    myID_CONVERTCRLF,
    myID_CONVERTLF,
    myID_MULTIPLE_SELECTIONS,
    myID_MULTI_PASTE,
    myID_MULTIPLE_SELECTIONS_TYPING,
    myID_CUSTOM_POPUP,
    myID_USECHARSET,
    myID_CHARSETANSI,
    myID_CHARSETMAC,
    myID_PAGEPREV,
    myID_PAGENEXT,
    myID_SELECTLINE,
    myID_EDIT_LAST = myID_SELECTLINE,
    myID_WINDOW_MINIMAL,

    // other IDs
    myID_STATUSBAR,
    myID_TITLEBAR,
    myID_ABOUTTIMER,
    myID_UPDATETIMER,

    // dialog find IDs
    myID_DLG_FIND_TEXT,

    // preferences IDs
    myID_PREFS_LANGUAGE,
    myID_PREFS_STYLETYPE,
    myID_PREFS_KEYWORDS,
};

enum class EventType
{
	EVENT_NONE = -1,
	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_UP,
	EVENT_DOWN,
	EVENT_LMOUSE
    
};

class CodeEditor :
	public wxStyledTextCtrl
{
public:
	CodeEditor(wxWindow* parent, class File *pFile);
	~CodeEditor();

    const class File* GetFile() const { return m_pFile; }
    class File* GetFile() { return m_pFile; }
    void SetFile(class File* pFile) { m_pFile = pFile; }
    void RemoveFile(class File* pFile) { if (m_pMainFrame) m_pMainFrame->RemoveFile(pFile); }

private:
	MainFrame* m_pMainFrame;

    class File* m_pFile;

    // language properties
    LanguageInfo const* m_language;

    // margin variables
    int m_LineNrID;
    int m_LineNrMargin;
    int m_FoldingID;
    int m_FoldingMargin;
    int m_DividerID;

    // call tip data
    int m_calltipNo;
private:

	void GetCursorPosition(size_t &lnPos, size_t &colPos);
	void SetCursorPosition(const EventType &evtType = EventType::EVENT_NONE);

	void OnKeyDown(wxKeyEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);

    // event handlers
    // common
    void OnSize(wxSizeEvent& event);
    // edit
    void OnEditRedo(wxCommandEvent& event);
    void OnEditUndo(wxCommandEvent& event);
    void OnEditClear(wxCommandEvent& event);
    void OnEditCut(wxCommandEvent& event);
    void OnEditCopy(wxCommandEvent& event);
    void OnEditPaste(wxCommandEvent& event);
    // find
    void OnFind(wxCommandEvent& event);
    void OnFindNext(wxCommandEvent& event);
    void OnReplace(wxCommandEvent& event);
    void OnReplaceNext(wxCommandEvent& event);
    void OnBraceMatch(wxCommandEvent& event);
    void OnGoto(wxCommandEvent& event);
    void OnEditIndentInc(wxCommandEvent& event);
    void OnEditIndentRed(wxCommandEvent& event);
    void OnEditSelectAll(wxCommandEvent& event);
    void OnEditSelectLine(wxCommandEvent& event);
    //! view
    void OnHighlightLang(wxCommandEvent& event);
    void OnDisplayEOL(wxCommandEvent& event);
    void OnIndentGuide(wxCommandEvent& event);
    void OnLineNumber(wxCommandEvent& event);
    void OnLongLineOn(wxCommandEvent& event);
    void OnWhiteSpace(wxCommandEvent& event);
    void OnFoldToggle(wxCommandEvent& event);
    void OnSetOverType(wxCommandEvent& event);
    void OnSetReadOnly(wxCommandEvent& event);
    void OnWrapmodeOn(wxCommandEvent& event);
    void OnUseCharset(wxCommandEvent& event);
    // annotations
    void OnAnnotationAdd(wxCommandEvent& event);
    void OnAnnotationRemove(wxCommandEvent& event);
    void OnAnnotationClear(wxCommandEvent& event);
    void OnAnnotationStyle(wxCommandEvent& event);
    //! extra
    void OnChangeCase(wxCommandEvent& event);
    void OnConvertEOL(wxCommandEvent& event);
    void OnMultipleSelections(wxCommandEvent& event);
    void OnMultiPaste(wxCommandEvent& event);
    void OnMultipleSelectionsTyping(wxCommandEvent& event);
    void OnCustomPopup(wxCommandEvent& evt);
    // stc
    void OnMarginClick(wxStyledTextEvent& event);
    void OnCharAdded(wxStyledTextEvent& event);
    void OnCallTipClick(wxStyledTextEvent& event);

    // call tips
    void ShowCallTipAt(int position);

    //! language/lexer
    wxString DeterminePrefs(const wxString& filename);
    bool InitializePrefs(const wxString& filename);
    bool UserSettings(const wxString& filename);
    LanguageInfo const* GetLanguageInfo() { return m_language; }

    //! load/save file
    /*bool LoadFile();
    bool LoadFile(const wxString& filename);
    bool SaveFile();
    bool SaveFile(const wxString& filename);
    bool Modified();
    wxString GetFilename() { return m_filename; }
    void SetFilename(const wxString& filename) { m_filename = filename; }
    */
	wxDECLARE_EVENT_TABLE();
};
