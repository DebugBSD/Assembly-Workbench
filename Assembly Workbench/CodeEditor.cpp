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
#include "CodeEditor.h"
#include "File.h"


 // The (uniform) style used for the annotations.
const int ANNOTATION_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;

// A small image of a hashtag symbol used in the autocompletion window.
const char* hashtag_xpm[] = {
"10 10 2 1",
" 	c None",
".	c #BD08F9",
"  ..  ..  ",
"  ..  ..  ",
"..........",
"..........",
"  ..  ..  ",
"  ..  ..  ",
"..........",
"..........",
"  ..  ..  ",
"  ..  ..  " };


wxBEGIN_EVENT_TABLE(CodeEditor, wxStyledTextCtrl)
    EVT_SIZE(CodeEditor::OnSize)
    // edit
    EVT_MENU(wxID_CLEAR, CodeEditor::OnEditClear)
    EVT_MENU(wxID_CUT, CodeEditor::OnEditCut)
    EVT_MENU(wxID_COPY, CodeEditor::OnEditCopy)
    EVT_MENU(wxID_PASTE, CodeEditor::OnEditPaste)
    EVT_MENU(myID_INDENTINC, CodeEditor::OnEditIndentInc)
    EVT_MENU(myID_INDENTRED, CodeEditor::OnEditIndentRed)
    EVT_MENU(wxID_SELECTALL, CodeEditor::OnEditSelectAll)
    EVT_MENU(myID_SELECTLINE, CodeEditor::OnEditSelectLine)
    EVT_MENU(wxID_REDO, CodeEditor::OnEditRedo)
    EVT_MENU(wxID_UNDO, CodeEditor::OnEditUndo)

    // view
    EVT_MENU(ID_View_LineNumber, CodeEditor::OnLineNumber)
    EVT_MENU(ID_View_LongLine, CodeEditor::OnLongLineOn)
    EVT_MENU(ID_View_CaretLine, CodeEditor::OnCaretLineOn)

    // stc
    EVT_STC_MARGINCLICK(wxID_ANY, CodeEditor::OnMarginClick)
    EVT_STC_CHARADDED(wxID_ANY, CodeEditor::OnCharAdded)
    EVT_STC_CALLTIP_CLICK(wxID_ANY, CodeEditor::OnCallTipClick)
    EVT_STC_MODIFIED(wxID_ANY, CodeEditor::OnDocumentModified)
    EVT_STC_AUTOCOMP_CHAR_DELETED(wxID_ANY, CodeEditor::OnDocumentModified)

    EVT_KEY_UP(CodeEditor::OnKeyUp)
	EVT_KEY_DOWN(CodeEditor::OnKeyDown)
	EVT_LEFT_DOWN(CodeEditor::OnMouseDown)
	EVT_LEFT_UP(CodeEditor::OnMouseUp)
wxEND_EVENT_TABLE()

CodeEditor::CodeEditor(wxWindow* parent, File* pFile):
	wxStyledTextCtrl(parent, wxID_ANY, { 0, 0 }, parent->GetClientSize(), wxVSCROLL), // Base class
	m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) },
	m_pFile{pFile}
{
    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	/*SetFont(f);
    SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
	SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));*/

    m_calltipNo = 1;

    // miscellaneous
    m_LineNrMargin = TextWidth(wxSTC_STYLE_LINENUMBER, "_999999");
    m_FoldingMargin = 16;
    m_LineNrID = 0;
    m_DividerID = 1;
    m_FoldingID = 2;

    // initialize language
    m_language = NULL;

    // default font for all styles
    SetViewEOL(g_CommonPrefs.displayEOLEnable);
    SetIndentationGuides(g_CommonPrefs.indentGuideEnable);
    SetEdgeMode(g_CommonPrefs.longLineOnEnable ?
        wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
    SetViewWhiteSpace(g_CommonPrefs.whiteSpaceEnable ?
        wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
    SetOvertype(g_CommonPrefs.overTypeInitial);
    SetReadOnly(g_CommonPrefs.readOnlyInitial);
    SetWrapMode(g_CommonPrefs.wrapModeInitial ?
        wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
    //wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
    StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxBLACK);
    StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour("DARK GREY"));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour("DARK GREY"));
    InitializePrefs(DEFAULT_LANGUAGE);

    // set visibility
    SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
    SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

    // set caret visibility in current line
    SetCaretLineVisible(g_CommonPrefs.caretLineEnable ? true : false);
    SetCaretLineBackground(*wxYELLOW);

    // markers
    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_DOTDOTDOT, "BLACK", "BLACK");
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, "BLACK", "BLACK");
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, "BLACK", "BLACK");
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_DOTDOTDOT, "BLACK", "WHITE");
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, "BLACK", "WHITE");
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, "BLACK", "BLACK");
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, "BLACK", "BLACK");

    // annotations
    AnnotationSetVisible(wxSTC_ANNOTATION_BOXED);

    // autocompletion
    wxBitmap bmp(hashtag_xpm);
    RegisterImage(0, bmp);

    // call tips
    CallTipSetBackground(*wxYELLOW);
    CmdKeyClear(wxSTC_KEY_TAB, 0); // this is done by the menu accelerator key
    SetLayoutCache(wxSTC_CACHE_PAGE);
    UsePopUp(wxSTC_POPUP_ALL);
    
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::GetCursorPosition(size_t& lnPos, size_t& colPos)
{

}

