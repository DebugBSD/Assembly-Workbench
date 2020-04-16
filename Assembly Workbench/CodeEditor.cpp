#include "stdafx.h"
#include "CodeEditor.h"


wxBEGIN_EVENT_TABLE(CodeEditor, wxTextCtrl)
	EVT_TEXT_ENTER(ID_TextChanged, CodeEditor::TextChanged)
	//EVT_KEY_DOWN(CodeEditor::OnKeyDown)
	//EVT_KEY_UP(CodeEditor::OnKeyUp)
	//EVT_CHAR(CodeEditor::OnKeyChar)
wxEND_EVENT_TABLE()

CodeEditor::CodeEditor(wxWindow* parent):
    wxTextCtrl(parent, ID_TextChanged, wxEmptyString, { 0, 0 }, { 400, 400 }, wxTE_MULTILINE | wxTE_RICH | wxTE_RICH2 | wxTE_AUTO_URL | wxHSCROLL | wxTE_PROCESS_ENTER) // Base class
{
	wxFont f(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	SetFont(f);
	SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
	SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
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
	AppendText("Hola");
}
