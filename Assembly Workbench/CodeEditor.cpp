#include "stdafx.h"
#include "CodeEditor.h"

/*
wxBEGIN_EVENT_TABLE(CodeEditor, wxStyledTextCtrl)
	EVT_TEXT_ENTER(ID_TextChanged, CodeEditor::TextChanged)
	//EVT_KEY_DOWN(CodeEditor::OnKeyDown)
	//EVT_KEY_UP(CodeEditor::OnKeyUp)
	//EVT_CHAR(CodeEditor::OnKeyChar)
wxEND_EVENT_TABLE()*/

CodeEditor::CodeEditor(wxWindow* parent):
    wxStyledTextCtrl(parent, wxID_ANY, { 0, 0 }, { 1000, 1000 }) // Base class
{

    // Set the lexer to the C++ lexer
    SetLexer(wxSTC_LEX_ASM);

    // Set the color to use for various elements
    StyleSetForeground(wxSTC_C_COMMENTLINE, wxColor(60, 162, 2));
    StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColor(0, 0, 255));
    StyleSetForeground(wxSTC_C_STRING, wxColor(255, 60, 10));
    StyleSetForeground(wxSTC_C_WORD, wxColor(0, 0, 255));

    // Give a list of keywords. They will be given the style specified for
    // wxSTC_C_WORD items.
    SetKeyWords(0, wxT("return int char"));

    // Populate the wxStyledTextCtrl with a small C++ program
    AddText("; Hello world of assembly language\n\n");
    AddText("extrn ExitProcess:PROC\n");
    AddText(".DATA\n\n");
    AddText(".DATA?\n\n");
    AddText(".CODE\n");
    AddText("main PROC\n");
    AddText("\n\n\n");
    AddText("\tmov rcx, 0\n");
    AddText("\tcall ExitProcess\n");
    AddText("main ENDP\n");
    AddText("END\n");

    // Set up the sizer for the panel
    /*wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
    panelSizer->Add(styledTextCtrl, 1, wxEXPAND);
    SetSizer(panelSizer);

    // Set up the sizer for the frame and resize the frame
    // according to its contents
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(this, 1, wxEXPAND);
    SetSizerAndFit(topSizer);*/
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::TextChanged(wxCommandEvent& event)
{
    int stop = 1;
}

void CodeEditor::OnKeyDown(wxKeyEvent& event)
{
	int stop = 1;

	event.Skip(); // We need to call this event because if not, the event EVT_CHAR will not happen!
}

void CodeEditor::OnKeyUp(wxKeyEvent& event)
{
	int stop = 1;
}

void CodeEditor::OnKeyChar(wxKeyEvent& event)
{
	int stop = 1;
	//AppendText("Hola");
}