void CodeEditor::SetCursorPosition(const EventType& evtType)
{
	switch (evtType)
	{
	case EventType::EVENT_LEFT:

		break;
	case EventType::EVENT_RIGHT:

		break;
	case EventType::EVENT_UP:

		break;
	case EventType::EVENT_DOWN:

		break;
	case EventType::EVENT_LMOUSE:

		break;
	}
}

void CodeEditor::OnSize(wxSizeEvent& event) {
    int x = GetClientSize().x +
        (g_CommonPrefs.lineNumberEnable ? m_LineNrMargin : 0) +
        (g_CommonPrefs.foldEnable ? m_FoldingMargin : 0);
    if (x > 0) SetScrollWidth(x);
    event.Skip();
}


void CodeEditor::OnKeyUp(wxKeyEvent& event)
{
    UpdateStatusBar();
}

void CodeEditor::OnKeyDown(wxKeyEvent& event)
{
    if (CallTipActive())
        CallTipCancel();
    if (event.GetKeyCode() == WXK_SPACE && event.ControlDown() && event.ShiftDown())
    {
        // Show our first call tip at the current position of the caret.
        m_calltipNo = 1;
        ShowCallTipAt(GetCurrentPos());
        return;
    }

    if (event.GetKeyCode() == WXK_TAB)
    {
        CmdKeyExecute(wxSTC_CMD_TAB);
    }
    event.Skip();
}


void CodeEditor::OnMouseDown(wxMouseEvent& event)
{
	event.Skip();
}

void CodeEditor::OnMouseUp(wxMouseEvent& event)
{
    UpdateStatusBar();
	event.Skip();
}


bool CodeEditor::InitializePrefs(const wxString& name) {

    // initialize styles
    StyleClearAll();
    LanguageInfo const* curInfo = NULL;

    // determine language
    bool found = false;
    int languageNr;
    for (languageNr = 0; languageNr < g_LanguagePrefsSize; languageNr++) {
        curInfo = &g_LanguagePrefs[languageNr];
        if (curInfo->name == name) {
            found = true;
            break;
        }
    }
    if (!found) return false;

    // set lexer and language
    SetLexer(curInfo->lexer);
    m_language = curInfo;

    // set margin for line numbers
    SetMarginType(m_LineNrID, wxSTC_MARGIN_NUMBER);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour("DARK GREY"));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, *wxWHITE);
    SetMarginWidth(m_LineNrID, g_CommonPrefs.lineNumberEnable ? m_LineNrMargin : 0); // start out not visible

    // annotations style
    StyleSetBackground(ANNOTATION_STYLE, wxColour(244, 220, 220));
    StyleSetForeground(ANNOTATION_STYLE, *wxBLACK);
    StyleSetSizeFractional(ANNOTATION_STYLE,
        (StyleGetSizeFractional(wxSTC_STYLE_DEFAULT) * 4) / 5);

    // default fonts for all styles!
    int Nr;
    for (Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++) {
        wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
        StyleSetFont(Nr, font);
    }

    // set common styles
    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour("DARK GREY"));
    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour("DARK GREY"));

    // initialize settings
    if (g_CommonPrefs.syntaxEnable) {
        int keywordnr = 0;
        for (Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++) {
            if (curInfo->styles[Nr].type == -1) continue;
            const StyleInfo& curType = g_StylePrefs[curInfo->styles[Nr].type];
            wxFont font(wxFontInfo(curType.fontsize)
                .Family(wxFONTFAMILY_MODERN)
                .FaceName(curType.fontname));
            StyleSetFont(Nr, font);
            if (curType.foreground.length()) {
                StyleSetForeground(Nr, wxColour(curType.foreground));
            }
            if (curType.background.length()) {
                StyleSetBackground(Nr, wxColour(curType.background));
            }
            StyleSetBold(Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
            StyleSetItalic(Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
            StyleSetUnderline(Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
            StyleSetVisible(Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
            StyleSetCase(Nr, curType.lettercase);
            const char* pwords = curInfo->styles[Nr].words;
            if (pwords) {
                SetKeyWords(keywordnr, pwords);
                keywordnr += 1;
            }
        }
    }

    // set margin as unused
    SetMarginType(m_DividerID, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth(m_DividerID, 0);
    SetMarginSensitive(m_DividerID, false);

    // folding
    SetMarginType(m_FoldingID, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(m_FoldingID, wxSTC_MASK_FOLDERS);
    StyleSetBackground(m_FoldingID, *wxWHITE);
    SetMarginWidth(m_FoldingID, 0);
    SetMarginSensitive(m_FoldingID, false);
    if (g_CommonPrefs.foldEnable) {
        SetMarginWidth(m_FoldingID, curInfo->folds != 0 ? m_FoldingMargin : 0);
        SetMarginSensitive(m_FoldingID, curInfo->folds != 0);
        SetProperty("fold", curInfo->folds != 0 ? "1" : "0");
        SetProperty("fold.comment",
            (curInfo->folds & mySTC_FOLD_COMMENT) > 0 ? "1" : "0");
        SetProperty("fold.compact",
            (curInfo->folds & mySTC_FOLD_COMPACT) > 0 ? "1" : "0");
        SetProperty("fold.preprocessor",
            (curInfo->folds & mySTC_FOLD_PREPROC) > 0 ? "1" : "0");
        SetProperty("fold.html",
            (curInfo->folds & mySTC_FOLD_HTML) > 0 ? "1" : "0");
        SetProperty("fold.html.preprocessor",
            (curInfo->folds & mySTC_FOLD_HTMLPREP) > 0 ? "1" : "0");
        SetProperty("fold.comment.python",
            (curInfo->folds & mySTC_FOLD_COMMENTPY) > 0 ? "1" : "0");
        SetProperty("fold.quotes.python",
            (curInfo->folds & mySTC_FOLD_QUOTESPY) > 0 ? "1" : "0");
    }
    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
        wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    // set spaces and indentation
    SetTabWidth(4);
    SetUseTabs(false);
    SetTabIndents(true);
    SetBackSpaceUnIndents(true);
    SetIndent(g_CommonPrefs.indentEnable ? 4 : 0);

    // others
    SetViewEOL(g_CommonPrefs.displayEOLEnable);
    SetIndentationGuides(g_CommonPrefs.indentGuideEnable);
    SetEdgeColumn(80);
    SetEdgeMode(g_CommonPrefs.longLineOnEnable ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
    SetViewWhiteSpace(g_CommonPrefs.whiteSpaceEnable ?
        wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE);
    SetOvertype(g_CommonPrefs.overTypeInitial);
    SetReadOnly(g_CommonPrefs.readOnlyInitial);
    SetWrapMode(g_CommonPrefs.wrapModeInitial ?
        wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);

    return true;
}

//----------------------------------------------------------------------------
// private functions
void CodeEditor::ShowCallTipAt(int position)
{
    // In a call tip string, the character '\001' will become a clickable
    // up arrow and '\002' will become a clickable down arrow.
    wxString ctString = wxString::Format("\001 %d of 3 \002 ", m_calltipNo);
    if (m_calltipNo == 1)
        ctString += "This is a call tip. Try clicking the up or down buttons.";
    else if (m_calltipNo == 2)
        ctString += "It is meant to be a context sensitive popup helper for "
        "the user.";
    else
        ctString += "This is a call tip with multiple lines.\n"
        "You can provide slightly longer help with "
        "call tips like these.";

    if (CallTipActive())
        CallTipCancel();
    CallTipShow(position, ctString);
}

void CodeEditor::UpdateStatusBar()
{
    m_pMainFrame->SetStatusBar(GetTextLength(), GetLineCount(), GetColumn(GetCurrentPos()), GetCurrentLine() + 1);
}

void CodeEditor::OnEditClear(wxCommandEvent& WXUNUSED(event)) {
    if (GetReadOnly()) return;
    Clear();
}


void CodeEditor::OnEditCut(wxCommandEvent& WXUNUSED(event)) {
    if (GetReadOnly() || (GetSelectionEnd() - GetSelectionStart() <= 0)) return;
    Cut();
}

void CodeEditor::OnEditCopy(wxCommandEvent& WXUNUSED(event)) {
    if (GetSelectionEnd() - GetSelectionStart() <= 0) return;
    Copy();
}

void CodeEditor::OnEditPaste(wxCommandEvent& WXUNUSED(event)) {
    if (!CanPaste()) return;
    Paste();
}

void CodeEditor::OnEditIndentInc(wxCommandEvent& WXUNUSED(event)) {
    CmdKeyExecute(wxSTC_CMD_TAB);
}

void CodeEditor::OnEditIndentRed(wxCommandEvent& WXUNUSED(event)) {
    CmdKeyExecute(wxSTC_CMD_DELETEBACK);
}

void CodeEditor::OnEditSelectAll(wxCommandEvent& WXUNUSED(event)) {
    SetSelection(0, GetTextLength());
}

void CodeEditor::OnEditSelectLine(wxCommandEvent& WXUNUSED(event)) {
    int lineStart = PositionFromLine(GetCurrentLine());
    int lineEnd = PositionFromLine(GetCurrentLine() + 1);
    SetSelection(lineStart, lineEnd);
}

void CodeEditor::OnEditRedo(wxCommandEvent& WXUNUSED(event)) {
    if (!CanRedo()) return;
    Redo();
}

void CodeEditor::OnEditUndo(wxCommandEvent& WXUNUSED(event)) {
    if (!CanUndo()) return;
    Undo();
}

void CodeEditor::OnLineNumber(wxCommandEvent& event)
{
    SetMarginWidth(m_LineNrID,
        GetMarginWidth(m_LineNrID) == 0 ? m_LineNrMargin : 0);
}

void CodeEditor::OnLongLineOn(wxCommandEvent& WXUNUSED(event)) {
    SetEdgeMode(GetEdgeMode() == 0 ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE);
}

void CodeEditor::OnCaretLineOn(wxCommandEvent& WXUNUSED(event)) {
    SetCaretLineVisible(GetCaretLineVisible() == true ? false : true);
}

void CodeEditor::OnMarginClick(wxStyledTextEvent& event) {
    if (event.GetMargin() == 2) {
        int lineClick = LineFromPosition(event.GetPosition());
        int levelClick = GetFoldLevel(lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold(lineClick);
        }
    }
}

void CodeEditor::OnCharAdded(wxStyledTextEvent& event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
    // Change this if support for mac files with \r is needed
    if (chr == '\n') {
        int lineInd = 0;
        if (currentLine > 0) {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation(currentLine, lineInd);
        GotoPos(PositionFromLine(currentLine) + lineInd);
    }
    /*else if (chr == '%') {
        wxString s = "define?0 elif?0 else?0 endif?0 error?0 if?0 ifdef?0 "
            "ifndef?0 include?0 line?0 pragma?0 undef?0";
        AutoCompShow(0, s);
    }*/
    UpdateStatusBar();
}

void CodeEditor::OnDocumentModified(wxStyledTextEvent& event)
{
    UpdateStatusBar();
}

void CodeEditor::OnCallTipClick(wxStyledTextEvent& event)
{
    if (event.GetPosition() == 1) {
        // If position=1, the up arrow has been clicked. Show the next tip.
        m_calltipNo = m_calltipNo == 3 ? 1 : (m_calltipNo + 1);
        ShowCallTipAt(CallTipPosAtStart());
    }
    else if (event.GetPosition() == 2) {
        // If position=2, the down arrow has been clicked. Show previous tip.
        m_calltipNo = m_calltipNo == 1 ? 3 : (m_calltipNo - 1);
        ShowCallTipAt(CallTipPosAtStart());
    }
}
